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
#include "XnVSessionMessage.h"

const XnChar* XnVSessionMessage::ms_strType = "Session";

XnVSessionMessage::XnVSessionMessage(XnBool bSession) :
XnVCCMessage(ms_strType, bSession)
{
	m_ptFocus = xnCreatePoint3D(0,0,0);
}

XnVSessionMessage::XnVSessionMessage(XnBool bSession, const XnPoint3D& ptFocus) :
XnVCCMessage(ms_strType, bSession), m_ptFocus(ptFocus)
{}

XnVSessionMessage::~XnVSessionMessage()
{}

XnVMessage* XnVSessionMessage::Clone() const
{
	return XN_NEW(XnVSessionMessage, GetValue(), m_ptFocus);
} // XnVSessionMessage::Clone

const XnPoint3D& XnVSessionMessage::GetFocusPoint()
{
	return m_ptFocus;
} // XnVSessionMessage::GetFocusPoint
