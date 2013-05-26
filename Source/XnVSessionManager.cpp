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
#include <XnOSCpp.h>

#include "XnVSessionManager.h"
#include "XnVNiteLog.h"

#include "XnVPointMessage.h"

#include "XnVBuiltInGesture.h"
#include "XnVBuiltInPointTracker.h"

struct GestureDescription
{
	XnUInt32 nID;
	XnVGesture* pGesture;
	XnBool bAuto;
	XnCallbackHandle hFocus;
	XnCallbackHandle hMidFocus;
};
XN_DECLARE_DEFAULT_HASH(XnUInt32, GestureDescription*, XnVGestureHash);


XnVSessionManager::XnVSessionManager(const XnChar* strName) :
	XnVMessageListener(strName),
	XnVSessionGenerator(strName),
	m_bGesturesEnabled(FALSE)
{
	m_eSessionState = SMGR_ILLEGAL;

	m_nNextGestureID = 1;
	m_bAutoTracker = FALSE;

	m_pMainGestures = XN_NEW(XnVGestureHash);
} // public XnVSessionManager::XnVSessionManager

XnVSessionManager::~XnVSessionManager()
{
	if (m_bAutoTracker)
	{
		XN_DELETE(m_pTracker);
	}

	ClearGestureHash(*m_pMainGestures);

	XN_DELETE(m_pMainGestures);
} // public XnVSessionManager::~XnVSessionManager

void XnVSessionManager::ClearGestureHash(XnVGestureHash& hash)
{
	for (XnVGestureHash::Iterator iter = hash.begin(); iter != hash.end(); ++iter)
	{
		XnVGesture* pGesture = iter.Value()->pGesture;
		if (iter.Value()->bAuto)
		{
			XN_DELETE(iter.Value()->pGesture);
		}
		XN_DELETE(iter.Value());
	}
	hash.Clear();
} // protected XnVSessionManager::ClearGestureHash

XnStatus XnVSessionManager::Initialize(xn::Context* pContext, const XnChar* strUseAsFocus, xn::HandsGenerator* pTracker, xn::GestureGenerator* pFocusGenerator)
{
	XnStatus rc = XN_STATUS_OK;
	xn::HandsGenerator tracker;
	m_bAutoTracker = FALSE;

	rc = FindTracker(pContext, pTracker, tracker);
	if (rc != XN_STATUS_OK)
	{
		return rc;
	}
	m_pTracker = XN_NEW(XnVBuiltInPointTracker, tracker);
	m_bAutoTracker = true;

	m_pTracker->AddListener(this);

	XnUInt32 focusID = 0, QRFocusID = 0;
	if (pFocusGenerator != NULL)
	{
		focusID = AddGesture(*pFocusGenerator, strUseAsFocus);
	}
	else
	{
		focusID = AddGesture(*pContext, strUseAsFocus);
	}

	EnableGestures();

	m_eSessionState = SMGR_NOT_IN_SESSION;

	return XN_STATUS_OK;
} // public XnVSessionManager::Initialize

XnStatus XnVSessionManager::FindTracker(xn::Context* pContext, xn::HandsGenerator* pTracker, xn::HandsGenerator& tracker)
{
	if (pTracker != NULL)
	{
		tracker = *pTracker;
		return XN_STATUS_OK;
	}

	XnStatus rc = pContext->FindExistingNode(XN_NODE_TYPE_HANDS, tracker);

	if (rc != XN_STATUS_OK)
		return XN_STATUS_NITE_NO_TRACKER;

	return XN_STATUS_OK;
} // protected XnVSessionManager::FindTracker

void XnVSessionManager::Update(XnVMessage* pMessage)
{
	// Hand Depth Messages
//	XnVContextControl::Update(pMessage);

	// Handle Point Messages
	XnVPointMessage* pPointMessage = XNV_GET_SPECIFIC_MESSAGE(pMessage, XnVPointMessage);

	if (pPointMessage != NULL)
	{
		XnVMultipleHands* pHands = (XnVMultipleHands*)pPointMessage->GetData();

		ProcessPoints(pHands);
		Generate(pPointMessage);
	}
} // public XnVSessionManager::Update

