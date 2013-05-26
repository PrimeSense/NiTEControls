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
#include "XnHash.h"
#include "ObjectAllocation.h"
#include "PointControl.h"
#include "MessageListener.h"
#include "XnVPointControl.h"

XN_DECLARE_DEFAULT_HASH(XnVObjectID, XnVPointControl*, PointControlsHash);
PointControlsHash g_PointControls;

void xnvPointControl_AddToHash(XnVObjectID id, XnVPointControl* pControl)
{
	g_PointControls.Set(id, pControl);
	xnvMessageListener_AddToHash(id, pControl);
}
XnVPointControl* xnvPointControl_RemoveFromHash(XnVObjectID id)
{
	xnvMessageListener_RemoveFromHash(id);

	XnVPointControl* pControl = NULL;
	XnStatus rc = g_PointControls.Get(id, pControl);
	if (rc != XN_STATUS_OK)
	{
		return NULL;
	}
	g_PointControls.Remove(id);
	return pControl;
}

XnVObjectID xnvPointControl_Create(const XnChar* strName)
{
	XnVPointControl* pControl = XN_NEW(XnVPointControl, strName);
	XnVObjectID id = pControl;//AllocateID();

	xnvPointControl_AddToHash(id, pControl);

	return id;
}
XnStatus xnvPointControl_Destroy(XnVObjectID id)
{
	XnVPointControl* pControl = xnvPointControl_RemoveFromHash(id);
	if (pControl == NULL)
	{
		return XN_STATUS_NITE_UNEXPECTED_TYPE;
	}
	else
	{
		XN_DELETE(pControl);
	}
	return XN_STATUS_OK;
}

XnStatus xnvPointControl_RegisterPointCreate(XnVObjectID id, PointCreateCB CB, void* pCookie,
													 XnCallbackHandle* pHandle)
{
	XN_VALIDATE_OUTPUT_PTR(pHandle);
	XnVPointControl* pControl = NULL;
	XnStatus rc = g_PointControls.Get(id, pControl);
	if (rc != XN_STATUS_OK)
	{
		return XN_STATUS_NITE_UNEXPECTED_TYPE;
	}
	*pHandle = pControl->RegisterPointCreate(pCookie, CB);
	return XN_STATUS_OK;
}
XnStatus xnvPointControl_RegisterPointUpdate(XnVObjectID id, PointUpdateCB CB,
											 void* pCookie, XnCallbackHandle* pHandle)
{
	XN_VALIDATE_OUTPUT_PTR(pHandle);
	XnVPointControl* pControl = NULL;
	XnStatus rc = g_PointControls.Get(id, pControl);
	if (rc != XN_STATUS_OK)
	{
		return XN_STATUS_NITE_UNEXPECTED_TYPE;
	}
	*pHandle = pControl->RegisterPointUpdate(pCookie, CB);
	return XN_STATUS_OK;
}
XnStatus xnvPointControl_RegisterPointDestroy(XnVObjectID id, PointDestroyCB CB,
											  void* pCookie, XnCallbackHandle* pHandle)
{
	XN_VALIDATE_OUTPUT_PTR(pHandle);
	XnVPointControl* pControl = NULL;
	XnStatus rc = g_PointControls.Get(id, pControl);
	if (rc != XN_STATUS_OK)
	{
		return XN_STATUS_NITE_UNEXPECTED_TYPE;
	}
	*pHandle = pControl->RegisterPointDestroy(pCookie, CB);
	return XN_STATUS_OK;
}
struct XnVExternalPrimaryPointCreate
{
	PrimaryPointCreateCB externalCB;
	void* pCookie;
};
XN_DECLARE_DEFAULT_HASH(XnCallbackHandle, XnVExternalPrimaryPointCreate*, ExternalPrimaryPointCreateCBs);
ExternalPrimaryPointCreateCBs g_ExternalPrimaryPointCreateCBs;
void XN_CALLBACK_TYPE InternalPrimaryPointCreateCB(const XnVHandPointContext* pContext,
												   const XnPoint3D& ptFocus, void* pCookie)
{
	XnVExternalPrimaryPointCreate* pExternalNoCB = (XnVExternalPrimaryPointCreate*)pCookie;
	if (pExternalNoCB->externalCB != NULL)
	{
		(*pExternalNoCB->externalCB)(pContext, &ptFocus, pExternalNoCB->pCookie);
	}
}

