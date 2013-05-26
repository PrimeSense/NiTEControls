/*****************************************************************************
*                                                                            *
*  NiTE Controls 1.x Alpha                                                   *
*  Copyright (C) 2013 PrimeSense Ltd.                                        *
*                                                                            *
*  This file is part of NiTE Controls Lab.                                   *
*                                                                            *
*  Licensed under the Apache License, Version 2.0 (the "License");           *
*  you may not use this file except in compliance with the License.          *
*  You may obtain a copy of the License at                                   *
*                                                                            *
*      http://www.apache.org/licenses/LICENSE-2.0                            *
*                                                                            *
*  Unless required by applicable law or agreed to in writing, software       *
*  distributed under the License is distributed on an "AS IS" BASIS,         *
*  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.  *
*  See the License for the specific language governing permissions and       *
*  limitations under the License.                                            *
*                                                                            *
*****************************************************************************/
#include <XnStringsHash.h>
#include "XnVNiteLog.h"
#include "XnVPointArea.h"
#include "XnVPointMessage.h"
#include "XnVNiteStatus.h"
#include "XnV3DVector.h"
#include "XnVNiteEvents.h"

XnVPointArea::XnVPointArea(const XnBoundingBox3D& bbArea, XnBool bRelative, const XnChar* strName) :
	XnVPointFilter(strName),
	m_bRemoveByGenerator(false),
	m_bRemoveByTracker(false),
	m_pRemovingGenerator(NULL),
	m_pRemovingTracker(NULL),
	m_bRelative(bRelative)
{
	ChangeArea(bbArea, bRelative);
	m_pPointSilencedCBs = XN_NEW(XnVUintSpecificEvent);
	m_pPointRevivedCBs = XN_NEW(XnVUintSpecificEvent);
	m_pSilentPointRemovedCBs = XN_NEW(XnVUintSpecificEvent);
	m_pSilentPoints = XN_NEW(XnVIntHash);
} // XnVPointArea::XnVPointArea

XnVPointArea::XnVPointArea(const XnPoint3D& ptMins, const XnPoint3D& ptMaxs, XnBool bRelative, const XnChar* strName) :
	XnVPointFilter(strName),
	m_bRemoveByGenerator(false),
	m_bRemoveByTracker(false),
	m_pRemovingGenerator(NULL),
	m_pRemovingTracker(NULL),
	m_bRelative(bRelative)
{
	ChangeArea(ptMins, ptMaxs, bRelative);
	m_pPointSilencedCBs = XN_NEW(XnVUintSpecificEvent);
	m_pPointRevivedCBs = XN_NEW(XnVUintSpecificEvent);
	m_pSilentPointRemovedCBs = XN_NEW(XnVUintSpecificEvent);
	m_pSilentPoints = XN_NEW(XnVIntHash);
} // XnVPointArea::XnVPointArea

XnVPointArea::~XnVPointArea()
{
	XN_DELETE(m_pSilentPoints);
	XN_DELETE(m_pPointSilencedCBs);
	XN_DELETE(m_pPointRevivedCBs);
	XN_DELETE(m_pSilentPointRemovedCBs);
}

void XnVPointArea::ChangeArea(const XnBoundingBox3D &bbArea, XnBool bRelative)
{
	XnAutoCSLocker local(m_hListenerCS);
	m_bRelative = bRelative;
	if (bRelative)
	{
		ChangeRelativeArea(bbArea);
	}
	else
	{
		ChangeConstantArea(bbArea);
	}
} // XnVPointArea::ChangeArea

void XnVPointArea::ChangeArea(const XnPoint3D& ptMins, const XnPoint3D& ptMaxs, XnBool bRelative)
{
	XnAutoCSLocker local(m_hListenerCS);
	XnBoundingBox3D bb;
	bb.LeftBottomNear = ptMins;
	bb.RightTopFar = ptMaxs;

	m_bRelative = bRelative;
	if (bRelative)
	{
		ChangeRelativeArea(bb);
	}
	else
	{
		ChangeConstantArea(bb);
	}
} // XnVPointArea::ChangeArea

