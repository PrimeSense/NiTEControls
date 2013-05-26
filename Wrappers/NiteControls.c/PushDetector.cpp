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
#include "XnVPushDetector.h"

XN_DECLARE_DEFAULT_HASH(XnVObjectID, XnVPushDetector*, PushDetectorsHash);
PushDetectorsHash g_PushDetectors;


XnVObjectID xnvPushDetector_Create(const XnChar* strName)
{
	XnVPushDetector* pDetector = XN_NEW(XnVPushDetector, strName);
	XnVObjectID id = pDetector;//AllocateID();
	g_PushDetectors.Set(id, pDetector);
	xnvPointControl_AddToHash(id, pDetector);

	return id;
}
XnStatus xnvPushDetector_Destroy(XnVObjectID id)
{
	xnvPointControl_RemoveFromHash(id);

	XnVPushDetector* pDetector = NULL;
	XnStatus rc = g_PushDetectors.Get(id, pDetector);
	if (rc != XN_STATUS_OK)
	{
		return XN_STATUS_NITE_UNEXPECTED_TYPE;
	}
	g_PushDetectors.Remove(id);

	XN_DELETE(pDetector);
	return XN_STATUS_OK;
}

XnStatus xnvPushDetector_RegisterPush(XnVObjectID id, PushCB pCB, void* pCookie, XnCallbackHandle* pHandle)
{
	XN_VALIDATE_OUTPUT_PTR(pHandle);
	XnVPushDetector* pDetector = NULL;
	XnStatus rc = g_PushDetectors.Get(id, pDetector);
	if (rc != XN_STATUS_OK)
	{
		return XN_STATUS_NITE_UNEXPECTED_TYPE;
	}
	*pHandle = pDetector->RegisterPush(pCookie, pCB);
	return XN_STATUS_OK;
}
XnStatus xnvPushDetector_RegisterStabilized(XnVObjectID id, StabilizedCB pCB, void* pCookie, XnCallbackHandle* pHandle)
{
	XN_VALIDATE_OUTPUT_PTR(pHandle);
	XnVPushDetector* pDetector = NULL;
	XnStatus rc = g_PushDetectors.Get(id, pDetector);
	if (rc != XN_STATUS_OK)
	{
		return XN_STATUS_NITE_UNEXPECTED_TYPE;
	}
	*pHandle = pDetector->RegisterStabilized(pCookie, pCB);
	return XN_STATUS_OK;
}
XnStatus xnvPushDetector_UnregisterPush(XnVObjectID id, XnCallbackHandle handle)
{
	XnVPushDetector* pDetector = NULL;
	XnStatus rc = g_PushDetectors.Get(id, pDetector);
	if (rc != XN_STATUS_OK)
	{
		return XN_STATUS_NITE_UNEXPECTED_TYPE;
	}
	pDetector->UnregisterPush(handle);
	return XN_STATUS_OK;
}
XnStatus xnvPushDetector_UnregisterStabilized(XnVObjectID id, XnCallbackHandle handle)
{
	XnVPushDetector* pDetector = NULL;
	XnStatus rc = g_PushDetectors.Get(id, pDetector);
	if (rc != XN_STATUS_OK)
	{
		return XN_STATUS_NITE_UNEXPECTED_TYPE;
	}
	pDetector->UnregisterStabilized(handle);
	return XN_STATUS_OK;
}
XnStatus xnvPushDetector_Reset(XnVObjectID id)
{
	XnVPushDetector* pDetector = NULL;
	XnStatus rc = g_PushDetectors.Get(id, pDetector);
	if (rc != XN_STATUS_OK)
	{
		return XN_STATUS_NITE_UNEXPECTED_TYPE;
	}
	pDetector->Reset();
	return XN_STATUS_OK;
}
XnStatus xnvPushDetector_GetPushImmediateMinimumVelocity(XnVObjectID id, XnFloat* pfVelocity)
{
	XN_VALIDATE_OUTPUT_PTR(pfVelocity);
	XnVPushDetector* pDetector = NULL;
	XnStatus rc = g_PushDetectors.Get(id, pDetector);
	if (rc != XN_STATUS_OK)
	{
		return XN_STATUS_NITE_UNEXPECTED_TYPE;
	}
	*pfVelocity = pDetector->GetPushImmediateMinimumVelocity();
	return XN_STATUS_OK;
}
XnStatus xnvPushDetector_GetPushImmediateDuration(XnVObjectID id, XnUInt32* pnDuration)
{
	XN_VALIDATE_OUTPUT_PTR(pnDuration);
	XnVPushDetector* pDetector = NULL;
	XnStatus rc = g_PushDetectors.Get(id, pDetector);
	if (rc != XN_STATUS_OK)
	{
		return XN_STATUS_NITE_UNEXPECTED_TYPE;
	}
	*pnDuration = pDetector->GetPushImmediateDuration();
	return XN_STATUS_OK;
}
XnStatus xnvPushDetector_GetPushImmediateOffset(XnVObjectID id, XnUInt32* pnOffset)
{
	XN_VALIDATE_OUTPUT_PTR(pnOffset);
	XnVPushDetector* pDetector = NULL;
	XnStatus rc = g_PushDetectors.Get(id, pDetector);
	if (rc != XN_STATUS_OK)
	{
		return XN_STATUS_NITE_UNEXPECTED_TYPE;
	}
	*pnOffset = pDetector->GetPushImmediateOffset();
	return XN_STATUS_OK;
}
XnStatus xnvPushDetector_GetPushPreviousMinimumVelocity(XnVObjectID id, XnFloat* pfVelocity)
{
	XN_VALIDATE_OUTPUT_PTR(pfVelocity);
	XnVPushDetector* pDetector = NULL;
	XnStatus rc = g_PushDetectors.Get(id, pDetector);
	if (rc != XN_STATUS_OK)
	{
		return XN_STATUS_NITE_UNEXPECTED_TYPE;
	}
	*pfVelocity = pDetector->GetPushPreviousMinimumVelocity();
	return XN_STATUS_OK;
}
XnStatus xnvPushDetector_GetPushPreviousDuration(XnVObjectID id, XnUInt32* pnDuration)
{
	XN_VALIDATE_OUTPUT_PTR(pnDuration);
	XnVPushDetector* pDetector = NULL;
	XnStatus rc = g_PushDetectors.Get(id, pDetector);
	if (rc != XN_STATUS_OK)
	{
		return XN_STATUS_NITE_UNEXPECTED_TYPE;
	}
	*pnDuration	= pDetector->GetPushPreviousDuration();
	return XN_STATUS_OK;
}
XnStatus xnvPushDetector_GetPushPreviousOffset(XnVObjectID id, XnUInt32* pnDuration)
{
	XN_VALIDATE_OUTPUT_PTR(pnDuration);
	XnVPushDetector* pDetector = NULL;
	XnStatus rc = g_PushDetectors.Get(id, pDetector);
	if (rc != XN_STATUS_OK)
	{
		return XN_STATUS_NITE_UNEXPECTED_TYPE;
	}
	*pnDuration = pDetector->GetPushPreviousOffset();
	return XN_STATUS_OK;
}
XnStatus xnvPushDetector_GetPushMaximumAngleBetweenImmediateAndZ(XnVObjectID id, XnFloat* pfAngle)
{
	XN_VALIDATE_OUTPUT_PTR(pfAngle);
	XnVPushDetector* pDetector = NULL;
	XnStatus rc = g_PushDetectors.Get(id, pDetector);
	if (rc != XN_STATUS_OK)
	{
		return XN_STATUS_NITE_UNEXPECTED_TYPE;
	}
	*pfAngle = pDetector->GetPushMaximumAngleBetweenImmediateAndZ();
	return XN_STATUS_OK;
}
XnStatus xnvPushDetector_GetPushMinimumAngleBetweenImmediateAndPrevious(XnVObjectID id, XnFloat* pfAngle)
{
	XN_VALIDATE_OUTPUT_PTR(pfAngle);
	XnVPushDetector* pDetector = NULL;
	XnStatus rc = g_PushDetectors.Get(id, pDetector);
	if (rc != XN_STATUS_OK)
	{
		return XN_STATUS_NITE_UNEXPECTED_TYPE;
	}
	*pfAngle = pDetector->GetPushMinimumAngleBetweenImmediateAndPrevious();
	return XN_STATUS_OK;
}
XnStatus xnvPushDetector_GetStableMaximumVelocity(XnVObjectID id, XnFloat* pfVelocity)
{
	XN_VALIDATE_OUTPUT_PTR(pfVelocity);
	XnVPushDetector* pDetector = NULL;
	XnStatus rc = g_PushDetectors.Get(id, pDetector);
	if (rc != XN_STATUS_OK)
	{
		return XN_STATUS_NITE_UNEXPECTED_TYPE;
	}
	*pfVelocity = pDetector->GetStableMaximumVelocity();
	return XN_STATUS_OK;
}
XnStatus xnvPushDetector_GetStableDuration(XnVObjectID id, XnUInt32* pnDuration)
{
	XN_VALIDATE_OUTPUT_PTR(pnDuration);
	XnVPushDetector* pDetector = NULL;
	XnStatus rc = g_PushDetectors.Get(id, pDetector);
	if (rc != XN_STATUS_OK)
	{
		return XN_STATUS_NITE_UNEXPECTED_TYPE;
	}
	*pnDuration = pDetector->GetStableDuration();
	return XN_STATUS_OK;
}
XnStatus xnvPushDetector_SetPushImmediateMinimumVelocity(XnVObjectID id, XnFloat fVelocity)
{
	XnVPushDetector* pDetector = NULL;
	XnStatus rc = g_PushDetectors.Get(id, pDetector);
	if (rc != XN_STATUS_OK)
	{
		return XN_STATUS_NITE_UNEXPECTED_TYPE;
	}
	pDetector->SetPushImmediateMinimumVelocity(fVelocity);
	return XN_STATUS_OK;
}
XnStatus xnvPushDetector_SetPushImmediateDuration(XnVObjectID id, XnUInt32 nDuration)
{
	XnVPushDetector* pDetector = NULL;
	XnStatus rc = g_PushDetectors.Get(id, pDetector);
	if (rc != XN_STATUS_OK)
	{
		return XN_STATUS_NITE_UNEXPECTED_TYPE;
	}
	pDetector->SetPushImmediateDuration(nDuration);
	return XN_STATUS_OK;
}
XnStatus xnvPushDetector_SetPushImmediateOffset(XnVObjectID id, XnUInt32 nOffset)
{
	XnVPushDetector* pDetector = NULL;
	XnStatus rc = g_PushDetectors.Get(id, pDetector);
	if (rc != XN_STATUS_OK)
	{
		return XN_STATUS_NITE_UNEXPECTED_TYPE;
	}
	pDetector->SetPushImmediateOffset(nOffset);
	return XN_STATUS_OK;
}
XnStatus xnvPushDetector_SetPushPreviousMinimumVelocity(XnVObjectID id, XnFloat fVelocity)
{
	XnVPushDetector* pDetector = NULL;
	XnStatus rc = g_PushDetectors.Get(id, pDetector);
	if (rc != XN_STATUS_OK)
	{
		return XN_STATUS_NITE_UNEXPECTED_TYPE;
	}
	pDetector->SetPushPreviousMinimumVelocity(fVelocity);
	return XN_STATUS_OK;
}
XnStatus xnvPushDetector_SetPushPreviousDuration(XnVObjectID id, XnUInt32 nDuration)
{
	XnVPushDetector* pDetector = NULL;
	XnStatus rc = g_PushDetectors.Get(id, pDetector);
	if (rc != XN_STATUS_OK)
	{
		return XN_STATUS_NITE_UNEXPECTED_TYPE;
	}
	pDetector->SetPushPreviousDuration(nDuration);
	return XN_STATUS_OK;
}
XnStatus xnvPushDetector_SetPushPreviousOffset(XnVObjectID id, XnUInt32 nOffset)
{
	XnVPushDetector* pDetector = NULL;
	XnStatus rc = g_PushDetectors.Get(id, pDetector);
	if (rc != XN_STATUS_OK)
	{
		return XN_STATUS_NITE_UNEXPECTED_TYPE;
	}
	pDetector->SetPushPreviousOffset(nOffset);
	return XN_STATUS_OK;
}
XnStatus xnvPushDetector_SetPushMaximumAngleBetweenImmediateAndZ(XnVObjectID id, XnFloat fAngle)
{
	XnVPushDetector* pDetector = NULL;
	XnStatus rc = g_PushDetectors.Get(id, pDetector);
	if (rc != XN_STATUS_OK)
	{
		return XN_STATUS_NITE_UNEXPECTED_TYPE;
	}
	pDetector->SetPushMaximumAngleBetweenImmediateAndZ(fAngle);
	return XN_STATUS_OK;
}
XnStatus xnvPushDetector_SetPushMinimumAngleBetweenImmediateAndPrevious(XnVObjectID id, XnFloat fAngle)
{
	XnVPushDetector* pDetector = NULL;
	XnStatus rc = g_PushDetectors.Get(id, pDetector);
	if (rc != XN_STATUS_OK)
	{
		return XN_STATUS_NITE_UNEXPECTED_TYPE;
	}
	pDetector->SetPushMinimumAngleBetweenImmediateAndPrevious(fAngle);
	return XN_STATUS_OK;
}
XnStatus xnvPushDetector_SetStableMaximumVelocity(XnVObjectID id, XnFloat fVelocity)
{
	XnVPushDetector* pDetector = NULL;
	XnStatus rc = g_PushDetectors.Get(id, pDetector);
	if (rc != XN_STATUS_OK)
	{
		return XN_STATUS_NITE_UNEXPECTED_TYPE;
	}
	pDetector->SetStableMaximumVelocity(fVelocity);
	return XN_STATUS_OK;
}
XnStatus xnvPushDetector_SetStableDuration(XnVObjectID id, XnUInt32 nDuration)
{
	XnVPushDetector* pDetector = NULL;
	XnStatus rc = g_PushDetectors.Get(id, pDetector);
	if (rc != XN_STATUS_OK)
	{
		return XN_STATUS_NITE_UNEXPECTED_TYPE;
	}
	pDetector->SetStableDuration(nDuration);
	return XN_STATUS_OK;
}
