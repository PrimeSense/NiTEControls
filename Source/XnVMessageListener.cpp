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
#include <XnOpenNI.h>
#include "XnVNiteEvents.h"
#include "XnVMessageListener.h"
#include "XnVNiteLog.h"
#include "XnVActivationMessage.h"

XN_DECLARE_THREAD_SAFE_QUEUE(XnVMessage*, XnVMessageQueue);
XN_DECLARE_EVENT_1ARG(XnVMessageSpecificEvent, XnVMessageEvent, XnVMessage*, pMessage);

const XnUInt32 XnVMessageListener::ms_nSessionDefaultLength = 10;

XnVMessageListener::XnVMessageListener(const XnChar* strName) :
	m_bInternalThreadAlive(false),
	m_bInternalThreadKill(false),
	m_bThreadProtectionQueueMode(false)
{
	xnLogVerbose(XNV_NITE_MASK_CREATE, "Listener %s [0x%08x]: Create", strName, this);

	xnOSCreateCriticalSection(&m_hListenerCS);

	SetCurrentThread();
	m_bThreadProtectionQueueMode = false;

	m_strListenerName = (XnChar*)xnOSCalloc(strlen(strName)+1, sizeof(XnChar));
	if (m_strListenerName != NULL)
	{
		xnOSStrCopy(m_strListenerName, strName, strlen(strName)+1);
	}

	m_pMessageQueue = XN_NEW(XnVMessageQueue);
	m_pMessageQueue->Init();

	m_pUpdateCBs = XN_NEW(XnVMessageSpecificEvent);
	m_pActivateCBs = XN_NEW(XnVEvent);
	m_pDeactivateCBs = XN_NEW(XnVEvent);
} // XnVMessageListener::XnVMessageListener

XnVMessageListener::~XnVMessageListener()
{
	if (m_bInternalThreadAlive)
	{
		XnAutoCSLocker local(m_hListenerCS);

		m_bInternalThreadKill = true;
		XnStatus rc = xnOSWaitForThreadExit(m_hActivityThread, 1000);
		if (rc != XN_STATUS_OK)
		{
			xnOSTerminateThread(&m_hActivityThread);
		}
		else
		{
			xnOSCloseThread(&m_hActivityThread);
		}
	}

	xnLogVerbose(XNV_NITE_MASK_CREATE, "Listener %s [0x%08x]: Destroy", m_strListenerName, this);

	XnVMessageListener::ClearQueue();

	if (m_strListenerName != NULL)
	{
		xnOSFree(m_strListenerName);
	}
	XN_DELETE(m_pMessageQueue);
	XN_DELETE(m_pUpdateCBs);
	XN_DELETE(m_pActivateCBs);
	XN_DELETE(m_pDeactivateCBs);
	xnOSCloseCriticalSection(&m_hListenerCS);
} // XnVMessageListener::~XnVMessageListener

void XnVMessageListener::HandleCCMessages(XnVMessage* pMessage)
{
	XnBool bValue;
	if (XnVCCMessage::GetValue(pMessage, XnVActivationMessage::ms_strType, bValue) == XN_STATUS_OK)
	{
		Activate(bValue);
		if (bValue)
		{
			m_pActivateCBs->Raise();
		}
		else
		{
			m_pDeactivateCBs->Raise();
		}
	}
} // XnVMessageListener::HandleCCMessages

void XnVMessageListener::BaseUpdate(XnVMessage* pMessage)
{
	if (!m_bThreadProtectionQueueMode || IsInActivityThread())
	{
		xnLogVerbose(XNV_NITE_MASK_FLOW, "Listener %s [0x%08x]: Update (%s)",
			GetListenerName(), this, pMessage->GetType());

		HandleCCMessages(pMessage);

		xnOSEnterCriticalSection(&m_hListenerCS);
		Update(pMessage);
		xnOSLeaveCriticalSection(&m_hListenerCS);
		m_pUpdateCBs->Raise(pMessage);
	}
	else
	{
		xnLogVerbose(XNV_NITE_MASK_MT_QUEUE, "Listener %s [%08x]: Adding to queue (%s)",
			GetListenerName(), this, pMessage->GetType());

		XnVMessage* pNewMessage = pMessage->Clone();
		m_pMessageQueue->Push(pNewMessage);
	}
} // XnVMessageListener::BaseUpdate

void XnVMessageListener::Run(XnUInt32 nSessionMaxLength)
{
	if (!IsInActivityThread())
	{
		return;
	}

	XnUInt32 nSessionLength = 0;

	while (!m_pMessageQueue->IsEmpty() && nSessionLength < nSessionMaxLength)
	{
		XnVMessage* pMessage = NULL;
		XnStatus rc = m_pMessageQueue->Pop(pMessage);
		if (rc == XN_STATUS_OK && pMessage != NULL)
		{
			xnLogVerbose(XNV_NITE_MASK_MT_QUEUE, "Listener %s [%08x]: Read from queue (%s)",
				GetListenerName(), this, pMessage->GetType());

			BaseUpdate(pMessage);
			XN_DELETE(pMessage);
			nSessionLength++;
		}
	}
} // XnVMessageListener::Run

