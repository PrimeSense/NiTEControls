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
#include "XnVSteadyDetector.h"

XN_DECLARE_DEFAULT_HASH(XnVObjectID, XnVSteadyDetector*, SteadyDetectorsHash);
SteadyDetectorsHash g_SteadyDetectors;

XnVObjectID xnvSteadyDetector_Create(XnUInt32 nCooldownFrames,
												   XnUInt32 nDetectionDuration,
												   XnFloat fMaximumAllowedVelocity,
												   const XnChar* strName)
{
	XnVSteadyDetector* pDetector = XN_NEW(XnVSteadyDetector, nCooldownFrames, nDetectionDuration, fMaximumAllowedVelocity, strName);
	XnVObjectID id = pDetector;//AllocateID();
	g_SteadyDetectors.Set(id, pDetector);
	xnvPointControl_AddToHash(id, pDetector);

	return id;
}
XnStatus xnvSteadyDetector_Destroy(XnVObjectID id)
{
	xnvPointControl_RemoveFromHash(id);

	XnVSteadyDetector* pDetector = NULL;
	XnStatus rc = g_SteadyDetectors.Get(id, pDetector);
	if (rc != XN_STATUS_OK)
	{
		return XN_STATUS_NITE_UNEXPECTED_TYPE;
	}
	g_SteadyDetectors.Remove(id);

	XN_DELETE(pDetector);
	return XN_STATUS_OK;
}
XnStatus xnvSteadyDetector_RegisterSteady(XnVObjectID id, SteadyCB CB,
																void* pCookie, XnCallbackHandle* pHandle)
{
	XN_VALIDATE_OUTPUT_PTR(pHandle);
	XnVSteadyDetector* pDetector = NULL;
	XnStatus rc = g_SteadyDetectors.Get(id, pDetector);
	if (rc != XN_STATUS_OK)
	{
		return XN_STATUS_NITE_UNEXPECTED_TYPE;
	}
	*pHandle = pDetector->RegisterSteady(pCookie, CB);
	return XN_STATUS_OK;
}
XnStatus xnvSteadyDetector_UnregisterSteady(XnVObjectID id, XnCallbackHandle hCB)
{
	XnVSteadyDetector* pDetector = NULL;
	XnStatus rc = g_SteadyDetectors.Get(id, pDetector);
	if (rc != XN_STATUS_OK)
	{
		return XN_STATUS_NITE_UNEXPECTED_TYPE;
	}
	pDetector->UnregisterSteady(hCB);
	return XN_STATUS_OK;
}
XnStatus xnvSteadyDetector_RegisterNotSteady(XnVObjectID id, NotSteadyCB CB,
										  void* pCookie, XnCallbackHandle* pHandle)
{
	XN_VALIDATE_OUTPUT_PTR(pHandle);
	XnVSteadyDetector* pDetector = NULL;
	XnStatus rc = g_SteadyDetectors.Get(id, pDetector);
	if (rc != XN_STATUS_OK)
	{
		return XN_STATUS_NITE_UNEXPECTED_TYPE;
	}
	*pHandle = pDetector->RegisterNotSteady(pCookie, CB);
	return XN_STATUS_OK;
}
XnStatus xnvSteadyDetector_UnregisterNotSteady(XnVObjectID id, XnCallbackHandle hCB)
{
	XnVSteadyDetector* pDetector = NULL;
	XnStatus rc = g_SteadyDetectors.Get(id, pDetector);
	if (rc != XN_STATUS_OK)
	{
		return XN_STATUS_NITE_UNEXPECTED_TYPE;
	}
	pDetector->UnregisterNotSteady(hCB);
	return XN_STATUS_OK;
}
XnStatus xnvSteadyDetector_Reset(XnVObjectID id)
{
	XnVSteadyDetector* pDetector = NULL;
	XnStatus rc = g_SteadyDetectors.Get(id, pDetector);
	if (rc != XN_STATUS_OK)
	{
		return XN_STATUS_NITE_UNEXPECTED_TYPE;
	}
	pDetector->Reset();
	return XN_STATUS_OK;
}
XnStatus xnvSteadyDetector_GetDetectionDuration(XnVObjectID id,  XnUInt32* pnDuration)
{
	XN_VALIDATE_OUTPUT_PTR(pnDuration);
	XnVSteadyDetector* pDetector = NULL;
	XnStatus rc = g_SteadyDetectors.Get(id, pDetector);
	if (rc != XN_STATUS_OK)
	{
		return XN_STATUS_NITE_UNEXPECTED_TYPE;
	}
	*pnDuration = pDetector->GetDetectionDuration();
	return XN_STATUS_OK;
}
XnStatus xnvSteadyDetector_SetDetectionDuration(XnVObjectID id, XnUInt32 nDuration)
{
	XnVSteadyDetector* pDetector = NULL;
	XnStatus rc = g_SteadyDetectors.Get(id, pDetector);
	if (rc != XN_STATUS_OK)
	{
		return XN_STATUS_NITE_UNEXPECTED_TYPE;
	}
	pDetector->SetDetectionDuration(nDuration);
	return XN_STATUS_OK;
}
XnStatus xnvSteadyDetector_GetMaximumStdDevForSteady(XnVObjectID id, XnFloat* pfStdDev)
{
	XN_VALIDATE_OUTPUT_PTR(pfStdDev);
	XnVSteadyDetector* pDetector = NULL;
	XnStatus rc = g_SteadyDetectors.Get(id, pDetector);
	if (rc != XN_STATUS_OK)
	{
		return XN_STATUS_NITE_UNEXPECTED_TYPE;
	}
	*pfStdDev = pDetector->GetMaximumStdDevForSteady();
	return XN_STATUS_OK;
}
XnStatus xnvSteadyDetector_SetMaximumStdDevForSteady(XnVObjectID id, XnFloat fStdDev)
{
	XnVSteadyDetector* pDetector = NULL;
	XnStatus rc = g_SteadyDetectors.Get(id, pDetector);
	if (rc != XN_STATUS_OK)
	{
		return XN_STATUS_NITE_UNEXPECTED_TYPE;
	}
	pDetector->SetMaximumStdDevForSteady(fStdDev);
	return XN_STATUS_OK;
}
XnStatus xnvSteadyDetector_GetMinimumStdDevForNotSteady(XnVObjectID id, XnFloat* pfStdDev)
{
	XN_VALIDATE_OUTPUT_PTR(pfStdDev);
	XnVSteadyDetector* pDetector = NULL;
	XnStatus rc = g_SteadyDetectors.Get(id, pDetector);
	if (rc != XN_STATUS_OK)
	{
		return XN_STATUS_NITE_UNEXPECTED_TYPE;
	}
	*pfStdDev = pDetector->GetMinimumStdDevForNotSteady();
	return XN_STATUS_OK;
}
XnStatus xnvSteadyDetector_SetMinimumStdDevForNotSteady(XnVObjectID id, XnFloat fStdDev)
{
	XnVSteadyDetector* pDetector = NULL;
	XnStatus rc = g_SteadyDetectors.Get(id, pDetector);
	if (rc != XN_STATUS_OK)
	{
		return XN_STATUS_NITE_UNEXPECTED_TYPE;
	}
	pDetector->SetMinimumStdDevForNotSteady(fStdDev);
	return XN_STATUS_OK;
}
