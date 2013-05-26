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
#include <XnVSelectableSlider2D.h>
#include <XnVHandPointContext.h>
#include "XnVNiteDefs.h"
#include "XnVNiteEvents.h"

XN_DECLARE_EVENT_3ARG(XnVItemSelectSpecificEvent, XnVItemSelectEvent, XnInt32, nItemX, XnInt32, nItemY, XnVDirection, eDir);

const XnFloat XnVSelectableSlider2D::ms_fDefaultSliderSize = 450;
const XnFloat XnVSelectableSlider2D::ms_fSecondarySliderSize = 150;

void XN_CALLBACK_TYPE XnVSelectableSlider2D::OffAxisHysteresis_ItemSelected(XnInt32 nItem, void* cxt)
{
	XnVSelectableSlider2D* pTP = (XnVSelectableSlider2D*)cxt;
	XnVDirection eDirection;
	if (nItem == 0)
	{
		eDirection = DIRECTION_FORWARD;
	}
	else if (nItem == pTP->m_pOffAxisHysteresis->GetItemCount()-1)
	{
		eDirection = DIRECTION_BACKWARD;
	}
	else
	{
		return;
	}

	pTP->ItemSelect(eDirection);
} // XnVSelectableSlider2D::OffAxisHysteresis_ItemSelected

void XN_CALLBACK_TYPE XnVSelectableSlider2D::SecondarySlider_OffAxisMovement(XnVDirection eDir, void* cxt)
{
	XnVSelectableSlider2D* pTP = (XnVSelectableSlider2D*)cxt;
	pTP->m_pOffAxisHysteresis->LostPoint();
	pTP->m_bInOffAxis = false;
} // XnVSelectableSlider2D::SecondarySlider_OffAxisMovement

void XN_CALLBACK_TYPE XnVSelectableSlider2D::SecondarySlider_ValueChange(XnFloat fValue, void* cxt)
{
	XnVSelectableSlider2D* pTP = (XnVSelectableSlider2D*)cxt;
	pTP->m_pOffAxisHysteresis->Update(fValue);	
} // XnVSelectableSlider2D::SecondarySlider_ValueChange

void XN_CALLBACK_TYPE XnVSelectableSlider2D::Slider_ValueChange(XnFloat fXValue, XnFloat fYValue, void* pContext)
{
	XnVSelectableSlider2D* pTP = (XnVSelectableSlider2D*)pContext;

	pTP->UpdateSlider(fXValue, fYValue);
} // XnVSelectableSlider2D::Slider_ValueChange

void XN_CALLBACK_TYPE XnVSelectableSlider2D::Hysteresis_ItemSelected(XnInt32 nXItem, XnInt32 nYItem, void* pContext)
{
	XnVSelectableSlider2D* pTP = (XnVSelectableSlider2D*)pContext;
	pTP->ItemHover(nXItem, nYItem);
} // XnVSelectableSlider2D::Hysteresis_ItemSelected

void XN_CALLBACK_TYPE XnVSelectableSlider2D::Slider_OffAxis(XnVDirection eDir, void* cxt)
{
	XnVSelectableSlider2D* pSS2 = (XnVSelectableSlider2D*)cxt;

	pSS2->m_pHysteresis->LostPoint();
	pSS2->m_bInOffAxis = true;
	if (pSS2->m_pSecondarySlider == NULL)
	{
		pSS2->m_pSecondarySlider = XN_NEW(XnVSlider1D, AXIS_Z, pSS2->m_pSlider->GetPosition(), ms_fSecondarySliderSize, .5, 0.0, 1.0);
		pSS2->m_hSecondarySliderOffAxis = pSS2->m_pSecondarySlider->RegisterOffAxisMovement(pSS2, SecondarySlider_OffAxisMovement);
		pSS2->m_hSecondarySliderValueChange = pSS2->m_pSecondarySlider->RegisterValueChange(pSS2, SecondarySlider_ValueChange);
	}
	else
	{
		pSS2->m_pSecondarySlider->Reinitialize(AXIS_Z, pSS2->m_pSlider->GetPosition(), ms_fSecondarySliderSize, 0.5, 0.0, 1.0);
	}
	pSS2->OffAxisMovement(eDir);
} // XnVSelectableSlider2D::Slider_OffAxis

