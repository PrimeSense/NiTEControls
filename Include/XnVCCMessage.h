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
#ifndef _XNV_CC_MESSAGE_H_
#define _XNV_CC_MESSAGE_H_

#include "XnVMessage.h"

/**
* A generic Message holding a single boolean value, basically for command and control (CC)
*/
class XNV_NITE_CONTROLS_API XnVCCMessage :
	public XnVMessage
{
public:
	/**
	* Constructor. Create a CC Message with a boolean and a specific type
	*
	* @param	[in]	strType	The type of the C&C Message
	* @param	[in]	bValue	The internal value
	*/
	XnVCCMessage(const XnChar* strType, XnBool bValue);
	~XnVCCMessage();

	/**
	* Create a new Message of the same type (CC Message), with a clone of the same data (the same boolean value)
	*
	* @return	The new message
	*/
	XnVMessage* Clone() const;

	/**
	* Get the internal value
	*
	* @return The internal value
	*/
	XnBool GetValue() const;

	static XnStatus GetValue(XnVMessage* pMessage, const XnChar* strType, XnBool& bValue);
protected:
	XnBool m_bValue;
};

#endif