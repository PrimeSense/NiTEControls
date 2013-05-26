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
#ifndef _XNV_SESSION_MESSAGE_H_
#define _XNV_SESSION_MESSAGE_H_

#include "XnVCCMessage.h"

/**
* A XnVSessionMessage is a Message holding a boolean value, indicating session start/stop
*/
class XNV_NITE_CONTROLS_API XnVSessionMessage :
	public XnVCCMessage
{
public:
	/**
	* Constructor. Create a Session Message with a boolean. Focus point is (0,0,0)
	*
	* @param	[in]	bStart	Indication of session start/stop
	*/
	XnVSessionMessage(XnBool bStart);
	/**
	* Constructor. Create a Session Message with a boolean, and a focus point.
	*
	* @param	[in]	bStart	Indication of session start/stop
	* @param	[in]	ptFocus	The focus point
	*/
	XnVSessionMessage(XnBool bStart, const XnPoint3D& ptFocus);
	~XnVSessionMessage();

	XnVMessage* Clone() const;

	/**
	* The default type for Session Messages - "Session"
	*/
	static const XnChar* ms_strType;

	const XnPoint3D& GetFocusPoint();
protected:
	XnPoint3D m_ptFocus;
};

#endif // _XNV_SESSION_MESSAGE_H_
