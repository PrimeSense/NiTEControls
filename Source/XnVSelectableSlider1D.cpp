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
#include <XnVSelectableSlider1D.h>
#include "XnVNiteDefs.h"
#include "XnVNiteEvents.h"

XN_DECLARE_EVENT_2ARG(XnVIntDirectionSpecificEvent, XnVIntDirectionEvent, XnInt32, nValue, XnVDirection, eValue);

void XN_CALLBACK_TYPE XnVSelectableSlider1D::PrimarySlider_OffAxisMovement(XnVDirection eDir,
																		   void* pContext)
{
	XnVSelectableSlider1D* pSS = (XnVSelectableSlider1D*)pContext;
	pSS->PrimarySlider_OffAxisMovement(eDir);
} // PrimarySlider_OffAxisMovement

void XnVSelectableSlider1D::PrimarySlider_OffAxisMovement(XnVDirection eDir)
{
	m_fPrimarySliderValueAtChange = m_pSlider->GetValue();
	m_pHysteresis->LostPoint();
	XnVAxis eAxis = XnVDirectionAsAxis(eDir);

	InternalCreateSlider(false, eAxis, m_pSlider->GetPosition(), 0.5);
	OffAxisMovement(eDir);
} // XnVSelectableSlider1D::PrimarySlider_OffAxisMovement

void XN_CALLBACK_TYPE XnVSelectableSlider1D::PrimarySlider_ValueChange(XnFloat fValue, void* pContext)
{
	XnVSelectableSlider1D* pSS = (XnVSelectableSlider1D*)pContext;
	pSS->UpdatePrimarySlider(fValue);
} // PrimarySlider_ValueChange

void XN_CALLBACK_TYPE XnVSelectableSlider1D::SecondarySlider_OffAxisMovement(XnVDirection eDir,
																			 void* pContext)
{
	XnVSelectableSlider1D* pSS = (XnVSelectableSlider1D*)pContext;
	pSS->SecondarySlider_OffAxisMovement(eDir);
} // SecondarySlider_OffAxisMovement

void XN_CALLBACK_TYPE XnVSelectableSlider1D::SecondarySlider_ValueChange(XnFloat fValue, void* pContext)
{
	XnVSelectableSlider1D* pSS = (XnVSelectableSlider1D*)pContext;
	pSS->UpdateSecondarySlider(fValue);
} // SecondarySlider_ValueChange

void XnVSelectableSlider1D::SecondarySlider_OffAxisMovement(XnVDirection eDir)
{
	m_pOffAxisHysteresis->LostPoint();
	InternalCreateSlider(true, m_ePrimaryAxisDirection, m_pSlider->GetPosition(),
		m_fPrimarySliderValueAtChange);
} // XnVSelectableSlider1D::SecondarySlider_OffAxisMovement

void XN_CALLBACK_TYPE XnVSelectableSlider1D::Hysteresis_ItemSelected(XnInt32 nItem, void* pContext)
{
	XnVSelectableSlider1D* pSS = (XnVSelectableSlider1D*)pContext;
	pSS->ItemHover(nItem);
} // Hysteresis_ItemSelected

void XN_CALLBACK_TYPE XnVSelectableSlider1D::OffAxisHysteresis_ItemSelected(XnInt32 nItem, void* pContext)
{
	XnVSelectableSlider1D* pSS = (XnVSelectableSlider1D*)pContext;
	pSS->OffAxisHysteresis_ItemSelected(nItem);
} // OffAxisHysteresis_ItemSelected

void XnVSelectableSlider1D::OffAxisHysteresis_ItemSelected(XnInt32 nItem)
{
	XnVDirection eDirection;
	if (nItem == 0)
	{
		switch (m_pSecondarySlider->GetAxis())
		{
		case AXIS_X:
			eDirection = DIRECTION_LEFT; break;
		case AXIS_Y:
			eDirection = DIRECTION_DOWN; break;
		case AXIS_Z:
			eDirection = DIRECTION_FORWARD; break;
		default:
			eDirection = DIRECTION_ILLEGAL; break;
		}
	}
	else if (nItem == m_pOffAxisHysteresis->GetItemCount()-1)
	{
		switch (m_pSecondarySlider->GetAxis())
		{
		case AXIS_X:
			eDirection = DIRECTION_RIGHT; break;
		case AXIS_Y:
			eDirection = DIRECTION_UP; break;
		case AXIS_Z:
			eDirection = DIRECTION_BACKWARD; break;
		default:
			eDirection = DIRECTION_ILLEGAL; break;
		}
	}
	else
	{
		return;
	}

	ItemSelect(eDirection);
} // XnVSelectableSlider1D::OffAxisHysteresis_ItemSelected

