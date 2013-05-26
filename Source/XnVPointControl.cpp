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
#include "XnVPointControl.h"
#include "XnVPointMessage.h"
#include "XnVNiteEvents.h"

XN_DECLARE_EVENT_1ARG(XnVHandPointContextSpecificEvent, XnVHandPointContextEvent, const XnVHandPointContext*, pContext);
XN_DECLARE_EVENT_1ARG(XnVMultipleHandsSpecificEvent, XnVMultipleHandsEvent, const XnVMultipleHands&, pContext);

XN_DECLARE_EVENT_2ARG(XnVHandPointContextPointSpecificEvent, XnVHandPointContextPointEvent, const XnVHandPointContext*, pContext, const XnPoint3D&, ptPos); 
XN_DECLARE_EVENT_2ARG(XnVUintHandPointContextSpecificEvent, XnVUintHandPointContextEvent, XnUInt32, nValue, const XnVHandPointContext*, pContext); 


XnVPointControl::XnVPointControl(const XnChar* strName) :
	XnVMessageListener(strName), m_nPrimaryID(0), m_nOverridePrimary(0)
	{
		m_pPointCreateCBs = XN_NEW(XnVHandPointContextSpecificEvent);
		m_pPointUpdateCBs = XN_NEW(XnVHandPointContextSpecificEvent);
		m_pPointDestroyCBs = XN_NEW(XnVUintSpecificEvent);

		m_pPrimaryPointCreateCBs = XN_NEW(XnVHandPointContextPointSpecificEvent);
		m_pPrimaryPointUpdateCBs = XN_NEW(XnVHandPointContextSpecificEvent);
		m_pPrimaryPointReplaceCBs = XN_NEW(XnVUintHandPointContextSpecificEvent);
		m_pPrimaryPointDestroyCBs = XN_NEW(XnVUintSpecificEvent);

		m_pNoPointsCBs = XN_NEW(XnVEvent);

		m_pHandsUpdateCBs = XN_NEW(XnVMultipleHandsSpecificEvent);

	}

XnVPointControl::~XnVPointControl()
{
	XN_DELETE(m_pPointCreateCBs);
	XN_DELETE(m_pPointUpdateCBs);
	XN_DELETE(m_pPointDestroyCBs);

	XN_DELETE(m_pPrimaryPointCreateCBs);
	XN_DELETE(m_pPrimaryPointUpdateCBs);
	XN_DELETE(m_pPrimaryPointReplaceCBs);
	XN_DELETE(m_pPrimaryPointDestroyCBs);

	XN_DELETE(m_pNoPointsCBs);

	XN_DELETE(m_pHandsUpdateCBs);
}

void XnVPointControl::Update(XnVMessage* pMessage)
{
	// Extract the Multiple Hands from the Point Message
	XnVPointMessage* pPointMessage = XNV_GET_SPECIFIC_MESSAGE(pMessage, XnVPointMessage);

	if (pPointMessage != NULL)
	{
		const XnVMultipleHands* pHands = (const XnVMultipleHands*)pPointMessage->GetData();
		Update(*pHands);
		m_pHandsUpdateCBs->Raise(*pHands);
	}
} // XnVPointControl::Update

void XnVPointControl::Update(const XnVMultipleHands& hands)
{
	const XnVHandPointContext* pPrimaryContext = hands.GetPrimaryContext();

	XnBool bPrimaryChanged = false;
	XnBool bPrimaryFirst = false;
	XnUInt32 nOldId = m_nPrimaryID;
	if (pPrimaryContext != NULL)
	{
		if (m_nPrimaryID == 0)
		{
			// First!
			bPrimaryFirst = true;
		}
		else if (m_nPrimaryID != pPrimaryContext->nID)
		{
			// not the same as before
			bPrimaryChanged = true;
		}

		m_nPrimaryID = pPrimaryContext->nID;
	}
	else
	{
		m_nPrimaryID = 0;
	}

	// Call specific methods for each hand, whether new or not.
	for (XnVMultipleHands::ConstIterator iter = hands.begin(); iter != hands.end(); ++iter)
	{
		XnBool bPrimary = (*iter)->nID == m_nPrimaryID;
		if (iter.IsNew())
		{
			m_pPointCreateCBs->Raise(*iter);
			OnPointCreate(*iter);
			if (bPrimary && bPrimaryFirst)
			{
				m_pPrimaryPointCreateCBs->Raise(*iter, hands.GetFocusPoint());
				OnPrimaryPointCreate(*iter, hands.GetFocusPoint());
			}
		}
		else if (iter.IsActive())
		{
			if (bPrimary && bPrimaryChanged)
			{
				OnPrimaryPointReplace(nOldId, pPrimaryContext);
				m_pPrimaryPointReplaceCBs->Raise(nOldId, pPrimaryContext);
			}
			OnPointUpdate(*iter);
			m_pPointUpdateCBs->Raise(*iter);
		}
		// Make sure the single id is the same, or change to the new one.
	} // for

	if (hands.ActiveEntries() > 0)
	{
		OnPrimaryPointUpdate(pPrimaryContext);
		m_pPrimaryPointUpdateCBs->Raise(pPrimaryContext);
	}
	else if (hands.OldEntries() > 0)
	{
		if (m_nPrimaryID == 0)
		{
			OnPrimaryPointDestroy(nOldId);
			m_pPrimaryPointDestroyCBs->Raise(nOldId);
		}

		OnNoPoints();
		m_pNoPointsCBs->Raise();
	}

	// Call specific methods for hands that are no longer
	for (XnVMultipleHands::ConstIteratorOld iter = hands.beginOld(); iter != hands.endOld(); ++iter)
	{
		XnUInt32 nID = XnUInt32(*iter);

		if (nID == m_nPrimaryID)
		{
			OnPrimaryPointDestroy(nID);
			m_pPrimaryPointDestroyCBs->Raise(nID);
		}

		OnPointDestroy(nID);
		m_pPointDestroyCBs->Raise(nID);
	}
} // XnVPointControl::Update