void XnVSessionManager::Update(const xn::Context* pContext)
{
	XnAutoCSLocker local(m_hListenerCS);
	m_pContext = pContext;

	// Update the Gesture
	UpdateGestures(pContext);

	// Initialize Tracker
	if (m_eSessionState == SMGR_START_SESSION)
	{
		m_eSessionState = SMGR_IN_SESSION;
		TrackPoint(m_ptStartTrackingPoint);

		xnLogVerbose(XNV_NITE_MASK_SESSION, "%s: Changing to In-Session state. Will now track points.",
			GetGeneratorName());
	}

	// Update Tracker
	m_pTracker->Update(pContext);

} // public XnVSessionManager::Update

void XnVSessionManager::ProcessPoints(XnVMultipleHands* pHands)
{
	if (IsInSession())
	{
		// Not in 'Not in Session' mode
		if (pHands->ActiveEntries() == 0)
		{
			xnLogVerbose(XNV_NITE_MASK_SESSION, "%s: Changing to No-Session Mode (Quick refocus not allowed). Will now look for gesture",
				GetGeneratorName());
			// No Quick Refocus allowed - move to 'Not in Session' mode
			m_eSessionState = SMGR_NOT_IN_SESSION;
			SessionStop();
			EnableGestures();
		} // if (pHands->ActiveEntries() == 0)
	} // if (IsInSession())
} // protected XnVSessionManager::ProcessPoints

void XnVSessionManager::SessionStop()
{
	EnableGestures();
	m_eSessionState	 = SMGR_NOT_IN_SESSION;

	XnVSessionGenerator::SessionStop();
} // protected XnVSessionManager::SessionStop

void XnVSessionManager::SetTracker(xn::HandsGenerator handsGenerator)
{
	XnVPointTracker* pNewTracker = XN_NEW(XnVBuiltInPointTracker, handsGenerator);
	SetTracker(pNewTracker);
	m_bAutoTracker = true;
} // public XnVSessionManager::SetTracker
void XnVSessionManager::SetTracker(XnVPointTracker* pTracker)
{
	if (m_pTracker != NULL)
	{
		if (IsInSession())
		{
			m_pTracker->Clear();
		}
		if (m_bAutoTracker)
		{
			XN_DELETE(m_pTracker);
		}
	}

	m_pTracker = pTracker;
	m_bAutoTracker = false;

	if (IsInSession())
	{
		EnableGestures();
		m_eSessionState	 = SMGR_NOT_IN_SESSION;
	}
} // public XnVSessionManager::SetTracker

void XN_CALLBACK_TYPE XnVSessionManager::Gesture_Recognized(const XnChar* strGesture, const XnPoint3D& ptIDPosition, const XnPoint3D& ptEndPosition, void* cxt)
{
	// A Gesture was recognized!
	XnVSessionManager* pSM = (XnVSessionManager*)cxt;

	if (pSM != NULL)
	{
		pSM->GestureRecognized(strGesture, ptIDPosition, ptEndPosition);
	}
} // protected XnVSessionManager::Gesture_Recognized

void XN_CALLBACK_TYPE XnVSessionManager::Gesture_StartRecognized(const XnChar* strGesture, const XnPoint3D& pos, XnFloat fProgress, void* cxt)
{
	XnVSessionManager* pSM = (XnVSessionManager*)cxt;

	if (pSM  != NULL)
	{
		pSM->GestureStartRecognized(strGesture, pos, fProgress);
	}
} // protected XnVSessionManager::Gesture_StartRecognized

void XnVSessionManager::GestureStartRecognized(const XnChar* strGesture, const XnPoint3D& pos, XnFloat fProgress)
{
	SessionMidGesture(strGesture, pos, fProgress);
} // protected XnVSessionManager::GestureStartRecognized

void XnVSessionManager::GestureRecognized(const XnChar* strGesture, const XnPoint3D& ptIDPosition, const XnPoint3D& ptEndPosition)
{
	xnLogVerbose(XNV_NITE_MASK_SESSION, "%s: Gesture recognized", GetGeneratorName());
	// Don't look for a gesture anymore
	DisableGestures();

	m_eSessionState = SMGR_START_SESSION;

	SessionStart(ptIDPosition);

	m_ptStartTrackingPoint = ptEndPosition;
} // protected XnVSessionManager::GestureRecognized

XnStatus XnVSessionManager::LosePoint(XnUInt32 nID)
{
	XnAutoCSLocker local(m_hListenerCS);
	return m_pTracker->StopTracking(nID);
} // public XnVSessionManager::LosePoint
XnStatus XnVSessionManager::LosePoints()
{
	XnAutoCSLocker local(m_hListenerCS);
	return m_pTracker->StopTracking();
} // public XnVSessionManager::LosePoints