const XnFloat XnVSelectableSlider1D::ms_fDefaultPrimarySliderSize = 250;
const XnFloat XnVSelectableSlider1D::ms_fSecondarySliderSize = 150;

XnFloat XnVSelectableSlider1D::GetFirstValue() const
{
	return m_fFirstValue;
} // XnVSelectableSlider1D::GetFirstValue

void XnVSelectableSlider1D::SetFirstValue(XnFloat fValue)
{
	m_fFirstValue = fValue;
	m_bFirstPointCreated = false;
} // XnVSelectableSlider1D::SetFirstValue

XnUInt32 XnVSelectableSlider1D::GetItemCount() const
{
	return m_nItemCount;
} // XnVSelectableSlider1D::GetItemCount

void XnVSelectableSlider1D::SetItemCount(XnUInt32 nCount)
{
	XnAutoCSLocker local(m_hListenerCS);
	if (nCount != m_nItemCount)
	{
		m_nItemCount = nCount;
		m_pHysteresis->SetItemCount(nCount);
	}
} // XnVSelectableSlider1D::SetItemCount

void XnVSelectableSlider1D::ItemHover(XnInt32 nItemIndex)
{
	if (m_nLastHover != nItemIndex)
	{
		m_pItemHoverCBs->Raise(nItemIndex);
	}
	m_nLastHover = nItemIndex;
} // XnVSelectableSlider1D::ItemHover

void XnVSelectableSlider1D::ItemSelect(XnVDirection nDirection)
{
	if (m_nLastHover != -1)
	{
		m_pItemSelectCBs->Raise(m_nLastHover, nDirection);
	}
} // XnVSelectableSlider1D::ItemSelect

void XnVSelectableSlider1D::ValueChange(XnFloat fValue)
{
	m_pValueChangeCBs->Raise(fValue);
} // XnVSelectableSlider1D::ValueChange

void XnVSelectableSlider1D::OffAxisMovement(XnVDirection eDir)
{
	m_pOffAxisMovementCBs->Raise(eDir);
} // XnVSelectableSlider1D::OffAxisMovement

XnCallbackHandle XnVSelectableSlider1D::RegisterItemHover(void* cxt, ItemHoverCB pCB)
{
	XnCallbackHandle handle;
	m_pItemHoverCBs->Register(pCB, cxt, &handle);
	return handle;
} // XnVSelectableSlider1D::RegisterItemHover

XnCallbackHandle XnVSelectableSlider1D::RegisterItemSelect(void* cxt, ItemSelectCB pCB)
{
	XnCallbackHandle handle;
	m_pItemSelectCBs->Register(pCB, cxt, &handle);
	return handle;
} // XnVSelectableSlider1D::RegisterItemSelect

XnCallbackHandle XnVSelectableSlider1D::RegisterValueChange(void* cxt, ValueChangeCB pCB)
{
	XnCallbackHandle handle;
	m_pValueChangeCBs->Register(pCB, cxt, &handle);
	return handle;
} // XnVSelectableSlider1D::RegisterValueChange

XnCallbackHandle XnVSelectableSlider1D::RegisterOffAxisMovement(void* cxt, OffAxisMovementCB pCB)
{
	XnCallbackHandle handle;
	m_pOffAxisMovementCBs->Register(pCB, cxt, &handle);
	return handle;
} // XnVSelectableSlider1D::RegisterOffAxisMovement

void XnVSelectableSlider1D::UnregisterItemHover(XnCallbackHandle handle)
{
	m_pItemHoverCBs->Unregister(handle);
} // XnVSelectableSlider1D::UnregisterItemHover

void XnVSelectableSlider1D::UnregisterItemSelect(XnCallbackHandle handle)
{
	m_pItemSelectCBs->Unregister(handle);
} // XnVSelectableSlider1D::UnregisterItemSelect

void XnVSelectableSlider1D::UnregisterValueChange(XnCallbackHandle handle)
{
	m_pValueChangeCBs->Unregister(handle);
} // XnVSelectableSlider1D::UnregisterValueChange

void XnVSelectableSlider1D::UnregisterOffAxisMovement(XnCallbackHandle handle)
{
	m_pOffAxisMovementCBs->Unregister(handle);
} // XnVSelectableSlider1D::UnregisterOffAxisMovement

