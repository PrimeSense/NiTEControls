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
#include "PointFilter.h"
#include "SessionGenerator.h"
#include "XnVPointArea.h"
#include "XnVSessionManager.h"

XN_DECLARE_DEFAULT_HASH(XnVObjectID, XnVPointArea*, PointAreasHash);
PointAreasHash g_PointAreas;

XnVObjectID xnvPointArea_CreateByBoundingBox(XnBoundingBox3D* pArea, XnBool bRelative,
	 const XnChar* strName)
{
	XnVPointArea* pPointArea = XN_NEW(XnVPointArea, *pArea, bRelative, strName);
	XnVObjectID id = pPointArea;//AllocateID();
	g_PointAreas.Set(id, pPointArea);
	xnvPointFilter_AddToHash(id, pPointArea);

	return id;
}
XnVObjectID xnvPointArea_CreateByPoints(const XnPoint3D* pMin, const XnPoint3D* pMax, XnBool bRelative,
													  const XnChar* strName)
{
	XnVPointArea* pPointArea = XN_NEW(XnVPointArea, *pMin, *pMax, bRelative, strName);
	XnVObjectID id = pPointArea;//AllocateID();
	g_PointAreas.Set(id, pPointArea);
	xnvPointFilter_AddToHash(id, pPointArea);

	return id;
}

XnStatus xnvPointArea_Destroy(XnVObjectID id)
{
	xnvPointFilter_RemoveFromHash(id);

	XnVPointArea* pArea = NULL;
	XnStatus rc = g_PointAreas.Get(id, pArea);
	if (rc != XN_STATUS_OK)
	{
		return XN_STATUS_NITE_UNEXPECTED_TYPE;
	}
	g_PointAreas.Remove(id);

	XN_DELETE(pArea);
	return XN_STATUS_OK;
}

XnStatus xnvPointArea_ChangeAreaByBoundingBox(XnVObjectID id, XnBoundingBox3D* pArea, XnBool bRelative)
{
	XnVPointArea* pPointArea = NULL;
	XnStatus rc = g_PointAreas.Get(id, pPointArea);
	if (rc != XN_STATUS_OK)
	{
		return XN_STATUS_NITE_UNEXPECTED_TYPE;
	}

	pPointArea->ChangeArea(*pArea, bRelative);
	return XN_STATUS_OK;
}
XnStatus xnvPointArea_ChangeAreaByPoints(XnVObjectID id, const XnPoint3D* pMin, const XnPoint3D* pMax,
												   XnBool bRelative)
{
	XnVPointArea* pPointArea = NULL;
	XnStatus rc = g_PointAreas.Get(id, pPointArea);
	if (rc != XN_STATUS_OK)
	{
		return XN_STATUS_NITE_UNEXPECTED_TYPE;
	}

	pPointArea->ChangeArea(*pMin, *pMax, bRelative);
	return XN_STATUS_OK;
}

XnStatus xnvPointArea_RegisterToPointSilenced(XnVObjectID id, PointSilencedCB CB,
													  void* pCookie, XnCallbackHandle* pHandle)
{
	XN_VALIDATE_OUTPUT_PTR(pHandle);
	XnVPointArea* pPointArea = NULL;
	XnStatus rc = g_PointAreas.Get(id, pPointArea);
	if (rc != XN_STATUS_OK)
	{
		return XN_STATUS_NITE_UNEXPECTED_TYPE;
	}
	*pHandle = pPointArea->RegisterPointSilenced(pCookie, CB);
	return XN_STATUS_OK;
}
XnStatus xnvPointArea_RegisterToPointRevived(XnVObjectID id, PointRevivedCB CB, void* pCookie, XnCallbackHandle* pHandle)
{
	XN_VALIDATE_OUTPUT_PTR(pHandle);
	XnVPointArea* pPointArea = NULL;
	XnStatus rc = g_PointAreas.Get(id, pPointArea);
	if (rc != XN_STATUS_OK)
	{
		return XN_STATUS_NITE_UNEXPECTED_TYPE;
	}

	*pHandle = pPointArea->RegisterPointRevived(pCookie, CB);
	return XN_STATUS_OK;
}
XnStatus xnvPointArea_RegisterToSilentPointRemoved(XnVObjectID id, SilentPointRemovedCB CB,
												   void* pCookie, XnCallbackHandle* pHandle)
{
	XN_VALIDATE_OUTPUT_PTR(pHandle);
	XnVPointArea* pPointArea = NULL;
	XnStatus rc = g_PointAreas.Get(id, pPointArea);
	if (rc != XN_STATUS_OK)
	{
		return XN_STATUS_NITE_UNEXPECTED_TYPE;
	}
	*pHandle = pPointArea->RegisterPointSilenced(pCookie, CB);
	return XN_STATUS_OK;
}
XnStatus xnvPointArea_UnregisterFromPointSilenced(XnVObjectID id, XnCallbackHandle hCallback)
{
	XnVPointArea* pPointArea = NULL;
	XnStatus rc = g_PointAreas.Get(id, pPointArea);
	if (rc != XN_STATUS_OK)
	{
		return XN_STATUS_NITE_UNEXPECTED_TYPE;
	}
	pPointArea->UnregisterPointSilenced(hCallback);
	return XN_STATUS_OK;
}
XnStatus xnvPointArea_UnregisterFromPointRevived(XnVObjectID id, XnCallbackHandle hCallback)
{
	XnVPointArea* pPointArea = NULL;
	XnStatus rc = g_PointAreas.Get(id, pPointArea);
	if (rc != XN_STATUS_OK)
	{
		return XN_STATUS_NITE_UNEXPECTED_TYPE;
	}
	pPointArea->UnregisterPointRevived(hCallback);
	return XN_STATUS_OK;
}
XnStatus xnvPointArea_UnregisterFromSilentPointRemoved(XnVObjectID id, XnCallbackHandle hCallback)
{
	XnVPointArea* pPointArea = NULL;
	XnStatus rc = g_PointAreas.Get(id, pPointArea);
	if (rc != XN_STATUS_OK)
	{
		return XN_STATUS_NITE_UNEXPECTED_TYPE;
	}
	pPointArea->UnregisterSilentPointRemoved(hCallback);
	return XN_STATUS_OK;
}
XnStatus xnvPointArea_RemoveSilent(XnVObjectID id, XnUInt64 nDelay, XnVObjectID sessionManagerId)
{
	XnVPointArea* pPointArea = NULL;
	XnStatus rc = g_PointAreas.Get(id, pPointArea);
	if (rc != XN_STATUS_OK)
	{
		return XN_STATUS_NITE_UNEXPECTED_TYPE;
	}

	XnVSessionGenerator* pGenerator = xnvSessionGenerator_GetFromHash(sessionManagerId);
	if (pGenerator == NULL)
	{
		return XN_STATUS_NITE_UNEXPECTED_TYPE;
	}

	pPointArea->RemoveSilent(nDelay, pGenerator);
	return XN_STATUS_OK;
}

