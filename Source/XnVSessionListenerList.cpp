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
#include "XnVSessionListenerList.h"
#include "XnVNiteLog.h"

XnVSessionListenerList::XnVSessionListenerList() :
	m_nSessionID(0)
{}

XnVSessionListenerList::~XnVSessionListenerList()
{
	for (XnVIntSessionListenerHash::Iterator iter = m_SessionListeners.begin(); iter != m_SessionListeners.end(); ++iter)
	{
		XnVHandle nID = iter.Key();
		MarkRemove(nID);
	}
	for (XnVIntSessionListenerHash::Iterator iter = m_AddList.begin(); iter != m_AddList.end(); ++iter)
	{
		XnVHandle nID = iter.Key();
		MarkRemove(nID);
	}
	UpdateLists();
} // XnVSessionListenerList::~XnVSessionListenerList

XnVHandle XnVSessionListenerList::MarkAdd(XnVSessionListener *pListener, XnBool bVirtual)
{
	for (XnVIntSessionListenerHash::Iterator iter = m_SessionListeners.begin(); iter != m_SessionListeners.end(); iter++)
	{
		XnVSessionListener* pCurListener = iter.Value();
		if (pCurListener == pListener)
		{
			return iter.Key();
		}
	}
	m_nSessionID++;
	m_AddList.Set(m_nSessionID, pListener);

	if (bVirtual)
	{
		m_SessionVirtualMap.AddLast(m_nSessionID);
	}

	xnLogVerbose(XNV_NITE_MASK_CONNECT, "Marking to add SessionListener %d [0x%08x] %s",
		m_nSessionID,		
		pListener, bVirtual?"v":"");

	return m_nSessionID;

} // XnVSessionListenerList::MarkAdd

void XnVSessionListenerList::MarkRemove(XnVHandle nHandle)
{
	xnLogVerbose(XNV_NITE_MASK_CONNECT, "Marking to remove SessionListener %d", nHandle);
	m_EraseList.AddLast(nHandle);
} // XnVSessionListenersList::MarkRemove

XnVHandle XnVSessionListenerList::GetID(XnVSessionListener* pSessionListener)
{
	for (XnVIntSessionListenerHash::Iterator iter = m_SessionListeners.begin(); iter != m_SessionListeners.end(); ++iter)
	{
		XnVSessionListener* pCurListener = iter.Value();
		if (pCurListener == pSessionListener)
		{
			return iter.Key();
		}
	}
	return 0;
} // XnVSessionListenersList::GetID

void XnVSessionListenerList::UpdateLists()
{
	// Add everything from add list
	for (XnVIntSessionListenerHash::Iterator miter = m_AddList.begin(); miter != m_AddList.end(); ++miter)
	{
		xnLogVerbose(XNV_NITE_MASK_CONNECT, "Adding SessionListener %d [0x%08x]", miter.Key(), miter.Value());
		m_SessionListeners.Set(miter.Key(), miter.Value());
	}
	m_AddList.Clear();

	// remove anything in the erase list from the real list.
	for (XnVIntList::Iterator  iter = m_EraseList.begin(); iter != m_EraseList.end(); ++iter)
	{
		XnVSessionListener* pListener;
		if (m_SessionListeners.Get(*iter, pListener) != XN_STATUS_NO_MATCH)
		{
			xnLogVerbose(XNV_NITE_MASK_CONNECT, "Removing SessionListener %d [0x%08x]", *iter, pListener);

			m_SessionListeners.Remove(*iter);
			XnVIntList::Iterator virtualListener = m_SessionVirtualMap.Find(*iter);
			if (virtualListener != m_SessionVirtualMap.end())
			{
				// From virtual map
				XN_DELETE(pListener);
				m_SessionVirtualMap.Remove(virtualListener);
			}
		}
	} // for

	m_EraseList.Clear();
} // XnVSessionListenersList::UpdateLists

XnVSessionListenerList::Iterator XnVSessionListenerList::begin()
{
	return m_SessionListeners.begin();
} // XnVSessionListenerList::begin

XnVSessionListenerList::Iterator XnVSessionListenerList::end()
{
	return m_SessionListeners.end();
} // XnVSessionListenerList::end