XnStatus xnvPointControl_RegisterPrimaryPointCreate(XnVObjectID id,
													PrimaryPointCreateCB CB,
													void* pCookie, XnCallbackHandle* pHandle)
{
	XN_VALIDATE_OUTPUT_PTR(pHandle);
	XnVPointControl* pControl = NULL;
	XnStatus rc = g_PointControls.Get(id, pControl);
	if (rc != XN_STATUS_OK)
	{
		return XN_STATUS_NITE_UNEXPECTED_TYPE;
	}
	XnVExternalPrimaryPointCreate* pNewExternal = XN_NEW(XnVExternalPrimaryPointCreate);
	pNewExternal->externalCB = CB;
	pNewExternal->pCookie = pCookie;
	*pHandle = pControl->RegisterPrimaryPointCreate(pNewExternal, InternalPrimaryPointCreateCB);
	g_ExternalPrimaryPointCreateCBs.Set(*pHandle, pNewExternal);

	return XN_STATUS_OK;
}
XnStatus xnvPointControl_RegisterPrimaryPointUpdate(XnVObjectID id,
																		  PrimaryPointUpdateCB CB,
																		  void* pCookie, XnCallbackHandle* pHandle)
{
	XN_VALIDATE_OUTPUT_PTR(pHandle);
	XnVPointControl* pControl = NULL;
	XnStatus rc = g_PointControls.Get(id, pControl);
	if (rc != XN_STATUS_OK)
	{
		return XN_STATUS_NITE_UNEXPECTED_TYPE;
	}
	*pHandle = pControl->RegisterPrimaryPointUpdate(pCookie, CB);
	return XN_STATUS_OK;
}
XnStatus xnvPointControl_RegisterPrimaryPointReplace(XnVObjectID id,
																		   PrimaryPointReplaceCB CB,
																		   void* pCookie, XnCallbackHandle* pHandle)
{
	XN_VALIDATE_OUTPUT_PTR(pHandle);
	XnVPointControl* pControl = NULL;
	XnStatus rc = g_PointControls.Get(id, pControl);
	if (rc != XN_STATUS_OK)
	{
		return XN_STATUS_NITE_UNEXPECTED_TYPE;
	}
	*pHandle = pControl->RegisterPrimaryPointReplace(pCookie, CB);
	return XN_STATUS_OK;
}
XnStatus xnvPointControl_RegisterPrimaryPointDestroy(XnVObjectID id,
																		   PrimaryPointDestroyCB CB,
																		   void* pCookie, XnCallbackHandle* pHandle)
{
	XN_VALIDATE_OUTPUT_PTR(pHandle);
	XnVPointControl* pControl = NULL;
	XnStatus rc = g_PointControls.Get(id, pControl);
	if (rc != XN_STATUS_OK)
	{
		return XN_STATUS_NITE_UNEXPECTED_TYPE;
	}
	*pHandle = pControl->RegisterPrimaryPointDestroy(pCookie, CB);
	return XN_STATUS_OK;
}
XnStatus xnvPointControl_RegisterNoPoints(XnVObjectID id, NoPointsCB CB,
																void* pCookie, XnCallbackHandle* pHandle)
{
	XN_VALIDATE_OUTPUT_PTR(pHandle);
	XnVPointControl* pControl = NULL;
	XnStatus rc = g_PointControls.Get(id, pControl);
	if (rc != XN_STATUS_OK)
	{
		return XN_STATUS_NITE_UNEXPECTED_TYPE;
	}
	*pHandle = pControl->RegisterNoPoints(pCookie, CB);
	return XN_STATUS_OK;
}
XnStatus xnvPointControl_UnregisterPointCreate(XnVObjectID id, XnCallbackHandle hCB)
{
	XnVPointControl* pControl = NULL;
	XnStatus rc = g_PointControls.Get(id, pControl);
	if (rc != XN_STATUS_OK)
	{
		return XN_STATUS_NITE_UNEXPECTED_TYPE;
	}
	pControl->UnregisterPointCreate(hCB);
	return XN_STATUS_OK;
}
XnStatus xnvPointControl_UnregisterPointUpdate(XnVObjectID id, XnCallbackHandle hCB)
{
	XnVPointControl* pControl = NULL;
	XnStatus rc = g_PointControls.Get(id, pControl);
	if (rc != XN_STATUS_OK)
	{
		return XN_STATUS_NITE_UNEXPECTED_TYPE;
	}
	pControl->UnregisterPointUpdate(hCB);
	return XN_STATUS_OK;
}
XnStatus xnvPointControl_UnregisterPointDestroy(XnVObjectID id, XnCallbackHandle hCB)
{
	XnVPointControl* pControl = NULL;
	XnStatus rc = g_PointControls.Get(id, pControl);
	if (rc != XN_STATUS_OK)
	{
		return XN_STATUS_NITE_UNEXPECTED_TYPE;
	}
	pControl->UnregisterPointCreate(hCB);
	return XN_STATUS_OK;
}
XnStatus xnvPointControl_UnregisterPrimaryPointCreate(XnVObjectID id, XnCallbackHandle hCB)
{
	XnVPointControl* pControl = NULL;
	XnStatus rc = g_PointControls.Get(id, pControl);
	if (rc != XN_STATUS_OK)
	{
		return XN_STATUS_NITE_UNEXPECTED_TYPE;
	}
	XnVExternalPrimaryPointCreate* pExternal = NULL;
	rc = g_ExternalPrimaryPointCreateCBs.Remove(hCB, pExternal);
	if (rc != XN_STATUS_OK)
	{
		return rc;
	}
	XN_DELETE(pExternal);

	pControl->UnregisterPointCreate(hCB);
	return XN_STATUS_OK;
}
XnStatus xnvPointControl_UnregisterPrimaryPointUpdate(XnVObjectID id, XnCallbackHandle hCB)
{
	XnVPointControl* pControl = NULL;
	XnStatus rc = g_PointControls.Get(id, pControl);
	if (rc != XN_STATUS_OK)
	{
		return XN_STATUS_NITE_UNEXPECTED_TYPE;
	}
	pControl->UnregisterPointCreate(hCB);
	return XN_STATUS_OK;
}
XnStatus xnvPointControl_UnregisterPrimaryPointReplace(XnVObjectID id, XnCallbackHandle hCB)
{
	XnVPointControl* pControl = NULL;
	XnStatus rc = g_PointControls.Get(id, pControl);
	if (rc != XN_STATUS_OK)
	{
		return XN_STATUS_NITE_UNEXPECTED_TYPE;
	}
	pControl->UnregisterPointCreate(hCB);
	return XN_STATUS_OK;
}
XnStatus xnvPointControl_UnregisterPrimaryPointDestroy(XnVObjectID id, XnCallbackHandle hCB)
{
	XnVPointControl* pControl = NULL;
	XnStatus rc = g_PointControls.Get(id, pControl);
	if (rc != XN_STATUS_OK)
	{
		return XN_STATUS_NITE_UNEXPECTED_TYPE;
	}
	pControl->UnregisterPointCreate(hCB);
	return XN_STATUS_OK;
}
XnStatus xnvPointControl_UnregisterNoPoints(XnVObjectID id, XnCallbackHandle hCB)
{
	XnVPointControl* pControl = NULL;
	XnStatus rc = g_PointControls.Get(id, pControl);
	if (rc != XN_STATUS_OK)
	{
		return XN_STATUS_NITE_UNEXPECTED_TYPE;
	}
	pControl->UnregisterPointCreate(hCB);
	return XN_STATUS_OK;
}
XnStatus xnvPointControl_GetPrimaryID(XnVObjectID id, XnUInt32* pPrimary)
{
	XN_VALIDATE_OUTPUT_PTR(pPrimary);
	XnVPointControl* pControl = NULL;
	XnStatus rc = g_PointControls.Get(id, pControl);
	if (rc != XN_STATUS_OK)
	{
		return XN_STATUS_NITE_UNEXPECTED_TYPE;
	}
	*pPrimary = pControl->GetPrimaryID();
	return XN_STATUS_OK;
}