XnStatus XnVMessageListener::RunAsThread()
{
	XnStatus rc = xnOSCreateThread(MessageListenerThread, this, &m_hActivityThread);
	if (rc == XN_STATUS_OK)
	{
		m_bInternalThreadAlive = true;
	}
	return rc;
} // XnVMessageListener::RunAsThread

void XnVMessageListener::SetThreadId(XN_THREAD_ID hThread)
{
	m_hActivityThreadId = hThread;
	m_bThreadProtectionQueueMode = true;
} // XnVMessageListener::SetThreadId

void XnVMessageListener::SetCurrentThread()
{
	xnOSGetCurrentThreadID(&m_hActivityThreadId);
	m_bThreadProtectionQueueMode = true;
} // XnVMessageListener::SetCurrentThread

XnBool XnVMessageListener::IsInActivityThread() const
{
	if (!m_bThreadProtectionQueueMode)
	{
		return TRUE;
	}

	XN_THREAD_ID hThread;
	xnOSGetCurrentThreadID(&hThread);

	return hThread == m_hActivityThreadId;
} // XnVMessageListener::IsInActivityThread

XN_THREAD_PROC XnVMessageListener::MessageListenerThread(XN_THREAD_PARAM param)
{
	XnVMessageListener* pMessageListener = (XnVMessageListener*)param;

	pMessageListener->MainLoop();

	XN_THREAD_PROC_RETURN(0);
} // XnVMessageListener::MessageListenerThread

void XnVMessageListener::MainLoop()
{
	SetCurrentThread();

	while (!m_bInternalThreadKill)
	{
		Run();
		xnOSSleep(10);
	}

	m_bInternalThreadAlive = false;
} // XnVMessageListener::MainLoop

XnCallbackHandle XnVMessageListener::RegisterUpdate(void *cxt, MessageUpdateCB CB)
{
	XnCallbackHandle handle;
	m_pUpdateCBs->Register(CB, cxt, &handle);
	return handle;
} // XnVMessageListener::RegisterUpdate
XnCallbackHandle XnVMessageListener::RegisterActivate(void *cxt, ActivateCB CB)
{
	XnCallbackHandle handle;
	m_pActivateCBs->Register(CB, cxt, &handle);
	return handle;
} // XnVMessageListener::RegisterActivate
XnCallbackHandle XnVMessageListener::RegisterDeactivate(void* cxt, DeactivateCB CB)
{
	XnCallbackHandle handle;
	m_pDeactivateCBs->Register(CB, cxt, &handle);
	return handle;
} // XnVMessageListener::RegisterDeactivate

void XnVMessageListener::UnregisterUpdate(XnCallbackHandle handle)
{
	m_pUpdateCBs->Unregister(handle);
} // XnVMessageListener::UnregisterUpdate
void XnVMessageListener::UnregisterActivate(XnCallbackHandle hCB)
{
	m_pActivateCBs->Unregister(hCB);
} // XnVMessageListener::UnregisterActivate
void XnVMessageListener::UnregisterDeactivate(XnCallbackHandle hCB)
{
	m_pDeactivateCBs->Unregister(hCB);
} // XnVMessageListener::UnregisterDeactivate

void XnVMessageListener::SetThreadProtectionQueueMode(XnBool bMode)
{
	m_bThreadProtectionQueueMode = bMode;
} // XnVMessageListener::SetThreadProtectionQueueMode

XnBool XnVMessageListener::GetThreadProtectecionQueueMode() const
{
	return m_bThreadProtectionQueueMode;
} // XnVMessageListener::GetThreadProtectecionQueueMode

const XnChar* XnVMessageListener::GetListenerName() const
{
	return m_strListenerName;
} // XnVMessageListener::GetListenerName

void XnVMessageListener::ClearQueue()
{
	XnAutoCSLocker local(m_hListenerCS);

	if (!m_bThreadProtectionQueueMode)
	{
		return;
	}

	while (!m_pMessageQueue->IsEmpty())
	{
		XnVMessage* pMessage = NULL;
		XnStatus rc = m_pMessageQueue->Pop(pMessage);
		if (rc == XN_STATUS_OK && pMessage != NULL)
		{
			xnLogVerbose(XNV_NITE_MASK_MT_QUEUE, "Listener %s [%08x]: Removing untreated from queue (%s)",
				GetListenerName(), this, pMessage->GetType());
			XN_DELETE(pMessage);
		}
	}
} // XnVMessageListener::ClearQueue
