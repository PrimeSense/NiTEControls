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
#include <XnHash.h>
#include "MessageListener.h"
#include "XnVMessageListener.h"

XN_DECLARE_DEFAULT_HASH(XnVObjectID, XnVMessageListener*, MessageListenersHash);
MessageListenersHash g_MessageListeners;

void xnvMessageListener_AddToHash(XnVObjectID id, XnVMessageListener* pListener)
{
	g_MessageListeners.Set(id, pListener);
}
XnVMessageListener* xnvMessageListener_GetFromHash(XnVObjectID id)
{
	XnVMessageListener* pListener = NULL;
	XnStatus rc = g_MessageListeners.Get(id, pListener);
	if (rc != XN_STATUS_OK)
	{
		return NULL;
	}
	return pListener;
}

XnVMessageListener* xnvMessageListener_RemoveFromHash(XnVObjectID id)
{
	XnVMessageListener* pListener = NULL;
	XnStatus rc = g_MessageListeners.Get(id, pListener);
	if (rc != XN_STATUS_OK)
	{
		return NULL;
	}
	g_MessageListeners.Remove(id);
	return pListener;
}
XnVObjectID xnvMessageListener_GetID(XnVMessageListener* pListener)
{
	for (MessageListenersHash::ConstIterator iter = g_MessageListeners.begin();
		iter != g_MessageListeners.end(); ++iter)
	{
		if (pListener == iter.Value())
		{
			return iter.Key();
		}
	}
	return 0;
}

XnStatus xnvMessageListener_RegisterActivate(XnVObjectID id, ActivateCB CB,
													 void* pCookie, XnCallbackHandle* pHandle)
{
	XN_VALIDATE_OUTPUT_PTR(pHandle);
	XnVMessageListener* pListener = NULL;
	XnStatus rc = g_MessageListeners.Get(id, pListener);
	if (rc != XN_STATUS_OK)
	{
		return XN_STATUS_NITE_UNEXPECTED_TYPE;
	}
	*pHandle = pListener->RegisterActivate(pCookie, CB);
	return XN_STATUS_OK;
}
XnStatus xnvMessageListener_RegisterDeactivate(XnVObjectID id, DeactivateCB CB,
											   void* pCookie, XnCallbackHandle* pHandle)
{
	XN_VALIDATE_OUTPUT_PTR(pHandle);
	XnVMessageListener* pListener = NULL;
	XnStatus rc = g_MessageListeners.Get(id, pListener);
	if (rc != XN_STATUS_OK)
	{
		return XN_STATUS_NITE_UNEXPECTED_TYPE;
	}
	*pHandle = pListener->RegisterDeactivate(pCookie, CB);
	return XN_STATUS_OK;
}
XnStatus xnvMessageListener_UnregisterActivate(XnVObjectID id, XnCallbackHandle hCB)
{
	XnVMessageListener* pListener = NULL;
	XnStatus rc = g_MessageListeners.Get(id, pListener);
	if (rc != XN_STATUS_OK)
	{
		return XN_STATUS_NITE_UNEXPECTED_TYPE;
	}
	pListener->UnregisterActivate(hCB);
	return XN_STATUS_OK;
}
XnStatus xnvMessageListener_UnregisterDeactivate(XnVObjectID id, XnCallbackHandle hCB)
{
	XnVMessageListener* pListener = NULL;
	XnStatus rc = g_MessageListeners.Get(id, pListener);
	if (rc != XN_STATUS_OK)
	{
		return XN_STATUS_NITE_UNEXPECTED_TYPE;
	}
	pListener->UnregisterDeactivate(hCB);
	return XN_STATUS_OK;
}

struct XnVExternalUpdateCB
{
	UpdateCB externalCB;
	void* pCookie;
};
XN_DECLARE_DEFAULT_HASH(XnCallbackHandle, XnVExternalUpdateCB*, ExternalUpdateCBs);
ExternalUpdateCBs g_ExternalUpdateCBs;
void XN_CALLBACK_TYPE InternalUpdateCB(XnVMessage* pMessage,
									   void* pCookie)
{
	XnVExternalUpdateCB* pExternalCB = (XnVExternalUpdateCB*)pCookie;
	if (pExternalCB->externalCB != NULL)
	{
		(*pExternalCB->externalCB)((void*)pMessage, pExternalCB->pCookie);
	}
}

XnStatus xnvMessageListener_RegisterUpdate(XnVObjectID id, UpdateCB CB,
													   void* pCookie, XnCallbackHandle* pHandle)
{
	XN_VALIDATE_OUTPUT_PTR(pHandle);
	XnVMessageListener* pListener = NULL;
	XnStatus rc = g_MessageListeners.Get(id, pListener);
	if (rc != XN_STATUS_OK)
	{
		return XN_STATUS_NITE_UNEXPECTED_TYPE;
	}
	XnVExternalUpdateCB* pNewExternal = XN_NEW(XnVExternalUpdateCB);
	pNewExternal->externalCB = CB;
	pNewExternal->pCookie = pCookie;
	*pHandle = pListener->RegisterUpdate(pNewExternal, InternalUpdateCB);
	g_ExternalUpdateCBs.Set(*pHandle, pNewExternal);

	return XN_STATUS_OK;
}
XnStatus xnvMessageListener_UnregisterUpdate(XnVObjectID id, XnCallbackHandle hCB)
{
	XnVMessageListener* pListener = NULL;
	XnStatus rc = g_MessageListeners.Get(id, pListener);
	if (rc != XN_STATUS_OK)
	{
		return XN_STATUS_NITE_UNEXPECTED_TYPE;
	}
	XnVExternalUpdateCB* pExternal = NULL;
	rc = g_ExternalUpdateCBs.Remove(hCB, pExternal);
	if (rc != XN_STATUS_OK)
	{
		return rc;
	}
	XN_DELETE(pExternal);

	pListener->UnregisterUpdate(hCB);
	return XN_STATUS_OK;
}

XnStatus xnvMessageListener_Update(XnVObjectID id, void* pMessage)
{
	XnVMessageListener* pListener = NULL;
	XnStatus rc = g_MessageListeners.Get(id, pListener);
	if (rc != XN_STATUS_OK)
	{
		return XN_STATUS_NITE_UNEXPECTED_TYPE;
	}

	XnVMessage* pMsg = (XnVMessage*)pMessage;
	pListener->Update(pMsg);
	return XN_STATUS_OK;
}