XnVSelectableSlider1D::XnVSelectableSlider1D(XnInt32 nCount, XnVAxis eAxis,
											 XnBool bRecenter,
											 XnFloat fPrimarySliderSize, XnFloat fFirstValue,
											 const XnChar* strName) :
	XnVPointControl(strName),
	m_bValueChangeOnOffAxis(false),
	m_bRecenter(bRecenter)
{
	m_pItemHoverCBs = XN_NEW(XnVIntSpecificEvent);
	m_pItemSelectCBs = XN_NEW(XnVIntDirectionSpecificEvent);
	m_pValueChangeCBs = XN_NEW(XnVFloatSpecificEvent);
	m_pOffAxisMovementCBs = XN_NEW(XnVDirectionSpecificEvent);

	m_fPrimarySliderSize = fPrimarySliderSize;
	m_ePrimaryAxisDirection = eAxis;
	m_nItemCount = nCount;
	m_nLastHover = -1;

	m_pHysteresis = XN_NEW(XnVMultiItem1D, nCount);
	m_pOffAxisHysteresis = XN_NEW(XnVMultiItem1D, 4);
	m_pPrimarySlider = NULL;
	m_pSecondarySlider = NULL;
	m_pSlider = NULL;

	m_bPositioned = false;

	m_hPrimaryHysteresisItemSelect = 
		m_pHysteresis->RegisterItemSelect(this, Hysteresis_ItemSelected);
	m_hSecondaryHysteresisItemSelect =
		m_pOffAxisHysteresis->RegisterItemSelect(this, OffAxisHysteresis_ItemSelected);

	m_fPrimarySliderValueAtChange = 0.5;
	m_fFirstValue = fFirstValue;
	m_bFirstPointCreated = false;
} // XnVSelectableSlider1D::XnVSelectableSlider1D

XnVSelectableSlider1D::~XnVSelectableSlider1D()
{
	m_pHysteresis->UnregisterItemSelect(m_hPrimaryHysteresisItemSelect);
	m_pOffAxisHysteresis->UnregisterItemSelect(m_hSecondaryHysteresisItemSelect);

	if (m_pPrimarySlider != NULL)
	{
		m_pPrimarySlider->UnregisterOffAxisMovement(m_hPrimarySliderOffAxis);
		m_pPrimarySlider->UnregisterValueChange(m_hPrimarySliderValueChange);
		XN_DELETE(m_pPrimarySlider);
	}
	if (m_pSecondarySlider != NULL)
	{
		m_pSecondarySlider->UnregisterOffAxisMovement(m_hSecondarySliderOffAxis);
		m_pSecondarySlider->UnregisterValueChange(m_hSecondarySliderValueChange);
		XN_DELETE(m_pSecondarySlider);
	}

	XN_DELETE(m_pHysteresis);
	XN_DELETE(m_pOffAxisHysteresis);

	XN_DELETE(m_pItemHoverCBs);
	XN_DELETE(m_pItemSelectCBs);
	XN_DELETE(m_pValueChangeCBs);
	XN_DELETE(m_pOffAxisMovementCBs);

} // XnVSelectableSlider1D::~XnVSelectableSlider1D

void XnVSelectableSlider1D::Reposition(const XnPoint3D& ptCenter)
{
	XnAutoCSLocker local(m_hListenerCS);
	m_ptCenter = ptCenter;
	m_bPositioned = true;
	InternalCreateSlider(true, m_ePrimaryAxisDirection, ptCenter, 0.5);
} // XnVSelectableSlider1D::Reposition

void XnVSelectableSlider1D::GetCenter(XnPoint3D& ptCenter) const
{
	ptCenter = m_ptCenter;
}

XnFloat XnVSelectableSlider1D::GetSliderSize() const
{
	return m_fPrimarySliderSize;
}

void XnVSelectableSlider1D::SetSliderSize(XnFloat fSize)
{
	XnAutoCSLocker local(m_hListenerCS);
	m_fPrimarySliderSize = fSize;
	InternalCreateSlider(true, m_ePrimaryAxisDirection, m_ptCenter, 0.5);
} // XnVSelectableSlider1D::SetSliderSize

void XnVSelectableSlider1D::PointCreated(const XnPoint3D& ptPos)
{
	if (!m_bFirstPointCreated)
	{
		InternalCreateSlider(true, m_ePrimaryAxisDirection, ptPos, m_fFirstValue);
		m_bFirstPointCreated = true;
	}
	else
	{
		InternalCreateSlider(true, m_ePrimaryAxisDirection, ptPos, 0.5);
	}
} // XnVSelectableSlider1D::PointCreated

void XnVSelectableSlider1D::PointDeleted(XnBool bReason)
{
	m_pSlider = NULL;
	m_pHysteresis->LostPoint();
	m_pOffAxisHysteresis->LostPoint();
} // XnVSelectableSlider1D::PointDeleted

void XnVSelectableSlider1D::PointMoved(const XnPoint3D& ptPos, XnFloat fTime)
{
	if (m_bValueChangeOnOffAxis && m_pSlider != m_pPrimarySlider)
	{
		m_pPrimarySlider->Update(ptPos, false);
	}
	if (m_pSlider != NULL)
	{
		m_pSlider->Update(ptPos, fTime);
	}
} // XnVSelectableSlider1D::PointMoved

