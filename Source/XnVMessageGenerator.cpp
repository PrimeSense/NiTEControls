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

#include "XnVMessageGenerator.h"
#include "XnVPointMessage.h"
#include "XnVNiteLog.h"

XN_DECLARE_DEFAULT_HASH_DECL(XNV_NITE_CONTROLS_API, XnUInt32, XnVMessageListener*, XnVIntMessageListenerHash);


static const XnChar* FR_PROPERTY_HANDS = "Hands";

XnVMessageGenerator::XnVMessageGenerator(const XnChar* strName)
{
	xnLogVerbose(XNV_NITE_MASK_CREATE, "Generator %s [0x%08x]: Create", strName, this);

	m_hNextAvailable = 1;

	m_strGeneratorName = (XnChar*)xnOSCalloc(strlen(strName)+1, sizeof(XnChar));
	if (m_strGeneratorName != NULL)
	{
		xnOSStrCopy(m_strGeneratorName, strName, strlen(strName)+1);
	}

	m_phListeners = XN_NEW(XnVIntMessageListenerHash);
	m_pProperties = XN_NEW(XnStringsHash);
	XnVMultipleHands* pHands = XN_NEW(XnVMultipleHands);
	if (pHands != NULL)
	{
		m_pProperties->Set(FR_PROPERTY_HANDS, pHands);
	}

} // XnVMessageGenerator::XnVMessageGenerator

XnVMessageGenerator::~XnVMessageGenerator()
{
	m_phListeners->Clear();

	xnLogVerbose(XNV_NITE_MASK_CREATE, "Generator %s [0x%08x]: Destroy", m_strGeneratorName, this);
	xnOSFree(m_strGeneratorName);

	XnVMultipleHands* pHands;
	XnValue value;
	m_pProperties->Get(FR_PROPERTY_HANDS, value);
	pHands = (XnVMultipleHands*)value;

	XN_DELETE(pHands);

} // XnVMessageGenerator::~XnVMessageGenerator

XnVHandle XnVMessageGenerator::AddListener(XnVMessageListener *pListener)
{
	// Add a listener to the Listener hash
	XnVHandle handle = m_hNextAvailable;

	m_phListeners->Set(handle, pListener);
	m_hNextAvailable++;

	xnLogVerbose(XNV_NITE_MASK_CONNECT, "Generator %s [0x%08x]: Adding listener %s [0x%08x] [%d]", 
		m_strGeneratorName, this, pListener->GetListenerName(), pListener, handle);

	OpenNewSession(pListener);

	return handle;
} // XnVMessageGenerator::AddListener

XnVMessageListener* XnVMessageGenerator::RemoveListener(XnVHandle hListener)
{
	XnVMessageListener* pListener;

	// Remove the listener from the listener hash
	if (m_phListeners->Remove(hListener, pListener) != XN_STATUS_OK)
	{
		// No such listener
		return NULL;
	}

	xnLogVerbose(XNV_NITE_MASK_CONNECT, "Generator %s [0x%08x]: Removing Listener %s [0x%08x] [%d]",
		m_strGeneratorName, this, pListener->GetListenerName(), pListener, hListener);

	CloseOldSession(pListener);

	return pListener;
} // XnVMessageGenerator::RemoveListener

XnVMessageListener* XnVMessageGenerator::RemoveListener(XnVMessageListener *pListener)
{
	XnVHandle hListener = 0;
	for (XnVIntMessageListenerHash::Iterator iter = m_phListeners->begin(); iter != m_phListeners->end(); ++iter)
	{
		XnVMessageListener* pCurListener = iter.Value();

		if (pCurListener == pListener)
		{
			hListener = iter.Key();
		}
	}

	if (hListener == 0)
	{
		return NULL;
	}

	return RemoveListener(hListener);
} // XnVMessageGenerator::RemoveListener

void XnVMessageGenerator::Generate(XnVMessage *pMessage)
{
	// Remember last point-state, for open/close sessions
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


	// Send Message to all listeners
	for (XnVIntMessageListenerHash::Iterator iter = m_phListeners->begin(); iter != m_phListeners->end(); ++iter)
	{
		XnVMessageListener* pListener = iter.Value();

		xnLogVerbose(XNV_NITE_MASK_FLOW, "Generator %s [0x%08x]: Send '%s' message",
			m_strGeneratorName, this, pMessage->GetType());

		pListener->BaseUpdate(pMessage);
	}
} // XnVMessageGenerator::Generate

void XnVMessageGenerator::ClearAllQueues()
{
	for (XnVIntMessageListenerHash::Iterator iter = m_phListeners->begin(); iter != m_phListeners->end(); ++iter)
	{
		XnVMessageListener* pListener = iter.Value();
		pListener->ClearQueue();
	}
} // XnVMessageGenerator::ClearAllQueues

const XnChar* XnVMessageGenerator::GetGeneratorName() const
{
	return m_strGeneratorName;
} // XnVMessageGenerator::GetGeneratorName

void XnVMessageGenerator::OpenNewSession(XnVMessageListener* pListener)
{
	XnVMultipleHands* pLocalHands;
	XnValue value;
	m_pProperties->Get(FR_PROPERTY_HANDS, value);
	pLocalHands = (XnVMultipleHands*)value;
	
	pLocalHands->ClearLists();
	for (XnVMultipleHands::Iterator iter = pLocalHands->begin(); iter != pLocalHands->end(); ++iter)
	{
		pLocalHands->MarkActive((*iter)->nID);
		pLocalHands->MarkNew((*iter)->nID);
	}
	XnVPointMessage pointMessage(pLocalHands);
	pListener->BaseUpdate(&pointMessage);
} // XnVMessageGenerator::OpenNewSession

void XnVMessageGenerator::CloseOldSession(XnVMessageListener* pListener)
{
	XnVMultipleHands* pLocalHands;
	XnValue value;
	m_pProperties->Get(FR_PROPERTY_HANDS, value);
	pLocalHands = (XnVMultipleHands*)value;

	XnVMultipleHands oldHands;
	for (XnVMultipleHands::ConstIterator iter = pLocalHands->begin(); iter != pLocalHands->end(); ++iter)
	{
		oldHands.MarkOld((*iter)->nID);
	}

	pListener->ClearQueue();
	XnVPointMessage pointMessage(&oldHands);
	pListener->BaseUpdate(&pointMessage);
} // XnVMessageGenerator::CloseOldSession