void XnVSelectableSlider2D::UpdateSlider(XnFloat fXValue, XnFloat fYValue)
{
	XnFloat fXValueNormalized = fXValue;
	XnFloat fYValueNormalized = fYValue;

	ValueChange(fXValueNormalized, fYValueNormalized);

	m_pHysteresis->Update(fXValue, fYValue);
} // XnVSelectableSlider2D::UpdateSlider

XnVSelectableSlider2D::XnVSelectableSlider2D(XnInt32 nXItems, XnInt32 nYItems, XnFloat fSliderSizeX, XnFloat fSliderSizeY,
						 const XnChar* strName) :
	XnVPointControl(strName),
	m_bValueChangeOnOffAxis(false)
{
	m_pItemHoverCBs = XN_NEW(XnVIntIntSpecificEvent);
	m_pValueChangeCBs = XN_NEW(XnVFloatFloatSpecificEvent);
	m_pItemSelectCBs = XN_NEW(XnVItemSelectSpecificEvent);
	m_pOffAxisMovementCBs = XN_NEW(XnVDirectionSpecificEvent);

	m_nXItems = nXItems;
	m_nYItems = nYItems;

	m_pHysteresis = XN_NEW(XnVMultiItem2D, nXItems, nYItems);
	m_hHysteresisItemSelect = m_pHysteresis->RegisterItemSelect(this, Hysteresis_ItemSelected);

	m_pSlider = NULL;
	m_bValueChangeWithoutBorders = false;

	m_bPositioned = false;
	m_fSliderSizeX = fSliderSizeX;
	m_fSliderSizeY = fSliderSizeY;

	m_pOffAxisHysteresis = XN_NEW(XnVMultiItem1D, 4);
	m_hOffAxisHysteresisItemSelect = m_pOffAxisHysteresis->RegisterItemSelect(this, OffAxisHysteresis_ItemSelected);
	m_pSecondarySlider = NULL;
	m_bInOffAxis = false;
} // XnVSelectableSlider2D::XnVSelectableSlider2D

XnVSelectableSlider2D::~XnVSelectableSlider2D()
{
	m_pHysteresis->UnregisterItemSelect(m_hHysteresisItemSelect);
	XN_DELETE(m_pHysteresis);
	if (m_pSlider != NULL)
	{
		m_pSlider->UnregisterValueChange(m_hSliderValueChange);
		m_pSlider->UnregisterOffAxisMovement(m_hSliderOffAxis);
		XN_DELETE(m_pSlider);
	}

	m_pOffAxisHysteresis->UnregisterItemSelect(m_hOffAxisHysteresisItemSelect);
	XN_DELETE(m_pOffAxisHysteresis);
	if (m_pSecondarySlider != NULL)
	{
		m_pSecondarySlider->UnregisterValueChange(m_hSecondarySliderValueChange);
		m_pSecondarySlider->UnregisterOffAxisMovement(m_hSecondarySliderOffAxis);
		XN_DELETE(m_pSecondarySlider);
	}

	XN_DELETE(m_pItemHoverCBs);
	XN_DELETE(m_pValueChangeCBs);
	XN_DELETE(m_pItemSelectCBs);
	XN_DELETE(m_pOffAxisMovementCBs);

} // XnVSelectableSlider2D::~XnVSelectableSlider2D

void XnVSelectableSlider2D::Reposition(const XnPoint3D& ptCenter)
{
	XnAutoCSLocker local(m_hListenerCS);
	if (m_bPositioned)
	{
		if (ptCenter.X == m_ptCenter.X &&
			ptCenter.Y == m_ptCenter.Y)
		{
			// No changes needed
			return;
		}
		XN_DELETE(m_pSlider);
	}

	m_ptCenter = ptCenter;

	XnPoint3D ptSliderMin;
	XnPoint3D ptSliderMax;

	ptSliderMin.Y = m_ptCenter.Y - m_fSliderSizeY / 2;
	ptSliderMax.Y = m_ptCenter.Y + m_fSliderSizeY / 2;
	ptSliderMin.X = m_ptCenter.X - m_fSliderSizeX / 2;
	ptSliderMax.X = m_ptCenter.X + m_fSliderSizeX / 2;

	m_pSlider = XN_NEW(XnVSlider2D, ptSliderMin, ptSliderMax);
	m_hSliderValueChange = m_pSlider->RegisterValueChange(this, Slider_ValueChange);
	m_hSliderOffAxis = m_pSlider->RegisterOffAxisMovement(this, Slider_OffAxis);

	m_bPositioned = true;
} // XnVSelectableSlider2D::Reposition

