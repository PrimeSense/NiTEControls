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
#include "XnVCCMessage.h"

XnVCCMessage::XnVCCMessage(const XnChar* strType, XnBool bValue) :
	XnVMessage(strType, (void*)(XnSizeT(bValue))),
	m_bValue(bValue)
	{}

XnVCCMessage::~XnVCCMessage()
{}

XnVMessage* XnVCCMessage::Clone() const
{
	return XN_NEW(XnVCCMessage, GetType(), m_bValue);
} // XnVCCMessage::Clone

XnBool XnVCCMessage::GetValue() const
{
	return m_bValue;
} // XnVCCMessage::GetValue

XnStatus XnVCCMessage::GetValue(XnVMessage* pMessage, const XnChar* strType, XnBool& bValue)
{
	XnVCCMessage* pMsg = (XnVCCMessage*)(*pMessage)[strType];
	if (pMsg == NULL)
	{
		return XN_STATUS_ERROR;
	}

	bValue = pMsg->GetValue();
	return XN_STATUS_OK;
} // XnVCCMessage::GetValue