void XnVPointArea::ChangeConstantArea(const XnBoundingBox3D& bbArea)
{
	m_bbArea = bbArea;
} // XnVPointArea::ChangeConstantArea

void XnVPointArea::ChangeRelativeArea(const XnBoundingBox3D& bbArea)
{
	m_bbRelativeArea = bbArea;
} // XnVPointArea::ChangeRelativeArea

XnBool XnVPointArea::InBoundingBox(const XnPoint3D& pt) const
{
	if (pt.X < m_bbArea.LeftBottomNear.X ||
		pt.Y < m_bbArea.LeftBottomNear.Y ||
		pt.Z < m_bbArea.LeftBottomNear.Z ||
		pt.X > m_bbArea.RightTopFar.X ||
		pt.Y > m_bbArea.RightTopFar.Y ||
		pt.Z > m_bbArea.RightTopFar.Z)
	{
		return FALSE;
	}

	return TRUE;
} // XnVPointArea::InBoundingBox

XnBool XnVPointArea::IsSilent(XnUInt32 nID) const
{
	XnUInt32 nValue = 0;
	return m_pSilentPoints->Get(nID, nValue) == XN_STATUS_OK;
} // XnVPointArea::IsSilent

void XnVPointArea::SilencePoint(XnUInt32 nID, XnFloat fTime)
{
	m_pSilentPoints->Set(nID, XnUInt32(fTime*1000));
} // XnVPointArea::SilencePoint

void XnVPointArea::RevivePoint(XnUInt32 nID)
{
	m_pSilentPoints->Remove(nID);
} // XnVPointArea::RevivePoint

XnStatus XnVPointArea::CheckDelay(XnUInt32 nID, XnFloat fTime)
{
	if (!m_bRemoveByGenerator && !m_bRemoveByTracker)
	{
		return XN_STATUS_NITE_SHOULDNT_REMOVE_POINT;
	}
	if ((m_bRemoveByTracker && m_pRemovingTracker == NULL) ||
		(m_bRemoveByGenerator && m_pRemovingGenerator == NULL))
	{
		return XN_STATUS_NITE_CANT_REMOVE_POINT;
	}

	XnUInt32 nValue;
	if (m_pSilentPoints->Get(nID, nValue) != XN_STATUS_OK)
	{
		return XN_STATUS_NITE_NO_SUCH_POINT;
	}

	XnUInt32 nStartTime = nValue;

 	XnUInt32 nNow = XnUInt32(fTime * 1000);
	if (nNow - nStartTime > m_nRemovalDelay)
	{
		if (m_bRemoveByTracker)
		{
			m_pRemovingTracker->StopTracking(nID);
			xnLogVerbose(XNV_NITE_MASK_EVENTS, "Point Area %s [0x%08x]: Requesting removal of silent point %d from PointTracker %s [0x%08x].",
				GetGeneratorName(), this, nID,
				m_pRemovingTracker->GetGeneratorName(), m_pRemovingTracker);
		}
		else if (m_bRemoveByGenerator)
		{
			m_pRemovingGenerator->LosePoint(nID);
			xnLogVerbose(XNV_NITE_MASK_EVENTS, "Point Area %s [0x%08x]: Requesting removal of silent point %d from Generator %s [0x%08x].",
				GetGeneratorName(), this, nID,
				m_pRemovingGenerator->GetGeneratorName(), m_pRemovingGenerator);
		}
	}

	return XN_STATUS_OK;
} // XnVPointArea::CheckDelay

void XnVPointArea::Update(XnVMessage* pMessage)
{
	XnVPointControl::Update(pMessage);

	GenerateReplaced(pMessage, m_FilteredHands);
} // XnVPointArea::Update

/**
 * Each time points are received, we make sure they are in the relevant area.
 * There are 4 possible state for each point:
 * 1. Point was previously in the area, and remains there. This point is reported as is to all listeners.
 * 2. Point was previously in the area, but is no longer there. This point is silenced. This means all listeners will be informed that the point was destroyed.
 * 3. Point was previously silent, and is still outside the area. This point continues to be silenced, and listeners will get no information about it.
 * 4. Point was previously silent, but is now in the area. This point is revived - it is no longer silent. All listeners will be informed of the point creation.
 *
 * Silent points may be configured to be really removed from a PointTracker of SessionGenerator.
 */
