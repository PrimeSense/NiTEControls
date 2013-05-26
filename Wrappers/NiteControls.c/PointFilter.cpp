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
#include "PointControl.h"
#include "MessageGenerator.h"
#include "XnVPointFilter.h"

XN_DECLARE_DEFAULT_HASH(XnVObjectID, XnVPointFilter*, PointFiltersHash);
PointFiltersHash g_PointFilters;

void xnvPointFilter_AddToHash(XnVObjectID id, XnVPointFilter* pFilter)
{
	g_PointFilters.Set(id, pFilter);
	xnvPointControl_AddToHash(id, pFilter);
	xnvMessageGenerator_AddToHash(id, pFilter);
}
XnVPointFilter* xnvPointFilter_RemoveFromHash(XnVObjectID id)
{
	xnvMessageGenerator_RemoveFromHash(id);
	xnvPointControl_RemoveFromHash(id);

	XnVPointFilter* pFilter = NULL;
	XnStatus rc = g_PointFilters.Get(id, pFilter);
	if (rc != XN_STATUS_OK)
	{
		return NULL;
	}
	g_PointFilters.Remove(id);
	return pFilter;
}

XnVObjectID xnvPointFilter_Create(const XnChar* strName)
{
	XnVPointFilter* pFilter = XN_NEW(XnVPointFilter, strName);
	XnVObjectID id = pFilter;//AllocateID();

	xnvPointFilter_AddToHash(id, pFilter);

	return id;
}

XnStatus xnvPointFilter_Destroy(XnVObjectID id)
{
	XnVPointFilter* pFilter = xnvPointFilter_RemoveFromHash(id);
	if (pFilter == NULL)
	{
		return XN_STATUS_NITE_UNEXPECTED_TYPE;
	}
	else
	{
		XN_DELETE(pFilter);
	}

	return XN_STATUS_OK;
}

XnStatus xnvPointFilter_OverridePrimary(XnVObjectID id, XnUInt32 nNewPrimary)
{
	XnVPointFilter* pPointFitler = NULL;
	XnStatus rc = g_PointFilters.Get(id, pPointFitler);
	if (rc != XN_STATUS_OK)
	{
		return XN_STATUS_NITE_UNEXPECTED_TYPE;
	}

	pPointFitler->OverridePrimary(nNewPrimary);
	return XN_STATUS_OK;
}