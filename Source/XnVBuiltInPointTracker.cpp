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
#include "XnVBuiltInPointTracker.h"
#include "XnVPointMessage.h"
#include "XnVNiteLog.h"
#include "XnVNiteEvents.h"

void XN_CALLBACK_TYPE XnVBuiltInPointTracker::HandCreate(xn::HandsGenerator& generator, XnUserID nId, const XnPoint3D* pPosition, XnFloat fTime, void* pCookie)
{
	XnVBuiltInPointTracker* pTracker = (XnVBuiltInPointTracker*)pCookie;

	pTracker->PointCreate(nId, *pPosition, fTime);
}
void XN_CALLBACK_TYPE XnVBuiltInPointTracker::HandUpdate(xn::HandsGenerator& generator, XnUserID nId, const XnPoint3D* pPosition, XnFloat fTime, void* pCookie)
{
	XnVBuiltInPointTracker* pTracker = (XnVBuiltInPointTracker*)pCookie;

	pTracker->PointUpdate(nId, *pPosition, fTime);
}
void XN_CALLBACK_TYPE XnVBuiltInPointTracker::HandDestroy(xn::HandsGenerator& generator, XnUserID nId, XnFloat fTime, void* pCookie)
{
	XnVBuiltInPointTracker* pTracker = (XnVBuiltInPointTracker*)pCookie;

	pTracker->PointDestroy(nId);
}



XnVBuiltInPointTracker::XnVBuiltInPointTracker(xn::HandsGenerator handsGenerator) :
	XnVPointTracker("Built-In Tracker"),
	m_HandsGenerator(handsGenerator)
{
	m_HandsGenerator.RegisterHandCallbacks(HandCreate, HandUpdate, HandDestroy, this, m_hCallbacks);
	m_nLastUsed = 0;
} // XnVBuiltInPointTracker::XnVBuiltInPointTracker

XnVBuiltInPointTracker::~XnVBuiltInPointTracker()
{
	Clear();
	m_HandsGenerator.UnregisterHandCallbacks(m_hCallbacks);
} // XnVBuiltInPointTracker::~XnVBuiltInPointTracker

XnStatus XnVBuiltInPointTracker::StartTracking(const XnPoint3D &ptPosition)
{
	m_HandsGenerator.StartTracking(ptPosition);
	m_Hands.SetFocusPoint(ptPosition);

	return XN_STATUS_OK;
} // XnVBuiltInPointTracker::StartTracking

XnStatus XnVBuiltInPointTracker::StopTracking(XnUInt32 nID)
{
	if (m_Hands.Find(nID) == m_Hands.end())
	{
		// No such ID
		return XN_STATUS_ERROR;
	}

	return XnVPointTracker::StopTracking(nID);
} // XnVBuiltInPointTracker::StopTracking

XnStatus XnVBuiltInPointTracker::StopTracking()
{
	for (XnVMultipleHands::Iterator iter = m_Hands.begin(); iter != m_Hands.end(); ++iter)
	{
		StopTracking((*iter)->nID);
	}

	return XN_STATUS_OK;
} // XnVBuiltInPointTracker::StopTracking

void XnVBuiltInPointTracker::LosePoints()
{
	for (XnVIntList::Iterator iter = m_pLostPoints->begin(); iter != m_pLostPoints->end(); ++iter)
	{
		XnUInt32 nID = *iter;
		m_HandsGenerator.StopTracking(nID);
	}
	m_pLostPoints->Clear();
} // XnVBuiltInPointTracker::LosePoints

void XnVBuiltInPointTracker::Update(const xn::Context* pContext)
{
	XnUInt64 nNow = m_HandsGenerator.GetTimestamp();
	if (nNow == m_nLastUsed)
	{
		// Already sent message relevant to this time
		return;
	}
	m_nLastUsed = nNow;

	XnVPointMessage pointMessage(&m_Hands);
	Generate(&pointMessage);

	m_Hands.ClearOldList();
	m_Hands.ClearNewList();

	LosePoints();

	XnVIntList removalList;

	for (XnVMultipleHands::Iterator iter = m_Hands.begin(); iter != m_Hands.end(); ++iter)
	{
		if ((*iter)->fConfidence > 0.03f)
		{
			(*iter)->fConfidence -= 0.03f;
		}
		else
		{
			removalList.AddLast((*iter)->nID);
		}
	}

	for (XnVIntList::Iterator iter = removalList.begin(); iter != removalList.end(); ++iter)
	{
		XnUInt32 nID = *iter;
		PointDestroy(nID);
	}
} // XnVBuiltInPointTracker::Update

void XnVBuiltInPointTracker::PointCreate(XnUInt32 nID, const XnPoint3D& ptPosition, XnDouble fTime)
{
	xnLogVerbose(XNV_NITE_MASK_POINTS, "Generator %s [0x%08x]: Point Created (%d) (%f,%f,%f) {%lf}",
		m_strGeneratorName, this, nID, ptPosition.X, ptPosition.Y, ptPosition.Z, fTime);
	PointUpdate(nID, ptPosition, fTime);
	m_Hands.MarkNew(nID);
} // XnVBuiltInPointTracker::PointCreate

void XnVBuiltInPointTracker::PointUpdate(XnUInt32 nID, const XnPoint3D& ptPosition, XnDouble fTime)
{
	XnVHandPointContext* pContext = m_Hands.GetContext(nID);
	if (pContext == NULL)
	{
		XnVHandPointContext hpc;
		hpc.nID = nID;
		hpc.nUserID = nID;
		m_Hands.Add(&hpc);

		// This should actually never happen unless it's called from PointUpdate. Just in case, though:
		m_Hands.MarkNew(nID);

		pContext = m_Hands.GetContext(nID);
	}
	pContext->ptPosition = ptPosition;
	pContext->fTime = XnFloat(fTime);
	pContext->fConfidence = 1;

	m_Hands.MarkActive(nID);
} // XnVBuiltInPointTracker::PointUpdate

void XnVBuiltInPointTracker::PointDestroy(XnUInt32 nID)
{
	xnLogVerbose(XNV_NITE_MASK_POINTS, "Generator %s [0x%08x]: Point Destroyed (%d)",
		m_strGeneratorName, this, nID);
	m_Hands.Remove(nID);

	m_Hands.MarkOld(nID);
	m_Hands.RemoveActive(nID);
} // XnVBuiltInPointTracker::PointDestroy

void XnVBuiltInPointTracker::Clear()
{
	m_Hands.Clear();
} // XnVBuiltInPointTracker::Clear