void XnVPointArea::Update(const XnVMultipleHands& hands)
{
	m_FilteredHands.Clear();
	XnVPointControl::Update(hands); // For callbacks

	// Go over all 'old' IDs
	for (XnVMultipleHands::ConstIteratorOld iter = hands.beginOld(); iter != hands.endOld(); ++iter)
	{
		XnUInt32 nID = XnUInt32(*iter);
		if (IsSilent(nID))
		{
			// 'Old' ID that is internally silenced.
			RevivePoint(nID);
			m_pSilentPointRemovedCBs->Raise(nID);
		}
		else
		{
			// Mark ID as 'Old' in our Multiple Hands too
			m_FilteredHands.MarkOld(nID);
		}
	}

	XnVIntList silentPoints, revivedPoints;

	if (m_bRelative && hands.GetPrimaryContext() != NULL)
	{
		if (hands.IsNew(hands.GetPrimaryContext()->nID))
		{
			// Single point is new!
			XnV3DVector v(hands.GetFocusPoint());
			XnBoundingBox3D bb;
			bb.LeftBottomNear = v+m_bbRelativeArea.LeftBottomNear;
			bb.RightTopFar = v+m_bbRelativeArea.RightTopFar;

			ChangeConstantArea(bb);
			xnLogVerbose(XNV_NITE_MASK_EVENTS, "Point Area %s [0x%08x]: Bounding Box changed. Is now: (%5.2f, %5.2f, %5.2f)-(%5.2f, %5.2f, %5.2f).",
				GetGeneratorName(), this,
				m_bbArea.LeftBottomNear.X, m_bbArea.LeftBottomNear.Y, m_bbArea.LeftBottomNear.Z, 
				m_bbArea.RightTopFar.X, m_bbArea.RightTopFar.Y, m_bbArea.RightTopFar.Z);
		}
	}

	// Go over all active points
	for (XnVMultipleHands::ConstIterator hiter = hands.begin(); hiter != hands.end(); ++hiter)
	{
		XnUInt32 nID = (*hiter)->nID;
		
		// Check if point is legal
		if (InBoundingBox((*hiter)->ptPosition))
		{
			// Legal point
			m_FilteredHands.Add(*hiter);
			if (hiter.IsActive())
			{
				m_FilteredHands.MarkActive(nID);
			}
			if (hiter.IsNew())
			{
				// Points that are 'New' in the original list are also 'New' in ours.
				m_FilteredHands.MarkNew(nID);
			}
			if (IsSilent(nID))
			{
				// Illegal point turned legal
				RevivePoint(nID);
				// Mark it as 'New' in our list
				m_FilteredHands.MarkNew(nID);
				revivedPoints.AddLast(nID);
				xnLogVerbose(XNV_NITE_MASK_EVENTS, "Point Area %s [0x%08x]: Point %d (%5.2f, %5.2f, %5.2f) returned to the area: (%5.2f, %5.2f, %5.2f)-(%5.2f, %5.2f, %5.2f). Is now revived.",
					GetGeneratorName(), this, (*hiter)->nID,
					(*hiter)->ptPosition.X, (*hiter)->ptPosition.Y, (*hiter)->ptPosition.Z,
					m_bbArea.LeftBottomNear.X, m_bbArea.LeftBottomNear.Y, m_bbArea.LeftBottomNear.Z, 
					m_bbArea.RightTopFar.X, m_bbArea.RightTopFar.Y, m_bbArea.RightTopFar.Z);
			}
		} // if (InBoundingBox(...
		else
		{
			// Illegal point
			if (!IsSilent(nID))
			{
				// was legal until now...
				// Mark it as 'Old' in our list
				m_FilteredHands.MarkOld(nID);
				SilencePoint(nID, (*hiter)->fTime);
				silentPoints.AddLast(nID);
				xnLogVerbose(XNV_NITE_MASK_EVENTS, "Point Area %s [0x%08x]: Point %d (%5.2f, %5.2f, %5.2f) outside the area: (%5.2f, %5.2f, %5.2f)-(%5.2f, %5.2f, %5.2f). Will now be silenced.",
					GetGeneratorName(), this, (*hiter)->nID,
					(*hiter)->ptPosition.X, (*hiter)->ptPosition.Y, (*hiter)->ptPosition.Z,
					m_bbArea.LeftBottomNear.X, m_bbArea.LeftBottomNear.Y, m_bbArea.LeftBottomNear.Z, 
					m_bbArea.RightTopFar.X, m_bbArea.RightTopFar.Y, m_bbArea.RightTopFar.Z);
			}
			else
			{
				// Stays illegal - check time delay
				CheckDelay(nID, (*hiter)->fTime);
			}
		}
	} // for

	// Call callbacks for revived points
	for (XnVIntList::Iterator iter = revivedPoints.begin(); iter != revivedPoints.end(); ++iter)
	{
		m_pPointRevivedCBs->Raise(*iter);
	}

	// Pick Single Interesting Point, the same as the original if possible
	const XnVHandPointContext* pPrimary = hands.GetPrimaryContext();
	XnUInt32 nSuggestedID = 0;
	if (pPrimary != NULL)
	{
		nSuggestedID = pPrimary->nID;
	}
	m_FilteredHands.ReassignPrimary(nSuggestedID);

	// Call callbacks for silenced points
	for (XnVIntList::Iterator iter = silentPoints.begin(); iter != silentPoints.end(); ++iter)
	{
		m_pPointSilencedCBs->Raise(*iter);
	}

	m_FilteredHands.SetFocusPoint(hands.GetFocusPoint());
} // XnVPointArea::Update

