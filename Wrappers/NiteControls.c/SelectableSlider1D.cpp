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
#include "XnVSelectableSlider1D.h"

XN_DECLARE_DEFAULT_HASH(XnVObjectID, XnVSelectableSlider1D*, SelectableSlider1DsHash);
SelectableSlider1DsHash g_SelectableSlider1Ds;

XnVObjectID xnvSelectableSlider1D_Create(XnInt32 nCount,
													   XnVAxis eAxis, XnBool bRecenter,
													   XnFloat fPrimarySliderSize, XnFloat fFirstValue,
													   const XnChar* strName)
{
	XnVSelectableSlider1D* pSS1 = XN_NEW(XnVSelectableSlider1D, nCount, eAxis, bRecenter, fPrimarySliderSize, fFirstValue, strName);
	XnVObjectID id = pSS1;//AllocateID();
	g_SelectableSlider1Ds.Set(id, pSS1);
	xnvPointControl_AddToHash(id, pSS1);

	return id;
}

XnStatus xnvSelectableSlider1D_Destroy(XnVObjectID id)
{
	xnvPointControl_RemoveFromHash(id);

	XnVSelectableSlider1D* pSS1 = NULL;
	XnStatus rc = g_SelectableSlider1Ds.Get(id, pSS1);
	if (rc != XN_STATUS_OK)
	{
		return XN_STATUS_NITE_UNEXPECTED_TYPE;
	}
	g_SelectableSlider1Ds.Remove(id);

	XN_DELETE(pSS1);
	return XN_STATUS_OK;
}
XnStatus xnvSelectableSlider1D_RegisterItemHover(XnVObjectID id, ItemHover1DCB pCB,
												 void* pCookie, XnCallbackHandle* pHandle)
{
	XN_VALIDATE_OUTPUT_PTR(pHandle);
	XnVSelectableSlider1D* pSS1 = NULL;
	XnStatus rc = g_SelectableSlider1Ds.Get(id, pSS1);
	if (rc != XN_STATUS_OK)
	{
		return XN_STATUS_NITE_UNEXPECTED_TYPE;
	}
	*pHandle = pSS1->RegisterItemHover(pCookie, pCB);
	return XN_STATUS_OK;
}
XnStatus xnvSelectableSlider1D_RegisterItemSelect(XnVObjectID id, ItemSelect1DCB pCB,
												  void* pCookie, XnCallbackHandle* pHandle)
{
	XN_VALIDATE_OUTPUT_PTR(pHandle);
	XnVSelectableSlider1D* pSS1 = NULL;
	XnStatus rc = g_SelectableSlider1Ds.Get(id, pSS1);
	if (rc != XN_STATUS_OK)
	{
		return XN_STATUS_NITE_UNEXPECTED_TYPE;
	}
	*pHandle = pSS1->RegisterItemSelect(pCookie, pCB);
	return XN_STATUS_OK;
}
XnStatus xnvSelectableSlider1D_RegisterValueChange(XnVObjectID id, ValueChange1DCB pCB,
												   void* pCookie, XnCallbackHandle* pHandle)
{
	XN_VALIDATE_OUTPUT_PTR(pHandle);
	XnVSelectableSlider1D* pSS1 = NULL;
	XnStatus rc = g_SelectableSlider1Ds.Get(id, pSS1);
	if (rc != XN_STATUS_OK)
	{
		return XN_STATUS_NITE_UNEXPECTED_TYPE;
	}
	*pHandle = pSS1->RegisterValueChange(pCookie, pCB);
	return XN_STATUS_OK;
}
XnStatus xnvSelectableSlider1D_RegisterOffAxisMovement(XnVObjectID id, OffAxisMovement1DCB pCB,
													   void* pCookie, XnCallbackHandle* pHandle)
{
	XN_VALIDATE_OUTPUT_PTR(pHandle);
	XnVSelectableSlider1D* pSS1 = NULL;
	XnStatus rc = g_SelectableSlider1Ds.Get(id, pSS1);
	if (rc != XN_STATUS_OK)
	{
		return XN_STATUS_NITE_UNEXPECTED_TYPE;
	}
	*pHandle = pSS1->RegisterOffAxisMovement(pCookie, pCB);
	return XN_STATUS_OK;
}
XnStatus xnvSelectableSlider1D_UnregisterItemHover(XnVObjectID id, XnCallbackHandle handle)
{
	XnVSelectableSlider1D* pSS1 = NULL;
	XnStatus rc = g_SelectableSlider1Ds.Get(id, pSS1);
	if (rc != XN_STATUS_OK)
	{
		return XN_STATUS_NITE_UNEXPECTED_TYPE;
	}
	pSS1->UnregisterItemHover(handle);
	return XN_STATUS_OK;
}
XnStatus xnvSelectableSlider1D_UnregisterItemSelect(XnVObjectID id, XnCallbackHandle handle)
{
	XnVSelectableSlider1D* pSS1 = NULL;
	XnStatus rc = g_SelectableSlider1Ds.Get(id, pSS1);
	if (rc != XN_STATUS_OK)
	{
		return XN_STATUS_NITE_UNEXPECTED_TYPE;
	}
	pSS1->UnregisterItemSelect(handle);
	return XN_STATUS_OK;
}
XnStatus xnvSelectableSlider1D_UnregisterValueChange(XnVObjectID id, XnCallbackHandle handle)
{
	XnVSelectableSlider1D* pSS1 = NULL;
	XnStatus rc = g_SelectableSlider1Ds.Get(id, pSS1);
	if (rc != XN_STATUS_OK)
	{
		return XN_STATUS_NITE_UNEXPECTED_TYPE;
	}
	pSS1->UnregisterValueChange(handle);
	return XN_STATUS_OK;
}
XnStatus xnvSelectableSlider1D_UnregisterOffAxisMovement(XnVObjectID id, XnCallbackHandle handle)
{
	XnVSelectableSlider1D* pSS1 = NULL;
	XnStatus rc = g_SelectableSlider1Ds.Get(id, pSS1);
	if (rc != XN_STATUS_OK)
	{
		return XN_STATUS_NITE_UNEXPECTED_TYPE;
	}
	pSS1->UnregisterOffAxisMovement(handle);
	return XN_STATUS_OK;
}
XnStatus xnvSelectableSlider1D_Reposition(XnVObjectID id, const XnPoint3D* pCenter)
{
	XnVSelectableSlider1D* pSS1 = NULL;
	XnStatus rc = g_SelectableSlider1Ds.Get(id, pSS1);
	if (rc != XN_STATUS_OK)
	{
		return XN_STATUS_NITE_UNEXPECTED_TYPE;
	}
	pSS1->Reposition(*pCenter);
	return XN_STATUS_OK;
}
XnStatus xnvSelectableSlider1D_GetCenter(XnVObjectID id, XnPoint3D* pCenter)
{
	XnVSelectableSlider1D* pSS1 = NULL;
	XnStatus rc = g_SelectableSlider1Ds.Get(id, pSS1);
	if (rc != XN_STATUS_OK)
	{
		return XN_STATUS_NITE_UNEXPECTED_TYPE;
	}
	pSS1->GetCenter(*pCenter);
	return XN_STATUS_OK;
}
XnStatus xnvSelectableSlider1D_GetItemCount(XnVObjectID id, XnUInt32* pItems)
{
	XN_VALIDATE_OUTPUT_PTR(pItems);
	XnVSelectableSlider1D* pSS1 = NULL;
	XnStatus rc = g_SelectableSlider1Ds.Get(id, pSS1);
	if (rc != XN_STATUS_OK)
	{
		return XN_STATUS_NITE_UNEXPECTED_TYPE;
	}
	*pItems = pSS1->GetItemCount();
	return XN_STATUS_OK;
}
XnStatus xnvSelectableSlider1D_SetItemCount(XnVObjectID id, XnUInt32 nItems)
{
	XnVSelectableSlider1D* pSS1 = NULL;
	XnStatus rc = g_SelectableSlider1Ds.Get(id, pSS1);
	if (rc != XN_STATUS_OK)
	{
		return XN_STATUS_NITE_UNEXPECTED_TYPE;
	}
	pSS1->SetItemCount(nItems);
	return XN_STATUS_OK;
}
XnStatus xnvSelectableSlider1D_GetSliderSize(XnVObjectID id, XnFloat* pfSliderSize)
{
	XN_VALIDATE_OUTPUT_PTR(pfSliderSize);
	XnVSelectableSlider1D* pSS1 = NULL;
	XnStatus rc = g_SelectableSlider1Ds.Get(id, pSS1);
	if (rc != XN_STATUS_OK)
	{
		return XN_STATUS_NITE_UNEXPECTED_TYPE;
	}
	*pfSliderSize = pSS1->GetSliderSize();
	return XN_STATUS_OK;
}
XnStatus xnvSelectableSlider1D_SetSliderSize(XnVObjectID id, XnFloat fSliderSize)
{
	XnVSelectableSlider1D* pSS1 = NULL;
	XnStatus rc = g_SelectableSlider1Ds.Get(id, pSS1);
	if (rc != XN_STATUS_OK)
	{
		return XN_STATUS_NITE_UNEXPECTED_TYPE;
	}
	pSS1->SetSliderSize(fSliderSize);
	return XN_STATUS_OK;
}
XnStatus xnvSelectableSlider1D_SetValueChangeOnOffAxis(XnVObjectID id, XnBool bReport)
{
	XnVSelectableSlider1D* pSS1 = NULL;
	XnStatus rc = g_SelectableSlider1Ds.Get(id, pSS1);
	if (rc != XN_STATUS_OK)
	{
		return XN_STATUS_NITE_UNEXPECTED_TYPE;
	}
	pSS1->SetValueChangeOnOffAxis(bReport);
	return XN_STATUS_OK;
}
XnStatus xnvSelectableSlider1D_GetValueChangeOnOffAxis(XnVObjectID id, XnBool* pbReport)
{
	XN_VALIDATE_OUTPUT_PTR(pbReport);
	XnVSelectableSlider1D* pSS1 = NULL;
	XnStatus rc = g_SelectableSlider1Ds.Get(id, pSS1);
	if (rc != XN_STATUS_OK)
	{
		return XN_STATUS_NITE_UNEXPECTED_TYPE;
	}
	*pbReport = pSS1->GetValueChangeOnOffAxis();
	return XN_STATUS_OK;
}