void XnVSelectableSlider2D::GetCenter(XnPoint3D& ptCenter) const
{
	ptCenter = m_ptCenter;
}

void XnVSelectableSlider2D::GetSliderSize(XnFloat& fSliderXSize, XnFloat& fSliderYSize) const
{
	fSliderXSize = m_fSliderSizeX;
	fSliderYSize = m_fSliderSizeY;
} // XnVSelectableSlider2D::GetSliderSize

void XnVSelectableSlider2D::SetSliderSize(XnFloat fSliderXSize, XnFloat fSliderYSize)
{
	XnAutoCSLocker local(m_hListenerCS);
	if (m_fSliderSizeX == fSliderXSize &&
		m_fSliderSizeY == fSliderYSize)
	{
		return;
	}
	XN_DELETE(m_pSlider);

	m_fSliderSizeY = fSliderYSize;
	m_fSliderSizeX = fSliderXSize;

	XnPoint3D ptSliderMin;
	XnPoint3D ptSliderMax;

	ptSliderMin.Y = m_ptCenter.Y - m_fSliderSizeY / 2;
	ptSliderMax.Y = m_ptCenter.Y + m_fSliderSizeY / 2;
	ptSliderMin.X = m_ptCenter.X - m_fSliderSizeX / 2;
	ptSliderMax.X = m_ptCenter.X + m_fSliderSizeX / 2;

	m_pSlider = XN_NEW(XnVSlider2D, ptSliderMin, ptSliderMax);
	m_hSliderValueChange = m_pSlider->RegisterValueChange(this, Slider_ValueChange);
	m_hSliderOffAxis = m_pSlider->RegisterOffAxisMovement(this, Slider_OffAxis);
} // XnVSelectableSlider2D::SetSliderSize

XnUInt32 XnVSelectableSlider2D::GetItemXCount() const
{
	return m_pHysteresis->GetItemXCount();
}
XnUInt32 XnVSelectableSlider2D::GetItemYCount() const
{
	return m_pHysteresis->GetItemYCount();
}
void XnVSelectableSlider2D::SetItemCount(XnUInt32 nItemXCount, XnUInt32 nItemYCount)
{
	XnAutoCSLocker local(m_hListenerCS);
	SetItemXCount(nItemXCount);
	SetItemYCount(nItemYCount);
} // XnVSelectableSlider2D::SetItemCount

void XnVSelectableSlider2D::SetItemXCount(XnUInt32 nItemXCount)
{
	XnAutoCSLocker local(m_hListenerCS);
	if (nItemXCount != m_nXItems)
	{
		m_nXItems = nItemXCount;
		m_pHysteresis->SetItemXCount(nItemXCount);
	}
} // XnVSelectableSlider2D::SetItemXCount

void XnVSelectableSlider2D::SetItemYCount(XnUInt32 nItemYCount)
{
	XnAutoCSLocker local(m_hListenerCS);
	if (nItemYCount != m_nYItems)
	{
		m_nYItems = nItemYCount;
		m_pHysteresis->SetItemYCount(nItemYCount);
	}
} // XnVSelectableSlider2D::SetItemYCount

void XnVSelectableSlider2D::OnPrimaryPointCreate(const XnVHandPointContext* cxt, const XnPoint3D& ptFocus)
{
	m_nXLastHover = -1;
	m_nYLastHover = -1;

	m_bInOffAxis = false;

	if (!m_bPositioned)
	{
		Reposition(ptFocus);
	}

	PointMoved(cxt->ptPosition, cxt->fTime);
} // XnVSelectableSlider2D::OnPrimaryPointCreate

void XnVSelectableSlider2D::OnPrimaryPointUpdate(const XnVHandPointContext* cxt)
{
	PointMoved(cxt->ptPosition, cxt->fTime);
} // XnVSelectableSlider2D::OnPrimaryPointUpdate

void XnVSelectableSlider2D::OnPrimaryPointDestroy(XnUInt32 nID)
{
	PointDeleted(true);
} // XnVSelectableSlider2D::OnPrimaryPointDestroy

XnCallbackHandle XnVSelectableSlider2D::RegisterItemHover(void* cxt, ItemHoverCB Hover)
{
	XnCallbackHandle handle;
	m_pItemHoverCBs->Register(Hover, cxt, &handle);
	return handle;
} // XnVSelectableSlider2D::RegisterItemHover

