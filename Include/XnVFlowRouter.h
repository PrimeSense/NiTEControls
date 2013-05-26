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
#ifndef _XNV_FLOW_ROUTER_H_
#define _XNV_FLOW_ROUTER_H_

#include "XnVMessageListener.h"

class XnStringsHash;

/**
* A XnVFlowRouter is a Message Listener, that holds another single Listener internally,
* and sends any Message it gets to that Listener.
* The single internal Listener (referred to as the Active Control) may be changed,
* thus enabling to create the application's automaton.
*/
class XNV_NITE_CONTROLS_API XnVFlowRouter :
	public XnVMessageListener
{
public:
	/**
	* Constructor. Create a new Flow Router
	*
	* @param	[in]	strName		Name of the control, for log purposes.
	*/
	XnVFlowRouter(const XnChar* strName = "XnVFlowRouter");
	virtual ~XnVFlowRouter();

	/**
	* Change the active Control.
	* If a session is kept, it is closed to the old Active Control (CloseOldSession),
	* and then opened to the new Active Control (OpenNewSession)
	*
	* @param	[in]	pActive	The new Active Control
	*/
	XnStatus SetActive(XnVMessageListener* pActive);
	/**
	* Get the current active Control
	*
	* @return	The current active Control
	*/
	XnVMessageListener* GetActive() const;

	/**
	* Send any Message received to the active Control
	*
	* @param	[in]	pMessage	The Message to route to the active Control
	*/
	void Update(XnVMessage* pMessage);

	/**
	* Clear the Multi-threading queue
	*/
	void ClearQueue();
protected:
	virtual void CloseOldSession();
	virtual void OpenNewSession();
	XnVMessageListener* m_pActive;

	XnStringsHash* m_pProperties; // for last points
};

#endif