XnStatus XnVSessionManager::EndSession()
{
	XnAutoCSLocker local(m_hListenerCS);
	LosePoints();

	if (!IsNotInSession()) // Neither QR nor NoSession
	{
		EnableGestures();
	}

	if (IsInSession()) // Either QR or InSession
	{
		XnVSessionGenerator::SessionStop();
	}

	m_eSessionState = SMGR_NOT_IN_SESSION;

	return XN_STATUS_OK;
} // public XnVSessionManager::EndSession

XnStatus XnVSessionManager::ForceSession(const XnPoint3D& ptFocus)
{
	XnAutoCSLocker local(m_hListenerCS);
	if (IsInSession())
	{
		// Already in session. Update focus point?
		return XN_STATUS_OK;
	}

	GestureRecognized("Manual", ptFocus, ptFocus);
	return XN_STATUS_OK;
} // public XnVSessionManager::ForceSession

XnStatus XnVSessionManager::TrackPoint(const XnPoint3D& pt)
{
	XnAutoCSLocker local(m_hListenerCS);
	if (m_pTracker == NULL)
	{
		return XN_STATUS_ERROR;
	}

	return m_pTracker->StartTracking(pt);
} // public XnVSessionManager::TrackPoint

void XnVSessionManager::ClearQueue()
{
	XnVMessageListener::ClearQueue();
	ClearAllQueues();
} // public XnVSessionManager::ClearQueue

XnUInt64 XnVSessionManager::GetTime()
{
	xn::DepthGenerator depthGenerator;
	XnStatus rc = m_pContext->FindExistingNode(XN_NODE_TYPE_DEPTH, depthGenerator);
	if (rc != XN_STATUS_OK)
	{
		return 0;
	}

	return depthGenerator.GetTimestamp();
} // protected XnVSessionManager::GetTime

void XnVSessionManager::EnableGestures(XnBoundingBox3D* pROI)
{
	for (XnVGestureHash::Iterator iter = m_pMainGestures->begin(); iter != m_pMainGestures->end(); ++iter)
	{
		iter.Value()->pGesture->StartGesture(pROI);
	}
	m_bGesturesEnabled = TRUE;
} // protected XnVSessionManager::EnableGestures

void XnVSessionManager::DisableGestures()
{
	m_bGesturesEnabled = FALSE;
	for (XnVGestureHash::Iterator iter = m_pMainGestures->begin(); iter != m_pMainGestures->end(); ++iter)
	{
		iter.Value()->pGesture->StopGesture();
	}
} // protected XnVSessionManager::DisableGestures

void XnVSessionManager::UpdateGestures(const xn::Context* pContext)
{
	for (XnVGestureHash::Iterator iter = m_pMainGestures->begin(); iter != m_pMainGestures->end(); ++iter)
	{
		iter.Value()->pGesture->Update(pContext);
	}
} // protected XnVSessionManager::UpdateGestures

static void CreateNamesHash(const XnChar* strName, XnStringsHash& hash)
{
	if (strName == NULL)
		return;

	XnChar* strCurrentGesture = const_cast<XnChar*>(xnOSStrDup(strName));
	XnChar* strForFree = strCurrentGesture;
	XnBool bLast = false;
	while (*strCurrentGesture != '\0')
	{
		// Replace first comma with NULL
		int nComma = 0;
		for (nComma = 0; strCurrentGesture[nComma] != '\0' && strCurrentGesture[nComma] != ','; ++nComma)
			;
		if (strCurrentGesture[nComma] == '\0')
			bLast = true;
		else
			strCurrentGesture[nComma] = '\0';

		// add current gesture
		hash.Set(strCurrentGesture, NULL);
		// current gesture is next space (if exists)
		if (bLast)
			break;

		strCurrentGesture += nComma + 1;
	}
	xnOSFree(strForFree);
} // static CreateNamesHash

XnUInt32 XnVSessionManager::AddGesture(xn::GestureGenerator& generator, const XnChar* strName)
{
	XnStringsHash strings;
	CreateNamesHash(strName, strings);
	return AddGesture(generator, strName, strings);
} // public XnVSessionManager::AddGesture
XnUInt32 XnVSessionManager::AddGesture(xn::Context& context, const XnChar* strName)
{
	XnStringsHash strings;
	CreateNamesHash(strName, strings);
	return AddGesture(context, strName, strings);
} // public XnVSessionManager::AddGesture

