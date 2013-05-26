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
#include <XnStringsHash.h>
#include "XnVFlowRouter.h"
#include "XnVMultipleHands.h"
#include "XnVPointMessage.h"
#include "XnVActivationMessage.h"
#include "XnVNiteLog.h"

static const XnChar* FR_PROPERTY_HANDS = "Hands";

XnVFlowRouter::XnVFlowRouter(const XnChar* strName) :
	XnVMessageListener(strName),
	m_pActive(NULL)
{
	m_pProperties = XN_NEW(XnStringsHash);
	XnVMultipleHands* pHands = XN_NEW(XnVMultipleHands);
	m_pProperties->Set(FR_PROPERTY_HANDS, pHands);

} // XnVFlowRouter::XnVFlowRouter

XnVFlowRouter::~XnVFlowRouter()
{
//	ClearQueue();
	m_pActive = NULL;
//	SetActive(NULL);

	XnVMultipleHands* pHands;
	XnValue value;
	m_pProperties->Get(FR_PROPERTY_HANDS, value);
	pHands = (XnVMultipleHands*)value;

	XN_DELETE(pHands);
	XN_DELETE(m_pProperties);
} // XnVFlowRouter::~XnVFlowRouter

XnVMessageListener* XnVFlowRouter::GetActive() const
{
	return m_pActive;
} // XnVFlowRouter::GetActive

void XnVFlowRouter::Update(XnVMessage *pMessage)
{
	XnAutoCSLocker local(m_hListenerCS);

	if (m_pActive != NULL)
	{
		m_pActive->BaseUpdate(pMessage);
	}

	// Remember the current Point situation, if exists
	XnVPointMessage* pPtMessage = XNV_GET_SPECIFIC_MESSAGE(pMessage, XnVPointMessage);

	if (pPtMessage != NULL)
	{
		const XnVMultipleHands* pNewHands = (const XnVMultipleHands*)(pPtMessage->GetData());
		XnVMultipleHands* pLocalHands;
		XnValue value;
		m_pProperties->Get(FR_PROPERTY_HANDS, value);
		pLocalHands = (XnVMultipleHands*)value;

		pNewHands->Clone(*pLocalHands);
	}

} // XnVFlowRouter::Update

XnStatus XnVFlowRouter::SetActive(XnVMessageListener *pActive)
{
	XnAutoCSLocker local(m_hListenerCS);

	if (m_pActive == pActive)
	{
		return XN_STATUS_OK;
	}

	CloseOldSession();
	m_pActive = pActive;
	OpenNewSession();

	return XN_STATUS_OK;
} // XnVFlowRouter::SetActive

void XnVFlowRouter::CloseOldSession()
{
	if (m_pActive == NULL)
	{
		return;
	}

	xnLogVerbose(XNV_NITE_MASK_CONNECT, "Router %s [0x%08x]: Disconnecting %s [0x%08x] as active",
		GetListenerName(), this, m_pActive->GetListenerName(), m_pActive);

	XnVMultipleHands oldHands;

	XnVMultipleHands* pLocalHands;
	XnValue value;
	m_pProperties->Get(FR_PROPERTY_HANDS, value);
	pLocalHands = (XnVMultipleHands*)value;

	// Create a Multiple Hands object with the same hands, indicating all of them are old
	pLocalHands->Clone(oldHands);
	oldHands.ClearLists();

	while (oldHands.begin() != oldHands.end())
	{
		oldHands.MarkOld((*oldHands.begin())->nID);
		oldHands.Remove((*oldHands.begin())->nID);
	}

	m_pActive->ClearQueue();

	// Send the 'all-old' Point Message to to listener that's being deactivated
	XnVPointMessage pointMessage(&oldHands);
	m_pActive->BaseUpdate(&pointMessage);

	// Send a Deactivation Message
	XnVActivationMessage deactivate(false);
	m_pActive->BaseUpdate(&deactivate);
} // XnVFlowRouter::CloseOldSession

void XnVFlowRouter::OpenNewSession()
{
	if (m_pActive == NULL)
	{
		return;
	}

	xnLogVerbose(XNV_NITE_MASK_CONNECT, "Router %s [0x%08x]: Connecting %s [0x%08x] as active",
		GetListenerName(), this, m_pActive->GetListenerName(), m_pActive);

	XnVMultipleHands newHands;

	XnVMultipleHands* pLocalHands;
	XnValue value;
	m_pProperties->Get(FR_PROPERTY_HANDS, value);
	pLocalHands = (XnVMultipleHands*)value;

	// Create a Multiple Hands object with the same hands, indicating all of them are new
	pLocalHands->Clone(newHands);
	newHands.ClearLists();
	for (XnVMultipleHands::Iterator iter = newHands.begin(); iter != newHands.end(); ++iter)
	{
		newHands.MarkActive((*iter)->nID);
		newHands.MarkNew((*iter)->nID);
	}

	// Send an Activation Message
	XnVActivationMessage activate(true);
	m_pActive->BaseUpdate(&activate);

	// Send the 'all-new' Point Message to to listener that's being activated
	XnVPointMessage pointMessage(&newHands);
	m_pActive->BaseUpdate(&pointMessage);
} // XnVFlowRouter::OpenNewSession

void XnVFlowRouter::ClearQueue()
{
	XnVMessageListener::ClearQueue();
	if (m_pActive != NULL)
	{
		m_pActive->ClearQueue();
	}
} // XnVFlowRouter::ClearQueue
