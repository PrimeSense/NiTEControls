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
#ifndef _XNV_BROADCASTER_H_
#define _XNV_BROADCASTER_H_

#include "XnVFilter.h"

/**
* A XnVBroadcast is a specific Filter, which sends the messages it receives to its listeners, unchanged.
*/
class XNV_NITE_CONTROLS_API XnVBroadcaster :
	public XnVFilter
{
public:
	/**
	* Constructor. Create a new Broadcaster
	*
	* @param	[in]	strName		Name of the control, for log purposes.
	*/
	XnVBroadcaster(const XnChar* strName = "Broadcaster");
	/**
	* Send all received messages to listeners.
	*
	* @param	[in]	pMessage	The message to handle
	*/
	void Update(XnVMessage* pMessage);
};

#endif // _XNV_BROADCASTER_H_