XnUInt32 XnVPointControl::GetPrimaryID() const
{
	return m_nPrimaryID;
} // XnVPointControl::GetPrimaryID

XnCallbackHandle XnVPointControl::RegisterPointCreate(void* cxt, PointCreateCB CB)
{
	XnCallbackHandle handle;
	m_pPointCreateCBs->Register(CB, cxt, &handle);
	return handle;
} // XnVPointControl::RegisterPointCreate

XnCallbackHandle XnVPointControl::RegisterPointUpdate(void* cxt, PointUpdateCB CB)
{
	XnCallbackHandle handle;
	m_pPointUpdateCBs->Register(CB, cxt, &handle);
	return handle;
} // XnVPointControl::RegisterPointUpdate

XnCallbackHandle XnVPointControl::RegisterPointDestroy(void* cxt, PointDestroyCB CB)
{
	XnCallbackHandle handle;
	m_pPointDestroyCBs->Register(CB, cxt, &handle);
	return handle;
} // XnVPointControl::RegisterPointDestroy

XnCallbackHandle XnVPointControl::RegisterPrimaryPointCreate(void* cxt, PrimaryPointCreateCB CB)
{
	XnCallbackHandle handle;
	m_pPrimaryPointCreateCBs->Register(CB, cxt, &handle);
	return handle;
} // XnVPointControl::RegisterPrimaryPointCreate

XnCallbackHandle XnVPointControl::RegisterPrimaryPointUpdate(void* cxt, PrimaryPointUpdateCB CB)
{
	XnCallbackHandle handle;
	m_pPrimaryPointUpdateCBs->Register(CB, cxt, &handle);
	return handle;
} // XnVPointControl::RegisterPrimaryPointUpdate

XnCallbackHandle XnVPointControl::RegisterPrimaryPointReplace(void* cxt, PrimaryPointReplaceCB CB)
{
	XnCallbackHandle handle;
	m_pPrimaryPointReplaceCBs->Register(CB, cxt, &handle);
	return handle;
} // XnVPointControl::RegisterPrimaryPointReplace

XnCallbackHandle XnVPointControl::RegisterPrimaryPointDestroy(void* cxt, PrimaryPointDestroyCB CB)
{
	XnCallbackHandle handle;
	m_pPrimaryPointDestroyCBs->Register(CB, cxt, &handle);
	return handle;
} // XnVPointControl::RegisterPrimaryPointDestroy

XnCallbackHandle XnVPointControl::RegisterNoPoints(void* cxt, NoPointsCB CB)
{
	XnCallbackHandle handle;
	m_pNoPointsCBs->Register(CB, cxt, &handle);
	return handle;
} // XnVPointControl::RegisterNoPoints

XnCallbackHandle XnVPointControl::RegisterHandsUpdate(void* cxt, HandsUpdateCB CB)
{
	XnCallbackHandle handle;
	m_pHandsUpdateCBs->Register(CB, cxt, &handle);
	return handle;
} // XnVPointControl::RegisterHandsUpdate

void XnVPointControl::UnregisterPointCreate(XnCallbackHandle hCB)
{
	m_pPointCreateCBs->Unregister(hCB);
} // XnVPointControl::UnregisterPointCreate

void XnVPointControl::UnregisterPointUpdate(XnCallbackHandle hCB)
{
	m_pPointUpdateCBs->Unregister(hCB);
} // XnVPointControl::UnregisterPointUpdate

void XnVPointControl::UnregisterPointDestroy(XnCallbackHandle hCB)
{
	m_pPointDestroyCBs->Unregister(hCB);
} // XnVPointControl::UnregisterPointDestroy

void XnVPointControl::UnregisterPrimaryPointCreate(XnCallbackHandle hCB)
{
	m_pPrimaryPointCreateCBs->Unregister(hCB);
} // XnVPointControl::UnregisterPrimaryPointCreate

void XnVPointControl::UnregisterPrimaryPointUpdate(XnCallbackHandle hCB)
{
	m_pPrimaryPointUpdateCBs->Unregister(hCB);
} // XnVPointControl::UnregisterPrimaryPointUpdate

void XnVPointControl::UnregisterPrimaryPointReplace(XnCallbackHandle hCB)
{
	m_pPrimaryPointReplaceCBs->Unregister(hCB);
} // XnVPointControl::UnregisterPrimaryPointReplace

void XnVPointControl::UnregisterPrimaryPointDestroy(XnCallbackHandle hCB)
{
	m_pPrimaryPointDestroyCBs->Unregister(hCB);
} // XnVPointControl::UnregisterPrimaryPointDestroy

void XnVPointControl::UnregisterNoPoints(XnCallbackHandle hCB)
{
	m_pNoPointsCBs->Unregister(hCB);
} // XnVPointControl::UnregisterNoPoints

void XnVPointControl::UnregisterHandsUpdate(XnCallbackHandle hCB)
{
	m_pHandsUpdateCBs->Unregister(hCB);
} // XnVPointControl::UnregisterHandsUpdate
