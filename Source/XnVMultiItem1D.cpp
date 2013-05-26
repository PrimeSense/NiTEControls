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
#include "XnVMultiItem1D.h"
#include "XnVNiteStatus.h"
#include "XnVNiteEvents.h"

XnVMultiItem1D::XnVMultiItem1D(XnInt32 nItemCount)
{
	m_nItemCount = 1;

	SetItemCount(nItemCount);
	m_nLastSelectedIndex = -1;

	m_pItemSelectCBs = XN_NEW(XnVIntSpecificEvent);

	CalculateMinAndMax();
} // XnVMultiItem1D::XnVMultiItem1D

XnVMultiItem1D::~XnVMultiItem1D()
{
	XN_DELETE(m_pItemSelectCBs);
} // XnVMultiItem1D::~XnVMultiItem1D

XnStatus XnVMultiItem1D::Update(XnFloat Value)
{
	// NOTE: Assumes Value is 0.0-1.0, which includes border regions
	if (!IsValueInRange(Value))
	{
		// we got a point that wasn't in our range
		LostPoint();
		return XN_STATUS_NITE_OUT_OF_RANGE;
	}

	// normalize the value to be between 0.0-1.0, after removing border regions
	XnFloat fNormalizedValue = Value;

	XnFloat fItemRange = 1.0f / GetItemCount();
	XnInt32 nCurrentSelectedIndex = -1;

	// if an item is highlighted, make sure value is still in its extended range
	if (-1 != m_nLastSelectedIndex)
	{
		XnFloat fValidMinValue = m_nLastSelectedIndex * fItemRange;
		XnFloat fValidMaxValue = (m_nLastSelectedIndex + 1) * fItemRange;

		if (fNormalizedValue < fValidMinValue || fNormalizedValue > fValidMaxValue)
		{
			nCurrentSelectedIndex = (int)(fNormalizedValue / fItemRange);

			// clamp to our desired range
			if (nCurrentSelectedIndex >= GetItemCount())
			{
				nCurrentSelectedIndex = GetItemCount() - 1;
			} else if (nCurrentSelectedIndex < 0) {
				nCurrentSelectedIndex = 0;
			}
		}
	} // if (-1 != m_nLastSelectedIndex)
	else
	{
		nCurrentSelectedIndex = (int)(fNormalizedValue / fItemRange);
		// clamp to our desired range
		if (nCurrentSelectedIndex >= GetItemCount())
		{
			nCurrentSelectedIndex = GetItemCount() - 1;
		}
		else if (nCurrentSelectedIndex < 0)
		{
			nCurrentSelectedIndex = 0;
		}
	} // if (-1 != m_nLastSelectedIndex) else

	// if we have a new selected index, fire our event
	if (nCurrentSelectedIndex != m_nLastSelectedIndex && -1 != nCurrentSelectedIndex)
	{
		m_nLastSelectedIndex = nCurrentSelectedIndex;
		ItemSelect(nCurrentSelectedIndex);
	}

	return XN_STATUS_OK;
} // XnVMultiItem1D::Update

XnCallbackHandle XnVMultiItem1D::RegisterItemSelect(void* cxt, ItemSelectedCB pCB)
{
	XnCallbackHandle handle;
	m_pItemSelectCBs->Register(pCB, cxt, &handle);
	return handle;
} // XnVMultiItem1D::RegisterItemSelect

void XnVMultiItem1D::UnregisterItemSelect(XnCallbackHandle handle)
{
	m_pItemSelectCBs->Unregister(handle);
} // XnVMultiItem1D::UnregisterItemSelect

void XnVMultiItem1D::LostPoint()
{
	m_nLastSelectedIndex = -1;
} // XnVMultiItem1D::LostPoint

void XnVMultiItem1D::ItemSelect(XnInt32 nItemIndex)
{
	m_pItemSelectCBs->Raise(nItemIndex);
} // XnVMultiItem1D::ItemSelect

// ItemCount
XnUInt32 XnVMultiItem1D::GetItemCount() const
{
	return m_nItemCount;
} // XnVMultiItem1D::GetItemCount

void XnVMultiItem1D::SetItemCount(XnUInt32 nCount)
{
	if (m_nItemCount != nCount)
	{
		m_nItemCount = nCount;
		LostPoint();
		CalculateMinAndMax();
	}
} // XnVMultiItem1D::SetItemCount

// Min/Max Values
XnFloat XnVMultiItem1D::GetMinValue() const
{
	return m_fMinValue;
} // XnVMultiItem1D::GetMinValue

XnFloat XnVMultiItem1D::GetMaxValue() const
{
	return m_fMaxValue;
} // XnVMultiItem1D::GetMaxValue

void XnVMultiItem1D::CalculateMinAndMax()
{
	XnFloat fItemRange = 1.0f / GetItemCount();
	m_fMinValue = 0.0f;
	m_fMaxValue = 1.0f;
} // XnVMultiItem1D::CalculateMinAndMax

bool XnVMultiItem1D::IsValueInRange(XnFloat fValue)
{
	return (fValue >= GetMinValue()) && (fValue <= GetMaxValue());
} // XnVMultiItem1D::IsValueInRange