XnCallbackHandle XnVPointArea::RegisterPointSilenced(void* cxt, PointSilencedCB CB)
{
	XnCallbackHandle handle;
	m_pPointSilencedCBs->Register(CB, cxt, &handle);
	return handle;
} // XnVPointArea::RegisterPointSilenced

XnCallbackHandle XnVPointArea::RegisterPointRevived(void* cxt, PointRevivedCB CB)
{
	XnCallbackHandle handle;
	m_pPointRevivedCBs->Register(CB, cxt, &handle);
	return handle;
} // XnVPointArea::RegisterPointRevived

XnCallbackHandle XnVPointArea::RegisterSilentPointRemoved(void* cxt, SilentPointRemovedCB CB)
{
	XnCallbackHandle handle;
	m_pSilentPointRemovedCBs->Register(CB, cxt, &handle);
	return handle;
} // XnVPointArea::RegisterSilentPointRemoved

void XnVPointArea::UnregisterPointSilenced(XnCallbackHandle hCB)
{
	m_pPointSilencedCBs->Unregister(hCB);
} // XnVPointArea::UnregisterPointSilenced

void XnVPointArea::UnregisterPointRevived(XnCallbackHandle hCB)
{
	m_pPointRevivedCBs->Unregister(hCB);
} // XnVPointArea::UnregisterPointRevived

void XnVPointArea::UnregisterSilentPointRemoved(XnCallbackHandle hCB)
{
	m_pSilentPointRemovedCBs->Unregister(hCB);
} // XnVPointArea::UnregisterSilentPointRemoved

void XnVPointArea::RemoveSilent(XnUInt64 nDelay, XnVSessionGenerator* pGenerator)
{
	XnAutoCSLocker local(m_hListenerCS);
	m_bRemoveByGenerator = true;
	m_bRemoveByTracker = false;
	m_pRemovingTracker = NULL;
	m_pRemovingGenerator = pGenerator;

	m_nRemovalDelay = nDelay;
} // XnVPointArea::RemoveSilent

void XnVPointArea::RemoveSilent(XnUInt64 nDelay, XnVPointTracker* pTracker)
{
	XnAutoCSLocker local(m_hListenerCS);
	m_bRemoveByGenerator = false;
	m_bRemoveByTracker = true;
	m_pRemovingTracker = pTracker;
	m_pRemovingGenerator = NULL;

	m_nRemovalDelay = nDelay;
} // XnVPointArea::RemoveSilent