void XnVSelectableSlider1D::InternalCreateSlider(XnBool bPrimaryAxis, XnVAxis eAxis,
											   const XnPoint3D& ptInitialPoint, XnFloat fInitialValue)
{
	XnFloat fSliderSize = XnFloat((bPrimaryAxis) ? m_fPrimarySliderSize : ms_fSecondarySliderSize);

	if (bPrimaryAxis)
	{
		XnPoint3D ptSliderMin = ptInitialPoint;
		XnPoint3D ptSliderMax = ptInitialPoint;

		switch (eAxis)
		{
		case AXIS_X:
			ptSliderMin.X = m_ptCenter.X - fSliderSize / 2;
			ptSliderMax.X = m_ptCenter.X + fSliderSize / 2;
			break;
		case AXIS_Y:
			ptSliderMin.Y = m_ptCenter.Y - fSliderSize / 2;
			ptSliderMax.Y = m_ptCenter.Y + fSliderSize / 2;
			break;
		case AXIS_Z:
			ptSliderMin.Z = m_ptCenter.Z - fSliderSize / 2;
			ptSliderMax.Z = m_ptCenter.Z + fSliderSize / 2;
			break;
		default:
			break;
		}
		if (m_pPrimarySlider == NULL)
		{
			m_pPrimarySlider =
				XN_NEW(XnVSlider1D, eAxis, ptInitialPoint, ptSliderMin, ptSliderMax, 0.0, 1.0);
			m_hPrimarySliderOffAxis =
				m_pPrimarySlider->RegisterOffAxisMovement(this, PrimarySlider_OffAxisMovement);
			m_hPrimarySliderValueChange = 
				m_pPrimarySlider->RegisterValueChange(this, PrimarySlider_ValueChange);
		}
		else
		{
			m_pPrimarySlider->Reinitialize(eAxis, ptInitialPoint, ptSliderMin, ptSliderMax, 0.0, 1.0);
		}
		m_pSlider = m_pPrimarySlider;
	} // if (bPrimaryAxis)
	else
	{
		if (m_pSecondarySlider == NULL)
		{
			m_pSecondarySlider =
				XN_NEW(XnVSlider1D, eAxis, ptInitialPoint, fSliderSize, fInitialValue, 0.0, 1.0);
			m_hSecondarySliderOffAxis =
				m_pSecondarySlider->RegisterOffAxisMovement(this, SecondarySlider_OffAxisMovement);
			m_hSecondarySliderValueChange = 
				m_pSecondarySlider->RegisterValueChange(this, SecondarySlider_ValueChange);
		}
		else
		{
			m_pSecondarySlider->Reinitialize(eAxis, ptInitialPoint, fSliderSize, fInitialValue, 0.0, 1.0);
		}
		m_pSlider = m_pSecondarySlider;
	} // if (bPrimaryAxis) else
} // XnVSelectableSlider1D::InternalCreateSlider

void XnVSelectableSlider1D::UpdatePrimarySlider(XnFloat fVal)
{
	ValueChange(fVal);
	// update our controller
	m_pHysteresis->Update(fVal);
} // XnVSelectableSlider1D::UpdatePrimarySlider

void XnVSelectableSlider1D::UpdateSecondarySlider(XnFloat fVal)
{
	m_pOffAxisHysteresis->Update(fVal);
} // XnVSelectableSlider1D::UpdateSecondarySlider

void XnVSelectableSlider1D::OnPrimaryPointCreate(const XnVHandPointContext* pContext,
												 const XnPoint3D& ptFocusPoint)
{
	if (!m_bPositioned)
	{
		if (m_bRecenter)
		{
			m_ptCenter =  pContext->ptPosition;
		}
		else
		{
			m_ptCenter = ptFocusPoint;
		}
	}
	m_nLastHover = -1;
	PointCreated(pContext->ptPosition);
	PointMoved(pContext->ptPosition, pContext->fTime);
} // XnVSelectableSlider1D::OnPrimaryPointCreate

void XnVSelectableSlider1D::OnPrimaryPointUpdate(const XnVHandPointContext* pContext)
{
	PointMoved(pContext->ptPosition, pContext->fTime);
} // XnVSelectableSlider1D::OnPrimaryPointUpdate

void XnVSelectableSlider1D::OnPrimaryPointDestroy(XnUInt32 nID)
{
	PointDeleted(true);
} // XnVSelectableSlider1D::OnPrimaryPointDestroy

void XnVSelectableSlider1D::SetValueChangeOnOffAxis(XnBool bReport)
{
	XnAutoCSLocker local(m_hListenerCS);
	m_bValueChangeOnOffAxis = bReport;
}
XnBool XnVSelectableSlider1D::GetValueChangeOnOffAxis() const
{
	return m_bValueChangeOnOffAxis;
}
