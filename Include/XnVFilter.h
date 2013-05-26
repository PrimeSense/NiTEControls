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
#ifndef _XNV_FILTER_H_
#define _XNV_FILTER_H_

#include "XnVMessageGenerator.h"
#include "XnVMessageListener.h"

/**
* A XnVFilter is a Message Listener and Generator.
* It receives Messages (as a Listener), may manipulate them, and send them (or others) on (as a Generator)
*/
class XNV_NITE_CONTROLS_API XnVFilter :
	public XnVMessageListener,
	public XnVMessageGenerator
{
public:
	/**
	* Constructor. Create a new filter
	*
	* @param	[in]	strName		Name of the control, for log purposes.
	*/
	XnVFilter(const XnChar* strName = "XnVFilter");

	/**
	* Clear the Multi-threading queue
	*/
	void ClearQueue();
};

#endif
