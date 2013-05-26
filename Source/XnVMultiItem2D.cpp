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
#include <list>
#include <XnOpenNI.h>
#include "XnVMultiItem2D.h"
#include "XnVPointBuffer.h"
#include "XnVNiteStatus.h"
#include "XnVNiteEvents.h"

class XnVSigmoid
{
public:
	XnVSigmoid(XnFloat fInMin, XnFloat fInMax, XnFloat fOutMin, XnFloat fOutMax) :
	  m_fInMin(fInMin), m_fInMax(fInMax), m_fOutMin(fOutMin), m_fOutMax(fOutMax)
	  {}

	  XnFloat GetValue(XnFloat fIn)
	  {
		  if (fIn < m_fInMin) return m_fOutMin;
		  if (fIn > m_fInMax) return m_fOutMax;
		  return ((fIn-m_fInMin) / GetInLength()) * GetOutLength() + m_fOutMin;
	  }

	  XnFloat GetInLength() {return m_fInMax-m_fInMin;}
	  XnFloat GetOutLength() {return m_fOutMax-m_fOutMin;}
protected:
	XnFloat m_fInMin;
	XnFloat m_fInMax;
	XnFloat m_fOutMin;
	XnFloat m_fOutMax;
}; // XnVSigmoid

XnVMultiItem2D::XnVMultiItem2D(XnInt32 nItemXCount, XnInt32 nItemYCount)
{
	m_pItemSelectCBs = XN_NEW(XnVIntIntSpecificEvent);
	m_nItemXCount = nItemXCount;
	m_nItemYCount = nItemYCount;

	m_nLastSelectedXIndex = -1;
	m_nLastSelectedYIndex = -1;

	CalculateMinAndMax();
} // XnVMultiItem2D::XnVMultiItem2D

XnVMultiItem2D::~XnVMultiItem2D()
{
	XN_DELETE(m_pItemSelectCBs);
} // XnVMultiItem2D::~XnVMultiItem2D

void XnVMultiItem2D::CalculateMinAndMax()
{
	XnFloat fItemRange = 1.0f / m_nItemXCount;
	m_fMinXValue = 0.0f;
	m_fMaxXValue = 1.0f;

	fItemRange = 1.0f / m_nItemYCount;
	m_fMinYValue = 0.0f;
	m_fMaxYValue = 1.0f;
} // XnVMultiItem2D::CalculateMinAndMax

void XnVMultiItem2D::LostPoint()
{
	m_nLastSelectedXIndex = -1;
	m_nLastSelectedYIndex = -1;
} // XnVMultiItem2D::LostPoint

XnBool XnVMultiItem2D::IsValueInRange(XnFloat fXValue, XnFloat fYValue)
{
	return (fXValue >= m_fMinXValue) && (fXValue <= m_fMaxXValue) &&
		(fYValue >= m_fMinYValue) && (fYValue <= m_fMaxYValue);
} // XnVMultiItem2D::IsValueInRange

