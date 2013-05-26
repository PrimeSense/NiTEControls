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
#ifndef _XNV_SESSION_LISTENER_LIST_H_
#define _XNV_SESSION_LISTENER_LIST_H_

#include "XnVSessionListener.h"
#include "XnVHandle.h"
#include "XnVNiteEvents.h"

XN_DECLARE_DEFAULT_HASH(XnUInt32, XnVSessionListener*, XnVIntSessionListenerHash);

class XnVSessionListenerList
{
public:
	XnVSessionListenerList();
	~XnVSessionListenerList();

	XnVHandle MarkAdd(XnVSessionListener* pListener, XnBool bVirtual);
	void MarkRemove(XnVHandle hListener);

	XnVHandle GetID(XnVSessionListener* pListener);
	void UpdateLists();

	typedef XnVIntSessionListenerHash::Iterator Iterator;

	Iterator begin();
	Iterator end();
protected:
	XnVIntSessionListenerHash m_SessionListeners;
	XnVIntList m_SessionVirtualMap;

	XnVIntSessionListenerHash m_AddList;
	XnVIntList m_EraseList;

	XnVHandle m_nSessionID;
};

#endif // _XNV_SESSION_LISTENER_LIST_H_
