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
#include "XnVSessionGenerator.h"
#include "XnVSessionListenerList.h"
#include "XnVSessionMessage.h"
#include "XnVNiteLog.h"

class XnVSessionListenerFuncContainer : public XnVSessionListener
{
public:
	XnVSessionListenerFuncContainer(void* pUserContext, OnSessionStartCB pCreateCB, OnSessionEndCB pDestroyCB,
		OnFocusStartDetectedCB pFocusStartDetectedCB = NULL) :
	m_pUserContext(pUserContext),
		m_pCreateCB(pCreateCB),
		m_pDestroyCB(pDestroyCB),
		m_pFocusStartDetectedCB(pFocusStartDetectedCB)
	{}

	void OnSessionStart(const XnPoint3D& ptPosition)
	{
		(*m_pCreateCB)(ptPosition, m_pUserContext);
	} // OnSessionStart

	void OnSessionEnd()
	{
		(*m_pDestroyCB)(m_pUserContext);
	} // OnSessionEnd

	void OnFocusStartDetected(const XnChar* strFocus, const XnPoint3D& ptPosition, XnFloat fProgress)
	{
		if (NULL != m_pFocusStartDetectedCB)
		{
			(*m_pFocusStartDetectedCB)(strFocus, ptPosition, fProgress, m_pUserContext);
		}
	}
protected:
	OnSessionStartCB m_pCreateCB;
	OnSessionEndCB m_pDestroyCB;
	OnFocusStartDetectedCB m_pFocusStartDetectedCB;
	void* m_pUserContext;
}; // XnVSessionListenerFuncContainer


XnVSessionGenerator::XnVSessionGenerator(const XnChar* strName) :
	XnVMessageGenerator(strName), m_bInSession(FALSE)
{
	m_pSessionListeners = XN_NEW(XnVSessionListenerList);
} // XnVSessionGenerator::XnVSessionGenerator

XnVSessionGenerator::~XnVSessionGenerator()
{
	XN_DELETE(m_pSessionListeners);
} // XnVSessionGenerator::~XnVSessionGenerator

XnVHandle XnVSessionGenerator::RegisterSession(XnVSessionListener* pListener)
{
	return m_pSessionListeners->MarkAdd(pListener, false);
} // XnVSessionGenerator::RegisterSession

XnVHandle XnVSessionGenerator::RegisterSession(void* cxt,
											   XnVSessionListener::OnSessionStartCB StartCB,
											   XnVSessionListener::OnSessionEndCB EndCB,
											   XnVSessionListener::OnFocusStartDetectedCB MidCB)
{
	XnVSessionListener* pSessionListener = XN_NEW(XnVSessionListenerFuncContainer, cxt, StartCB, EndCB, MidCB);
	if (pSessionListener == NULL)
	{
		return XN_HANDLE_INVALID;
	}

	return m_pSessionListeners->MarkAdd(pSessionListener, true);
} // XnVSessionGenerator::RegisterSession

void XnVSessionGenerator::UnregisterSession(XnVSessionListener* pSessionListener)
{
	XnUInt32 nHandle = m_pSessionListeners->GetID(pSessionListener);
	m_pSessionListeners->MarkRemove(nHandle);
} // XnVSessionGenerator::UnregisterSession

void XnVSessionGenerator::UnregisterSession(XnVHandle nHandle)
{
	m_pSessionListeners->MarkRemove(nHandle);
} // XnVSessionGenerator::UnregisterSession

void XnVSessionGenerator::SessionStart(const XnPoint3D& ptFocus)
{
	xnLogVerbose(XNV_NITE_MASK_SESSION, "Session Created, focus at (%f,%f,%f)",
		ptFocus.X, ptFocus.Y, ptFocus.Z);

	// Call callbacks
	m_ptFocusPoint = ptFocus;
	m_bInSession = true;

	m_pSessionListeners->UpdateLists();

	for (XnVSessionListenerList::Iterator iter = m_pSessionListeners->begin();
		iter != m_pSessionListeners->end(); iter++)
	{
		XnVSessionListener* pListener = iter.Value();
		pListener->OnSessionStart(ptFocus);
	}
	m_pSessionListeners->UpdateLists();

	XnVSessionMessage sessionMessage(true, ptFocus);
	Generate(&sessionMessage);
} // XnVSessionGenerator::SessionStart

void XnVSessionGenerator::SessionStop()
{
	xnLogVerbose(XNV_NITE_MASK_SESSION, "Session Ended");
	m_bInSession = false;

	XnVSessionMessage sessionMessage(false);
	Generate(&sessionMessage);

	m_pSessionListeners->UpdateLists();

	for (XnVSessionListenerList::Iterator iter = m_pSessionListeners->begin();
		iter != m_pSessionListeners->end(); iter++)
	{
		XnVSessionListener* pListener = iter.Value();
		pListener->OnSessionEnd();
	}

	m_pSessionListeners->UpdateLists();
} // XnVSessionGenerator::SessionStop

void XnVSessionGenerator::SessionMidGesture(const XnChar* strFocus, const XnPoint3D& ptFocus, XnFloat fProgress)
{
	xnLogVerbose(XNV_NITE_MASK_SESSION, "Session about to start, at (%f,%f,%f), by gesture '%s'. Progress %f",
		ptFocus.X, ptFocus.Y, ptFocus.Z, strFocus, fProgress);

	// Call callbacks
	m_pSessionListeners->UpdateLists();

	for (XnVSessionListenerList::Iterator iter = m_pSessionListeners->begin();
		iter != m_pSessionListeners->end(); iter++)
	{
		XnVSessionListener* pListener = iter.Value();
		pListener->OnFocusStartDetected(strFocus, ptFocus, fProgress);
	}
	m_pSessionListeners->UpdateLists();
} // XnVSessionGenerator::SessionMidGesture

XnBool XnVSessionGenerator::IsInSession() const
{
	return m_bInSession;
} // XnVSessionGenerator::IsInSession

XnStatus XnVSessionGenerator::GetFocusPoint(XnPoint3D& ptFocus) const
{
	ptFocus = m_ptFocusPoint;
	return XN_STATUS_OK;
}
