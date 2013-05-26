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
#include "XnVPointMessage.h"

const XnChar* XnVPointMessage::ms_strType = "Points";

XnVPointMessage::XnVPointMessage(const XnVMultipleHands* pHands) :
	XnVMessage(ms_strType, (void*)pHands), m_pHands(pHands)
	{}

XnVPointMessage::~XnVPointMessage()
{
	if (m_bFreeOnDestruction)
	{
		XN_DELETE(m_pHands);
	}
} // XnVPointMessage::~XnVPointMessage

XnVMessage* XnVPointMessage::Clone() const
{
	XnVMultipleHands* pHands = XN_NEW(XnVMultipleHands);
	if (pHands == NULL)
	{
		return NULL;
	}

	m_pHands->Clone(*pHands);

	XnVPointMessage* pNewMessage = XN_NEW(XnVPointMessage, pHands);
	if (pNewMessage == NULL)
	{
		XN_DELETE(pHands);
		return NULL;
	}
	pNewMessage->m_bFreeOnDestruction = true;

	return pNewMessage;
} // XnVPointMessage::Clone