XnUInt32 XnVSessionManager::AddGesture(XnVGesture *pGesture)
{
	return AddGesture(pGesture, FALSE, 0);
} // public XnVSessionManager::AddGesture
XnUInt32 XnVSessionManager::AddGesture(XnVGesture *pGesture, XnBool bAuto, XnUInt32 nHintId)
{
	if (pGesture == NULL)
	{
		return 0;
	}

	XnUInt32 nID = nHintId;
	if (nHintId == 0)
	{
		nID = m_nNextGestureID++;
	}

	GestureDescription* pGD = XN_NEW(GestureDescription);
	pGD->bAuto = bAuto;
	pGD->pGesture = pGesture;
	pGD->nID = nID;

	m_pMainGestures->Set(nID, pGD);
	// Register to gesture
	pGD->hFocus = pGD->pGesture->RegisterRecognize(this, Gesture_Recognized);
	// Register to mid gesture
	pGD->hMidFocus = pGD->pGesture->RegisterStartRecognize(this, Gesture_StartRecognized);

	if (m_bGesturesEnabled)
	{
		pGD->pGesture->StartGesture();
	}

	return nID;
} // protected XnVSessionManager::AddGesture


XnUInt32 XnVSessionManager::AddGesture(xn::Context& context, const XnChar* strName, const XnStringsHash& hash)
{
	xn::NodeInfoList nodes;
	// Search context for a hand generator
	XnStatus rc = context.EnumerateExistingNodes(nodes, XN_NODE_TYPE_GESTURE);
	if (rc != XN_STATUS_OK)
	{
		printf("Enumeration for gestures failed!\n");
		return rc;
	}
	if (nodes.IsEmpty())
	{
		return XN_STATUS_NITE_NO_GESTURE;
	}

	XnBool bFound = FALSE;
	xn::GestureGenerator generator;
	for (xn::NodeInfoList::Iterator iter = nodes.Begin(); iter != nodes.End(); ++iter)
	{
		(*iter).GetInstance(generator);
		XnUInt id = AddGesture(generator, strName, hash);
		if (id != 0)
		{
			// Success!
			return id;
		}
	}

	// No matching generator found in context
	return 0;

} // protected XnVSessionManager::AddGesture
XnUInt32 XnVSessionManager::AddGesture(xn::GestureGenerator& generator, const XnChar* strName, const XnStringsHash& hash)
{
	if (hash.begin() == hash.end())
	{
		return 0;
	}

	XnBool bLegal = TRUE;
	for (XnStringsHash::ConstIterator required = hash.begin();
		required != hash.end(); ++required)
	{
		xnLogVerbose(XNV_NITE_MASK_SESSION, "Looking for gesture '%s' in generator '%s'",
			required.Key(), generator.GetInfo().GetInstanceName());
		if (!generator.IsGestureAvailable(required.Key()))
		{
			xnLogVerbose(XNV_NITE_MASK_SESSION, "\tNope. Generator doesn't match.");
			bLegal = FALSE;
			break;
		}
		xnLogVerbose(XNV_NITE_MASK_SESSION, "\tOK.");
	}

	if (!bLegal)
	{
		return 0;
	}

	xnLogVerbose(XNV_NITE_MASK_SESSION, "Found generator that supports all gestures");
	XnVBuiltInGesture* pGesture = XN_NEW(XnVBuiltInGesture, generator, strName);

	return AddGesture(pGesture, TRUE, 0);
} // protected XnVSessionManager::AddGesture

void XnVSessionManager::RemoveGesture(XnUInt32 nId)
{
	RemoveGesture(nId, *m_pMainGestures);
} // public XnVSessionManager::RemoveGesture
void XnVSessionManager::RemoveGesture(XnUInt32 nId, XnVGestureHash &hash)
{
	GestureDescription* pGD = NULL;
	XnStatus rc = hash.Remove(nId, pGD);
	if (rc != XN_STATUS_OK || pGD == NULL)
	{
		return;
	}

	pGD->pGesture->StopGesture();
	pGD->pGesture->UnregisterRecognize(pGD->hFocus);
	pGD->pGesture->UnregisterStartRecognize(pGD->hMidFocus);

	if (pGD->bAuto)
		XN_DELETE(pGD->pGesture);

	XN_DELETE(pGD);
} // protected XnVSessionManager::RemoveGesture
