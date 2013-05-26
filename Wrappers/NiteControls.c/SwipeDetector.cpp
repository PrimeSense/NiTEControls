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
#include "XnVSwipeDetector.h"

XN_DECLARE_DEFAULT_HASH(XnVObjectID, XnVSwipeDetector*, SwipeDetectorsHash);
SwipeDetectorsHash g_SwipeDetectors;

XnVObjectID xnvSwipeDetector_Create(XnBool bUseSteadyBeforeSwipe, const XnChar* strName)
{
	XnVSwipeDetector* pDetector = XN_NEW(XnVSwipeDetector, bUseSteadyBeforeSwipe, strName);
	XnVObjectID id = pDetector;//AllocateID();
	g_SwipeDetectors.Set(id, pDetector);
	xnvPointControl_AddToHash(id, pDetector);

	return id;
}
XnStatus xnvSwipeDetector_Destroy(XnVObjectID id)
{
	xnvPointControl_RemoveFromHash(id);

	XnVSwipeDetector* pDetector = NULL;
	XnStatus rc = g_SwipeDetectors.Get(id, pDetector);
	if (rc != XN_STATUS_OK)
	{
		return XN_STATUS_NITE_UNEXPECTED_TYPE;
	}
	g_SwipeDetectors.Remove(id);

	XN_DELETE(pDetector);
	return XN_STATUS_OK;
}
XnStatus xnvSwipeDetector_Reset(XnVObjectID id)
{
	XnVSwipeDetector* pDetector = NULL;
	XnStatus rc = g_SwipeDetectors.Get(id, pDetector);
	if (rc != XN_STATUS_OK)
	{
		return XN_STATUS_NITE_UNEXPECTED_TYPE;
	}
	pDetector->Reset();
	return XN_STATUS_OK;
}
XnStatus xnvSwipeDetector_RegisterSwipeUp(XnVObjectID id, SwipeCB pCB,
																void* pCookie, XnCallbackHandle* pHandle)
{
	XN_VALIDATE_OUTPUT_PTR(pHandle);
	XnVSwipeDetector* pDetector = NULL;
	XnStatus rc = g_SwipeDetectors.Get(id, pDetector);
	if (rc != XN_STATUS_OK)
	{
		return XN_STATUS_NITE_UNEXPECTED_TYPE;
	}
	*pHandle = pDetector->RegisterSwipeUp(pCookie, pCB);
	return XN_STATUS_OK;
}
XnStatus xnvSwipeDetector_RegisterSwipeDown(XnVObjectID id, SwipeCB pCB,
																  void* pCookie, XnCallbackHandle* pHandle)
{
	XN_VALIDATE_OUTPUT_PTR(pHandle);
	XnVSwipeDetector* pDetector = NULL;
	XnStatus rc = g_SwipeDetectors.Get(id, pDetector);
	if (rc != XN_STATUS_OK)
	{
		return XN_STATUS_NITE_UNEXPECTED_TYPE;
	}
	*pHandle = pDetector->RegisterSwipeDown(pCookie, pCB);
	return XN_STATUS_OK;
}
XnStatus xnvSwipeDetector_RegisterSwipeLeft(XnVObjectID id, SwipeCB pCB,
																  void* pCookie, XnCallbackHandle* pHandle)
{
	XN_VALIDATE_OUTPUT_PTR(pHandle);
	XnVSwipeDetector* pDetector = NULL;
	XnStatus rc = g_SwipeDetectors.Get(id, pDetector);
	if (rc != XN_STATUS_OK)
	{
		return XN_STATUS_NITE_UNEXPECTED_TYPE;
	}
	*pHandle = pDetector->RegisterSwipeLeft(pCookie, pCB);
	return XN_STATUS_OK;
}
XnStatus xnvSwipeDetector_RegisterSwipeRight(XnVObjectID id, SwipeCB pCB,
																   void* pCookie, XnCallbackHandle* pHandle)
{
	XN_VALIDATE_OUTPUT_PTR(pHandle);
	XnVSwipeDetector* pDetector = NULL;
	XnStatus rc = g_SwipeDetectors.Get(id, pDetector);
	if (rc != XN_STATUS_OK)
	{
		return XN_STATUS_NITE_UNEXPECTED_TYPE;
	}
	*pHandle = pDetector->RegisterSwipeRight(pCookie, pCB);
	return XN_STATUS_OK;
}
XnStatus xnvSwipeDetector_RegisterSwipe(XnVObjectID id, GeneralSwipeCB pCB,
															  void* pCookie, XnCallbackHandle* pHandle)
{
	XN_VALIDATE_OUTPUT_PTR(pHandle);
	XnVSwipeDetector* pDetector = NULL;
	XnStatus rc = g_SwipeDetectors.Get(id, pDetector);
	if (rc != XN_STATUS_OK)
	{
		return XN_STATUS_NITE_UNEXPECTED_TYPE;
	}
	*pHandle = pDetector->RegisterSwipe(pCookie, pCB);
	return XN_STATUS_OK;
}
XnStatus xnvSwipeDetector_UnregisterSwipeUp(XnVObjectID id, XnCallbackHandle handle)
{
	XnVSwipeDetector* pDetector = NULL;
	XnStatus rc = g_SwipeDetectors.Get(id, pDetector);
	if (rc != XN_STATUS_OK)
	{
		return XN_STATUS_NITE_UNEXPECTED_TYPE;
	}
	pDetector->UnregisterSwipeUp(handle);
	return XN_STATUS_OK;
}
XnStatus xnvSwipeDetector_UnregisterSwipeDown(XnVObjectID id, XnCallbackHandle handle)
{
	XnVSwipeDetector* pDetector = NULL;
	XnStatus rc = g_SwipeDetectors.Get(id, pDetector);
	if (rc != XN_STATUS_OK)
	{
		return XN_STATUS_NITE_UNEXPECTED_TYPE;
	}
	pDetector->UnregisterSwipeDown(handle);
	return XN_STATUS_OK;
}
XnStatus xnvSwipeDetector_UnregisterSwipeLeft(XnVObjectID id, XnCallbackHandle handle)
{
	XnVSwipeDetector* pDetector = NULL;
	XnStatus rc = g_SwipeDetectors.Get(id, pDetector);
	if (rc != XN_STATUS_OK)
	{
		return XN_STATUS_NITE_UNEXPECTED_TYPE;
	}
	pDetector->UnregisterSwipeLeft(handle);
	return XN_STATUS_OK;
}
XnStatus xnvSwipeDetector_UnregisterSwipeRight(XnVObjectID id, XnCallbackHandle handle)
{
	XnVSwipeDetector* pDetector = NULL;
	XnStatus rc = g_SwipeDetectors.Get(id, pDetector);
	if (rc != XN_STATUS_OK)
	{
		return XN_STATUS_NITE_UNEXPECTED_TYPE;
	}
	pDetector->UnregisterSwipeRight(handle);
	return XN_STATUS_OK;
}
XnStatus xnvSwipeDetector_UnregisterSwipe(XnVObjectID id, XnCallbackHandle handle)
{
	XnVSwipeDetector* pDetector = NULL;
	XnStatus rc = g_SwipeDetectors.Get(id, pDetector);
	if (rc != XN_STATUS_OK)
	{
		return XN_STATUS_NITE_UNEXPECTED_TYPE;
	}
	pDetector->UnregisterSwipe(handle);
	return XN_STATUS_OK;
}
XnStatus xnvSwipeDetector_SetMotionSpeedThreshold(XnVObjectID id, XnFloat fThreshold)
{
	XnVSwipeDetector* pDetector = NULL;
	XnStatus rc = g_SwipeDetectors.Get(id, pDetector);
	if (rc != XN_STATUS_OK)
	{
		return XN_STATUS_NITE_UNEXPECTED_TYPE;
	}
	pDetector->SetMotionSpeedThreshold(fThreshold);
	return XN_STATUS_OK;
}
XnStatus xnvSwipeDetector_GetMotionSpeedThreshold(XnVObjectID id, XnFloat* pfSpeed)
{
	XN_VALIDATE_OUTPUT_PTR(pfSpeed);
	XnVSwipeDetector* pDetector = NULL;
	XnStatus rc = g_SwipeDetectors.Get(id, pDetector);
	if (rc != XN_STATUS_OK)
	{
		return XN_STATUS_NITE_UNEXPECTED_TYPE;
	}
	*pfSpeed = pDetector->GetMotionSpeedThreshold();
	return XN_STATUS_OK;
}
XnStatus xnvSwipeDetector_SetMotionTime(XnVObjectID id, XnUInt32 nThreshold)
{
	XnVSwipeDetector* pDetector = NULL;
	XnStatus rc = g_SwipeDetectors.Get(id, pDetector);
	if (rc != XN_STATUS_OK)
	{
		return XN_STATUS_NITE_UNEXPECTED_TYPE;
	}
	pDetector->SetMotionTime(nThreshold);
	return XN_STATUS_OK;
}
XnStatus xnvSwipeDetector_GetMotionTime(XnVObjectID id, XnUInt32* pnTime)
{
	XN_VALIDATE_OUTPUT_PTR(pnTime);
	XnVSwipeDetector* pDetector = NULL;
	XnStatus rc = g_SwipeDetectors.Get(id, pDetector);
	if (rc != XN_STATUS_OK)
	{
		return XN_STATUS_NITE_UNEXPECTED_TYPE;
	}
	*pnTime = pDetector->GetMotionTime();
	return XN_STATUS_OK;
}
XnStatus xnvSwipeDetector_SetXAngleThreshold(XnVObjectID id, XnFloat fThreshold)
{
	XnVSwipeDetector* pDetector = NULL;
	XnStatus rc = g_SwipeDetectors.Get(id, pDetector);
	if (rc != XN_STATUS_OK)
	{
		return XN_STATUS_NITE_UNEXPECTED_TYPE;
	}
	pDetector->SetXAngleThreshold(fThreshold);
	return XN_STATUS_OK;
}
XnStatus xnvSwipeDetector_GetXAngleThreshold(XnVObjectID id, XnFloat* pfAngle)
{
	XN_VALIDATE_OUTPUT_PTR(pfAngle);
	XnVSwipeDetector* pDetector = NULL;
	XnStatus rc = g_SwipeDetectors.Get(id, pDetector);
	if (rc != XN_STATUS_OK)
	{
		return XN_STATUS_NITE_UNEXPECTED_TYPE;
	}
	*pfAngle = pDetector->GetXAngleThreshold();
	return XN_STATUS_OK;
}
XnStatus xnvSwipeDetector_SetYAngleThreshold(XnVObjectID id, XnFloat fThreshold)
{
	XnVSwipeDetector* pDetector = NULL;
	XnStatus rc = g_SwipeDetectors.Get(id, pDetector);
	if (rc != XN_STATUS_OK)
	{
		return XN_STATUS_NITE_UNEXPECTED_TYPE;
	}
	pDetector->SetYAngleThreshold(fThreshold);
	return XN_STATUS_OK;
}
XnStatus xnvSwipeDetector_GetYAngleThreshold(XnVObjectID id, XnFloat* pfAngle)
{
	XN_VALIDATE_OUTPUT_PTR(pfAngle);
	XnVSwipeDetector* pDetector = NULL;
	XnStatus rc = g_SwipeDetectors.Get(id, pDetector);
	if (rc != XN_STATUS_OK)
	{
		return XN_STATUS_NITE_UNEXPECTED_TYPE;
	}
	*pfAngle = pDetector->GetYAngleThreshold();
	return XN_STATUS_OK;
}
XnStatus xnvSwipeDetector_SetSteadyMaxStdDev(XnVObjectID id, XnFloat fStdDev)
{
	XnVSwipeDetector* pDetector = NULL;
	XnStatus rc = g_SwipeDetectors.Get(id, pDetector);
	if (rc != XN_STATUS_OK)
	{
		return XN_STATUS_NITE_UNEXPECTED_TYPE;
	}
	pDetector->SetSteadyMaxStdDev(fStdDev);
	return XN_STATUS_OK;
}
XnStatus xnvSwipeDetector_GetSteadyMaxStdDev(XnVObjectID id, XnFloat* pfStdDev)
{
	XN_VALIDATE_OUTPUT_PTR(pfStdDev);
	XnVSwipeDetector* pDetector = NULL;
	XnStatus rc = g_SwipeDetectors.Get(id, pDetector);
	if (rc != XN_STATUS_OK)
	{
		return XN_STATUS_NITE_UNEXPECTED_TYPE;
	}
	*pfStdDev = pDetector->GetSteadyMaxStdDev();
	return XN_STATUS_OK;
}
XnStatus xnvSwipeDetector_SetSteadyDuration(XnVObjectID id, XnUInt32 nDuration)
{
	XnVSwipeDetector* pDetector = NULL;
	XnStatus rc = g_SwipeDetectors.Get(id, pDetector);
	if (rc != XN_STATUS_OK)
	{
		return XN_STATUS_NITE_UNEXPECTED_TYPE;
	}
	pDetector->SetSteadyDuration(nDuration);
	return XN_STATUS_OK;
}
XnStatus xnvSwipeDetector_GetSteadyDuration(XnVObjectID id, XnUInt32* pnDuration)
{
	XN_VALIDATE_OUTPUT_PTR(pnDuration);
	XnVSwipeDetector* pDetector = NULL;
	XnStatus rc = g_SwipeDetectors.Get(id, pDetector);
	if (rc != XN_STATUS_OK)
	{
		return XN_STATUS_NITE_UNEXPECTED_TYPE;
	}
	*pnDuration = pDetector->GetSteadyDuration();
	return XN_STATUS_OK;
}
XnStatus xnvSwipeDetector_SetUseSteady(XnVObjectID id, XnBool bUse)
{
	XnVSwipeDetector* pDetector = NULL;
	XnStatus rc = g_SwipeDetectors.Get(id, pDetector);
	if (rc != XN_STATUS_OK)
	{
		return XN_STATUS_NITE_UNEXPECTED_TYPE;
	}
	pDetector->SetUseSteady(bUse);
	return XN_STATUS_OK;
}
XnStatus xnvSwipeDetector_GetUseSteady(XnVObjectID id, XnBool* pbUse)
{
	XN_VALIDATE_OUTPUT_PTR(pbUse);
	XnVSwipeDetector* pDetector = NULL;
	XnStatus rc = g_SwipeDetectors.Get(id, pDetector);
	if (rc != XN_STATUS_OK)
	{
		return XN_STATUS_NITE_UNEXPECTED_TYPE;
	}
	*pbUse = pDetector->GetUseSteady();
	return XN_STATUS_OK;
}