XnStatus XnVMultiItem2D::Update(XnFloat fXValue, XnFloat fYValue)
{
	if (!IsValueInRange(fXValue, fYValue))
	{
		LostPoint();
		return XN_STATUS_NITE_OUT_OF_RANGE;
	}

	XnFloat fNormalizedXValue = fXValue;
	XnFloat fNormalizedYValue = fYValue;

	XnFloat fItemXRange = 1.0f / m_nItemXCount;
	XnFloat fItemYRange = 1.0f / m_nItemYCount;
	XnInt32 nCurrentSelectedXIndex = -1;
	XnInt32 nCurrentSelectedYIndex = -1;

	if (-1 != m_nLastSelectedXIndex && -1 != m_nLastSelectedYIndex)
	{
		XnFloat fValidMinXValue = m_nLastSelectedXIndex * fItemXRange;
		XnFloat fValidMinYValue = m_nLastSelectedYIndex * fItemYRange;
		XnFloat fValidMaxXValue = (m_nLastSelectedXIndex + 1) * fItemXRange;
		XnFloat fValidMaxYValue = (m_nLastSelectedYIndex + 1) * fItemYRange;

		XnBool bChanged = false;

		if (fNormalizedXValue < fValidMinXValue || fNormalizedXValue > fValidMaxXValue)
		{
			nCurrentSelectedXIndex = (int)(fNormalizedXValue / fItemXRange);

			// clamp to our desired range
			if (nCurrentSelectedXIndex >= m_nItemXCount)
			{
				nCurrentSelectedXIndex = m_nItemXCount - 1;
			}
			else if (nCurrentSelectedXIndex < 0)
			{
				nCurrentSelectedXIndex = 0;
			}
			bChanged = true;
		}
		else
		{
			nCurrentSelectedXIndex = m_nLastSelectedXIndex;
		}
		if (fNormalizedYValue < fValidMinYValue || fNormalizedYValue > fValidMaxYValue)
		{
			nCurrentSelectedYIndex = (int)(fNormalizedYValue / fItemYRange);

			// clamp to our desired range
			if (nCurrentSelectedYIndex >= m_nItemYCount)
			{
				nCurrentSelectedYIndex = m_nItemYCount - 1;
			}
			else if (nCurrentSelectedYIndex < 0)
			{
				nCurrentSelectedYIndex = 0;
			}
			bChanged = true;
		}
		else
		{
			nCurrentSelectedYIndex = m_nLastSelectedYIndex;
		}
	} // if (-1 != m_nLastSelectedXIndex && -1 != m_nLastSelectedYIndex)
	else
	{
		if (-1 == m_nLastSelectedXIndex)
		{
			nCurrentSelectedXIndex = (int)(fNormalizedXValue / fItemXRange);
			// clamp to our desired range
			if (nCurrentSelectedXIndex >= m_nItemXCount)
			{
				nCurrentSelectedXIndex = m_nItemXCount - 1;
			}
			else if (nCurrentSelectedXIndex < 0)
			{
				nCurrentSelectedXIndex = 0;
			}
		}
		if (-1 == m_nLastSelectedYIndex)
		{
			nCurrentSelectedYIndex = (int)(fNormalizedYValue / fItemYRange);
			// clamp to our desired range
			if (nCurrentSelectedYIndex >= m_nItemYCount)
			{
				nCurrentSelectedYIndex = m_nItemYCount - 1;
			}
			else if (nCurrentSelectedYIndex < 0)
			{
				nCurrentSelectedYIndex = 0;
			}
		}
	} // if (-1 != m_nLastSelectedXIndex && -1 != m_nLastSelectedYIndex) else

	// if we have a new selected index, fire our event
	if (nCurrentSelectedXIndex != -1 && nCurrentSelectedYIndex != -1 &&
		((nCurrentSelectedXIndex != m_nLastSelectedXIndex) || (nCurrentSelectedYIndex != m_nLastSelectedYIndex)))
	{
		m_nLastSelectedXIndex = nCurrentSelectedXIndex;
		m_nLastSelectedYIndex = nCurrentSelectedYIndex;
		m_pItemSelectCBs->Raise(m_nLastSelectedXIndex, m_nLastSelectedYIndex);
	}

	return XN_STATUS_OK;
} // XnVMultiItem2D::Update

XnCallbackHandle XnVMultiItem2D::RegisterItemSelect(void* cxt, ItemSelectCB pCB)
{
	XnCallbackHandle handle;
	m_pItemSelectCBs->Register(pCB, cxt, &handle);
	return handle;
} // XnVMultiItem2D::RegisterItemSelect

void XnVMultiItem2D::UnregisterItemSelect(XnCallbackHandle handle)
{
	m_pItemSelectCBs->Unregister(handle);
} // XnVMultiItem2D::UnregisterItemSelect

XnFloat XnVMultiItem2D::GetMinXValue() const
{
	return m_fMinXValue;
} // XnVMultiItem2D::GetMinXValue

XnFloat XnVMultiItem2D::GetMinYValue() const
{
	return m_fMinYValue;
} // XnVMultiItem2D::GetMinYValue

XnFloat XnVMultiItem2D::GetMaxXValue() const
{
	return m_fMaxXValue;
} // XnVMultiItem2D::GetMaxXValue

XnFloat XnVMultiItem2D::GetMaxYValue() const
{
	return m_fMaxYValue;
} // XnVMultiItem2D::GetMaxYValue

XnUInt32 XnVMultiItem2D::GetItemXCount() const
{
	return m_nItemXCount;
}
XnUInt32 XnVMultiItem2D::GetItemYCount() const
{
	return m_nItemYCount;
}

void XnVMultiItem2D::SetItemXCount(XnUInt32 nItemXCount)
{
	if  (m_nItemXCount != nItemXCount)
	{
		m_nItemXCount = nItemXCount;
		LostPoint();
		CalculateMinAndMax();
	}
} // XnVMultiItem2D::SetItemXCount

void XnVMultiItem2D::SetItemYCount(XnUInt32 nItemYCount)
{
	if (m_nItemYCount != nItemYCount)
	{
		m_nItemYCount = nItemYCount;
		LostPoint();
		CalculateMinAndMax();
	}
} // XnVMultiItem2D::SetItemYCount

void XnVMultiItem2D::SetItemCount(XnUInt32 nItemXCount, XnUInt32 nItemYCount)
{
	SetItemXCount(nItemXCount);
	SetItemYCount(nItemYCount);
} // XnVMultiItem2D::SetItemCount
