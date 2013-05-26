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
#include "XnVPointFilter.h"
#include "XnVPointMessage.h"

XnVPointFilter::XnVPointFilter(const XnChar* strName) :
	XnVPointControl(strName),
	XnVMessageGenerator(strName)
{}

void XnVPointFilter::Update(XnVMessage* pMessage)
{
	XnVPointControl::Update(pMessage);

	if (m_nOverridePrimary != 0)
	{
		XnVMultipleHands newHands;
		XnVPointMessage* pPointMessage = XNV_GET_SPECIFIC_MESSAGE(pMessage, XnVPointMessage);

		if (pPointMessage != NULL)
		{
			const XnVMultipleHands* pHands = (const XnVMultipleHands*)pPointMessage->GetData();
			pHands->Clone(newHands);
		}
		GenerateReplaced(pMessage, newHands);
	}
	else
	{
		Generate(pMessage);
	}
} // XnVPointFilter::Update

void XnVPointFilter::ClearQueue()
{
	XnVMessageListener::ClearQueue();
	ClearAllQueues();
} // XnVPointFilter::ClearQueue

void XnVPointFilter::GenerateReplaced(XnVMessage* pMessage, XnVMultipleHands& newHands)
{
	if (m_nOverridePrimary != 0)
	{
		if (newHands.GetContext(m_nOverridePrimary) != NULL)
		{
			newHands.ReassignPrimary(m_nOverridePrimary);
		}
		else
		{
			m_nOverridePrimary = 0;
		}
	}

	XnVPointMessage pointMessage(&newHands);

	XnVPointMessage* pOrigPointMessage = dynamic_cast<XnVPointMessage*>(pMessage);
	if (pOrigPointMessage != NULL)
	{
		// It's a point message - send the newly created PointMessage instead.
		Generate(&pointMessage);
	}
	else
	{
		// Not a point message - just let it through
		Generate(pMessage);
	}
} // XnVPointFilter::GenerateReplaced

void XnVPointFilter::OverridePrimary(XnUInt32 nNewPrimary)
{
	m_nOverridePrimary = nNewPrimary;
}