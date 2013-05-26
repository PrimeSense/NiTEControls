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
#include "ObjectAllocation.h"
#include "XnHash.h"
#include "PointControl.h"
#include "XnVSelectableSlider2D.h"

XN_DECLARE_DEFAULT_HASH(XnVObjectID, XnVSelectableSlider2D*, SelectableSlider2DsHash);
SelectableSlider2DsHash g_SelectableSlider2Ds;

XnVObjectID xnvSelectableSlider2D_Create(XnInt32 nXItems, XnInt32 nYItems,
													   XnFloat fSliderSizeX, XnFloat fSliderSizeY,
													   const XnChar* strName)
{
	XnVSelectableSlider2D* pSS2 = XN_NEW(XnVSelectableSlider2D, nXItems, nYItems, fSliderSizeX, fSliderSizeY, strName);
	XnVObjectID id = pSS2;//AllocateID();
	g_SelectableSlider2Ds.Set(id, pSS2);
	xnvPointControl_AddToHash(id, pSS2);

	return id;
}

XnStatus xnvSelectableSlider2D_Destroy(XnVObjectID id)
{
	xnvPointControl_RemoveFromHash(id);

	XnVSelectableSlider2D* pSS2 = NULL;
	XnStatus rc = g_SelectableSlider2Ds.Get(id, pSS2);
	if (rc != XN_STATUS_OK)
	{
		return XN_STATUS_NITE_UNEXPECTED_TYPE;
	}
	g_SelectableSlider2Ds.Remove(id);

	XN_DELETE(pSS2);
	return XN_STATUS_OK;
}
XnStatus xnvSelectableSlider2D_RegisterItemHover(XnVObjectID id,
																	   ItemHover2DCB CB,
																	   void* pCookie, XnCallbackHandle* pHandle)
{
	XN_VALIDATE_OUTPUT_PTR(pHandle);
	XnVSelectableSlider2D* pSS2 = NULL;
	XnStatus rc = g_SelectableSlider2Ds.Get(id, pSS2);
	if (rc != XN_STATUS_OK)
	{
		return XN_STATUS_NITE_UNEXPECTED_TYPE;
	}
	*pHandle = pSS2->RegisterItemHover(pCookie, CB);
	return XN_STATUS_OK;
}
XnStatus xnvSelectableSlider2D_RegisterValueChange(XnVObjectID id,
																		 ValueChange2DCB CB,
																		 void* pCookie, XnCallbackHandle* pHandle)
{
	XN_VALIDATE_OUTPUT_PTR(pHandle);
	XnVSelectableSlider2D* pSS2 = NULL;
	XnStatus rc = g_SelectableSlider2Ds.Get(id, pSS2);
	if (rc != XN_STATUS_OK)
	{
		return XN_STATUS_NITE_UNEXPECTED_TYPE;
	}
	*pHandle = pSS2->RegisterValueChange(pCookie, CB);
	return XN_STATUS_OK;
}
XnStatus xnvSelectableSlider2D_RegisterItemSelect(XnVObjectID id,
																		ItemSelect2DCB CB,
																		void* pCookie, XnCallbackHandle* pHandle)
{
	XN_VALIDATE_OUTPUT_PTR(pHandle);
	XnVSelectableSlider2D* pSS2 = NULL;
	XnStatus rc = g_SelectableSlider2Ds.Get(id, pSS2);
	if (rc != XN_STATUS_OK)
	{
		return XN_STATUS_NITE_UNEXPECTED_TYPE;
	}
	*pHandle = pSS2->RegisterItemSelect(pCookie, CB);
	return XN_STATUS_OK;
}
XnStatus xnvSelectableSlider2D_RegisterOffAxisMovement(XnVObjectID id,
																			 OffAxisMovement2DCB CB,
																			 void* pCookie, XnCallbackHandle* pHandle)
{
	XN_VALIDATE_OUTPUT_PTR(pHandle);
	XnVSelectableSlider2D* pSS2 = NULL;
	XnStatus rc = g_SelectableSlider2Ds.Get(id, pSS2);
	if (rc != XN_STATUS_OK)
	{
		return XN_STATUS_NITE_UNEXPECTED_TYPE;
	}
	*pHandle = pSS2->RegisterOffAxisMovement(pCookie, CB);
	return XN_STATUS_OK;
}
XnStatus xnvSelectableSlider2D_UnregisterItemHover(XnVObjectID id, XnCallbackHandle hCB)
{
	XnVSelectableSlider2D* pSS2 = NULL;
	XnStatus rc = g_SelectableSlider2Ds.Get(id, pSS2);
	if (rc != XN_STATUS_OK)
	{
		return XN_STATUS_NITE_UNEXPECTED_TYPE;
	}
	pSS2->UnregisterItemHover(hCB);
	return XN_STATUS_OK;
}
XnStatus xnvSelectableSlider2D_UnregisterValueChange(XnVObjectID id, XnCallbackHandle hCB)
{
	XnVSelectableSlider2D* pSS2 = NULL;
	XnStatus rc = g_SelectableSlider2Ds.Get(id, pSS2);
	if (rc != XN_STATUS_OK)
	{
		return XN_STATUS_NITE_UNEXPECTED_TYPE;
	}
	pSS2->UnregisterValueChange(hCB);
	return XN_STATUS_OK;
}
XnStatus xnvSelectableSlider2D_UnregisterItemSelect(XnVObjectID id, XnCallbackHandle hCB)
{
	XnVSelectableSlider2D* pSS2 = NULL;
	XnStatus rc = g_SelectableSlider2Ds.Get(id, pSS2);
	if (rc != XN_STATUS_OK)
	{
		return XN_STATUS_NITE_UNEXPECTED_TYPE;
	}
	pSS2->UnregisterItemSelect(hCB);
	return XN_STATUS_OK;
}
XnStatus xnvSelectableSlider2D_UnregisterOffAxisMovement(XnVObjectID id,
																   XnCallbackHandle hCB)
{
	XnVSelectableSlider2D* pSS2 = NULL;
	XnStatus rc = g_SelectableSlider2Ds.Get(id, pSS2);
	if (rc != XN_STATUS_OK)
	{
		return XN_STATUS_NITE_UNEXPECTED_TYPE;
	}
	pSS2->UnregisterOffAxisMovement(hCB);
	return XN_STATUS_OK;
}
XnStatus xnvSelectableSlider2D_Reposition(XnVObjectID id, const XnPoint3D* pCenter)
{
	XnVSelectableSlider2D* pSS2 = NULL;
	XnStatus rc = g_SelectableSlider2Ds.Get(id, pSS2);
	if (rc != XN_STATUS_OK)
	{
		return XN_STATUS_NITE_UNEXPECTED_TYPE;
	}
	pSS2->Reposition(*pCenter);
	return XN_STATUS_OK;
}
XnStatus xnvSelectableSlider2D_GetCenter(XnVObjectID id, XnPoint3D* pCenter)
{
	XnVSelectableSlider2D* pSS2 = NULL;
	XnStatus rc = g_SelectableSlider2Ds.Get(id, pSS2);
	if (rc != XN_STATUS_OK)
	{
		return XN_STATUS_NITE_UNEXPECTED_TYPE;
	}
	pSS2->GetCenter(*pCenter);
	return XN_STATUS_OK;
}
XnStatus xnvSelectableSlider2D_GetSliderSize(XnVObjectID id, XnFloat* pfSliderXSize,
													   XnFloat* pfSliderYSize)
{
	XnVSelectableSlider2D* pSS2 = NULL;
	XnStatus rc = g_SelectableSlider2Ds.Get(id, pSS2);
	if (rc != XN_STATUS_OK)
	{
		return XN_STATUS_NITE_UNEXPECTED_TYPE;
	}
	pSS2->GetSliderSize(*pfSliderXSize, *pfSliderYSize);
	return XN_STATUS_OK;
}
XnStatus xnvSelectableSlider2D_SetSliderSize(XnVObjectID id, XnFloat fSliderXSize,
													   XnFloat fSliderYSize)
{
	XnVSelectableSlider2D* pSS2 = NULL;
	XnStatus rc = g_SelectableSlider2Ds.Get(id, pSS2);
	if (rc != XN_STATUS_OK)
	{
		return XN_STATUS_NITE_UNEXPECTED_TYPE;
	}
	pSS2->SetSliderSize(fSliderXSize, fSliderYSize);
	return XN_STATUS_OK;
}
XnStatus xnvSelectableSlider2D_GetItemXCount(XnVObjectID id, XnUInt32* pnItems)
{
	XN_VALIDATE_OUTPUT_PTR(pnItems);
	XnVSelectableSlider2D* pSS2 = NULL;
	XnStatus rc = g_SelectableSlider2Ds.Get(id, pSS2);
	if (rc != XN_STATUS_OK)
	{
		return XN_STATUS_NITE_UNEXPECTED_TYPE;
	}
	*pnItems = pSS2->GetItemXCount();
	return XN_STATUS_OK;
}
XnStatus xnvSelectableSlider2D_GetItemYCount(XnVObjectID id, XnUInt32* pnItems)
{
	XN_VALIDATE_OUTPUT_PTR(pnItems);
	XnVSelectableSlider2D* pSS2 = NULL;
	XnStatus rc = g_SelectableSlider2Ds.Get(id, pSS2);
	if (rc != XN_STATUS_OK)
	{
		return XN_STATUS_NITE_UNEXPECTED_TYPE;
	}
	*pnItems = pSS2->GetItemYCount();
	return XN_STATUS_OK;
}
XnStatus xnvSelectableSlider2D_SetItemCount(XnVObjectID id, XnUInt32 nItemXCount,
													  XnUInt32 nItemYCount)
{
	XnVSelectableSlider2D* pSS2 = NULL;
	XnStatus rc = g_SelectableSlider2Ds.Get(id, pSS2);
	if (rc != XN_STATUS_OK)
	{
		return XN_STATUS_NITE_UNEXPECTED_TYPE;
	}
	pSS2->SetItemCount(nItemXCount, nItemYCount);
	return XN_STATUS_OK;
}
XnStatus xnvSelectableSlider2D_SetItemXCount(XnVObjectID id, XnUInt32 nItemXCount)
{
	XnVSelectableSlider2D* pSS2 = NULL;
	XnStatus rc = g_SelectableSlider2Ds.Get(id, pSS2);
	if (rc != XN_STATUS_OK)
	{
		return XN_STATUS_NITE_UNEXPECTED_TYPE;
	}
	pSS2->SetItemXCount(nItemXCount);
	return XN_STATUS_OK;
}
XnStatus xnvSelectableSlider2D_SetItemYCount(XnVObjectID id, XnUInt32 nItemYCount)
{
	XnVSelectableSlider2D* pSS2 = NULL;
	XnStatus rc = g_SelectableSlider2Ds.Get(id, pSS2);
	if (rc != XN_STATUS_OK)
	{
		return XN_STATUS_NITE_UNEXPECTED_TYPE;
	}
	pSS2->SetItemYCount(nItemYCount);
	return XN_STATUS_OK;
}
XnStatus xnvSelectableSlider2D_SetValueChangeOnOffAxis(XnVObjectID id, XnBool bReport)
{
	XnVSelectableSlider2D* pSS2 = NULL;
	XnStatus rc = g_SelectableSlider2Ds.Get(id, pSS2);
	if (rc != XN_STATUS_OK)
	{
		return XN_STATUS_NITE_UNEXPECTED_TYPE;
	}
	pSS2->SetValueChangeOnOffAxis(bReport);
	return XN_STATUS_OK;
}
XnStatus xnvSelectableSlider2D_GetValueChangeOnOffAxis(XnVObjectID id, XnBool* pbReport)
{
	XN_VALIDATE_OUTPUT_PTR(pbReport);
	XnVSelectableSlider2D* pSS2 = NULL;
	XnStatus rc = g_SelectableSlider2Ds.Get(id, pSS2);
	if (rc != XN_STATUS_OK)
	{
		return XN_STATUS_NITE_UNEXPECTED_TYPE;
	}
	*pbReport = pSS2->GetValueChangeOnOffAxis();
	return XN_STATUS_OK;
}
