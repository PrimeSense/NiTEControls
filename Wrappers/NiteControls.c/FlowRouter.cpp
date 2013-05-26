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
#include "MessageListener.h"
#include "XnVFlowRouter.h"

XN_DECLARE_DEFAULT_HASH(XnVObjectID, XnVFlowRouter*, FlowRoutersHash);
FlowRoutersHash g_FlowRouters;

XnVObjectID xnvFlowRouter_Create(const XnChar* strName)
{
	XnVFlowRouter* pFlowRouter = XN_NEW(XnVFlowRouter, strName);
	XnVObjectID id = pFlowRouter;//AllocateID();
	g_FlowRouters.Set(id, pFlowRouter);
	xnvMessageListener_AddToHash(id, pFlowRouter);

	return id;
}
XnStatus  xnvFlowRouter_Destroy(XnVObjectID id)
{
	xnvMessageListener_RemoveFromHash(id);

	XnVFlowRouter* pFlowRouter = NULL;
	XnStatus rc = g_FlowRouters.Get(id, pFlowRouter);
	if (rc != XN_STATUS_OK)
	{
		return XN_STATUS_NITE_UNEXPECTED_TYPE;
	}
	g_FlowRouters.Remove(id);

	XN_DELETE(pFlowRouter);

	return XN_STATUS_OK;
}
XnStatus xnvFlowRouter_SetActive(XnVObjectID id, XnVObjectID messageListener)
{
	XnVFlowRouter* pFlowRouter = NULL;
	XnVMessageListener* pListener = NULL;
	XnStatus rc = g_FlowRouters.Get(id, pFlowRouter);
	if (rc != XN_STATUS_OK)
	{
		return XN_STATUS_NITE_UNEXPECTED_TYPE;
	}
	pListener = xnvMessageListener_GetFromHash(messageListener);
	if (pListener == NULL && messageListener != 0)
	{
		return XN_STATUS_NITE_UNEXPECTED_TYPE;
	}

	return pFlowRouter->SetActive(pListener);
}
XnStatus xnvFlowRouter_GetActive(XnVObjectID id, XnVObjectID* pMessageListener)
{
	XnVFlowRouter* pFlowRouter = NULL;
	XnVMessageListener* pListener = NULL;
	XnStatus rc = g_FlowRouters.Get(id, pFlowRouter);
	if (rc != XN_STATUS_OK)
	{
		return XN_STATUS_NITE_UNEXPECTED_TYPE;
	}
	XnVMessageListener* pActive = pFlowRouter->GetActive();

	*pMessageListener = xnvMessageListener_GetID(pActive);

	return XN_STATUS_OK;
}

