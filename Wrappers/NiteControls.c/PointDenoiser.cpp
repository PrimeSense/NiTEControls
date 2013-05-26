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
#include "XnVPointDenoiser.h"

XN_DECLARE_DEFAULT_HASH(XnVObjectID, XnVPointDenoiser*, PointDenoisersHash);
PointDenoisersHash g_PointDenoisers;

XnVObjectID xnvPointDenoiser_Create(XnFloat fDistanceThreshold, const XnChar* strName)
{
	XnVPointDenoiser* pPointDenoiser = XN_NEW(XnVPointDenoiser, fDistanceThreshold, strName);
	XnVObjectID id = pPointDenoiser;//AllocateID();
	g_PointDenoisers.Set(id, pPointDenoiser);
	xnvPointFilter_AddToHash(id, pPointDenoiser);

	return id;
}
XnStatus xnvPointDenoiser_Destroy(XnVObjectID id)
{
	xnvPointFilter_RemoveFromHash(id);

	XnVPointDenoiser* pPointDenoiser = NULL;
	XnStatus rc = g_PointDenoisers.Get(id, pPointDenoiser);
	if (rc != XN_STATUS_OK)
	{
		return XN_STATUS_NITE_UNEXPECTED_TYPE;
	}
	g_PointDenoisers.Remove(id);

	XN_DELETE(pPointDenoiser);
	return XN_STATUS_OK;
}
XnStatus xnvPointDenoiser_GetDistanceThreshold(XnVObjectID id, XnFloat* pfDistanceThreshold)
{
	XnVPointDenoiser* pPointDenoiser = NULL;
	XnStatus rc = g_PointDenoisers.Get(id, pPointDenoiser);
	if (rc != XN_STATUS_OK)
	{
		return XN_STATUS_NITE_UNEXPECTED_TYPE;
	}
	*pfDistanceThreshold = pPointDenoiser->GetDistanceThreshold();
	return XN_STATUS_OK;
}
XnStatus xnvPointDenoiser_SetDistanceThreshold(XnVObjectID id, XnFloat fDistanceThreshold)
{
	XnVPointDenoiser* pPointDenoiser = NULL;
	XnStatus rc = g_PointDenoisers.Get(id, pPointDenoiser);
	if (rc != XN_STATUS_OK)
	{
		return XN_STATUS_NITE_UNEXPECTED_TYPE;
	}
	pPointDenoiser->SetDistanceThreshold(fDistanceThreshold);
	return XN_STATUS_OK;
}
XnStatus xnvPointDenoiser_GetCloseRatio(XnVObjectID id, XnFloat* pfCloseRatio)
{
	XN_VALIDATE_OUTPUT_PTR(pfCloseRatio);
	XnVPointDenoiser* pPointDenoiser = NULL;
	XnStatus rc = g_PointDenoisers.Get(id, pPointDenoiser);
	if (rc != XN_STATUS_OK)
	{
		return XN_STATUS_NITE_UNEXPECTED_TYPE;
	}
	*pfCloseRatio = pPointDenoiser->GetCloseRatio();
	return XN_STATUS_OK;
}
XnStatus xnvPointDenoiser_GetFarRatio(XnVObjectID id, XnFloat* pfFarRatio)
{
	XN_VALIDATE_OUTPUT_PTR(pfFarRatio);
	XnVPointDenoiser* pPointDenoiser = NULL;
	XnStatus rc = g_PointDenoisers.Get(id, pPointDenoiser);
	if (rc != XN_STATUS_OK)
	{
		return XN_STATUS_NITE_UNEXPECTED_TYPE;
	}
	*pfFarRatio =  pPointDenoiser->GetFarRatio();
	return XN_STATUS_OK;
}
XnStatus xnvPointDenoiser_SetCloseRatio(XnVObjectID id, XnFloat fCloseRatio)
{
	XnVPointDenoiser* pPointDenoiser = NULL;
	XnStatus rc = g_PointDenoisers.Get(id, pPointDenoiser);
	if (rc != XN_STATUS_OK)
	{
		return XN_STATUS_NITE_UNEXPECTED_TYPE;
	}
	pPointDenoiser->SetCloseRatio(fCloseRatio);
	return XN_STATUS_OK;
}
XnStatus xnvPointDenoiser_SetFarRatio(XnVObjectID id, XnFloat fFarRatio)
{
	XnVPointDenoiser* pPointDenoiser = NULL;
	XnStatus rc = g_PointDenoisers.Get(id, pPointDenoiser);
	if (rc != XN_STATUS_OK)
	{
		return XN_STATUS_NITE_UNEXPECTED_TYPE;
	}
	pPointDenoiser->SetFarRatio(fFarRatio);
	return XN_STATUS_OK;
}


