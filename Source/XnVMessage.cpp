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
#include <XnOS.h>
#include "XnVMessage.h"

XnVMessage::XnVMessage(const XnChar* strType, void* pData) :
	m_pData(pData), m_bFreeOnDestruction(false)
{
	m_strType = (XnChar*)xnOSCalloc(strlen(strType)+1, sizeof(XnChar));
	xnOSStrCopy(m_strType, strType, strlen(strType)+1);
} // XnVMessage::XnVMessage

XnVMessage::~XnVMessage()
{
	xnOSFree(m_strType);
} // XnVMessage::~XnVMessage

void* XnVMessage::GetData()
{
	return m_pData;
} // XnVMessage::GetData

XnVMessage* XnVMessage::operator [](const XnChar *strType)
{
	if (strcmp(m_strType, strType) != 0)
	{
		return NULL;
	}
	return this;
} // XnVMessage::operator[]

const XnChar* XnVMessage::GetType() const
{
	return m_strType;
} // XnVMessage::GetType