XnCallbackHandle XnVSelectableSlider2D::RegisterValueChange(void* cxt, ValueChangeCB ValueChange)
{
	XnCallbackHandle handle;
	m_pValueChangeCBs->Register(ValueChange, cxt, &handle);
	return handle;
} // XnVSelectableSlider2D::RegisterValueChange

XnCallbackHandle XnVSelectableSlider2D::RegisterItemSelect(void* cxt, ItemSelectCB pCB)
{
	XnCallbackHandle handle;
	m_pItemSelectCBs->Register(pCB, cxt, &handle);
	return handle;
} // XnVSelectableSlider2D::RegisterItemSelect

XnCallbackHandle XnVSelectableSlider2D::RegisterOffAxisMovement(void* cxt, OffAxisMovementCB pCB)
{
	XnCallbackHandle handle;
	m_pOffAxisMovementCBs->Register(pCB, cxt, &handle);
	return handle;
} // XnVSelectableSlider2D::RegisterOffAxisMovement

void XnVSelectableSlider2D::UnregisterItemHover(XnCallbackHandle handle)
{
	m_pItemHoverCBs->Unregister(handle);
} // XnVSelectableSlider2D::UnregisterItemHover

void XnVSelectableSlider2D::UnregisterValueChange(XnCallbackHandle handle)
{
	m_pValueChangeCBs->Unregister(handle);
} // XnVSelectableSlider2D::UnregisterValueChange

void XnVSelectableSlider2D::UnregisterItemSelect(XnCallbackHandle handle)
{
	m_pItemSelectCBs->Unregister(handle);
} // XnVSelectableSlider2D::UnregisterItemSelect

void XnVSelectableSlider2D::UnregisterOffAxisMovement(XnCallbackHandle handle)
{
	m_pOffAxisMovementCBs->Unregister(handle);
} // XnVSelectableSlider2D::UnregisterOffAxisMovement

void XnVSelectableSlider2D::ItemHover(XnInt32 nXIndex, XnInt32 nYIndex)
{
	if (nXIndex != m_nXLastHover || nYIndex != m_nYLastHover)
	{
		m_pItemHoverCBs->Raise(nXIndex, nYIndex);
	}
	m_nXLastHover = nXIndex;
	m_nYLastHover = nYIndex;
} // XnVSelectableSlider2D::ItemHover

void XnVSelectableSlider2D::ValueChange(XnFloat fXValue, XnFloat fYValue)
{
	m_pValueChangeCBs->Raise(fXValue, fYValue);
} // XnVSelectableSlider2D::ValueChange

void XnVSelectableSlider2D::ItemSelect(XnVDirection eDir)
{
	if (m_nXLastHover != -1 && m_nYLastHover != -1)
	{
		m_pItemSelectCBs->Raise(m_nXLastHover, m_nYLastHover, eDir);
	}
} // XnVSelectableSlider2D::ItemSelect

void XnVSelectableSlider2D::OffAxisMovement(XnVDirection eDir)
{
	m_pOffAxisMovementCBs->Raise(eDir);
} // XnVSelectableSlider2D::OffAxisMovement

void XnVSelectableSlider2D::PointDeleted(XnBool bReason)
{
	m_pHysteresis->LostPoint();
} // XnVSelectableSlider2D::PointDeleted

void XnVSelectableSlider2D::PointMoved(const XnPoint3D& ptPos, XnFloat fTime)
{
	if (!m_bInOffAxis && m_pSlider != NULL)
	{
		m_pSlider->Update(ptPos, fTime);
	}
	else if (m_bInOffAxis && m_pSecondarySlider != NULL)
	{
		if (m_bValueChangeOnOffAxis)
		{
			m_pSlider->Update(ptPos, fTime, false);
		}
		m_pSecondarySlider->Update(ptPos, fTime);
	}
} // XnVSelectableSlider2D::PointMoved

void XnVSelectableSlider2D::SetValueChangeOnOffAxis(XnBool bReport)
{
	XnAutoCSLocker local(m_hListenerCS);
	m_bValueChangeOnOffAxis = bReport;
}
XnBool XnVSelectableSlider2D::GetValueChangeOnOffAxis() const
{
	return m_bValueChangeOnOffAxis;
}
