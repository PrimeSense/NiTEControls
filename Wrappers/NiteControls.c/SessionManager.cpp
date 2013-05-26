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
#include "XnVSessionManager.h"
#include "SessionGenerator.h"

XN_DECLARE_DEFAULT_HASH(XnVObjectID, XnVSessionManager*, SessionManagersHash);
SessionManagersHash g_SessionManagers;

XnVObjectID xnvSessionManager_Create(const XnChar* strName)
{
	XnVSessionManager* pSessionManager = XN_NEW(XnVSessionManager, strName);
	XnVObjectID id = pSessionManager;//AllocateID();
	g_SessionManagers.Set(id, pSessionManager);
	xnvSessionGenerator_AddToHash(id, pSessionManager);

	return id;
}
XnStatus xnvSessionManager_Destroy(XnVObjectID id)
{
	xnvSessionGenerator_RemoveFromHash(id);

	XnVSessionManager* pManager = NULL;
	XnStatus rc = g_SessionManagers.Get(id, pManager);
	if (rc != XN_STATUS_OK)
	{
		return XN_STATUS_NITE_UNEXPECTED_TYPE;
	}
	g_SessionManagers.Remove(id, pManager);

	XN_DELETE(pManager);
	return XN_STATUS_OK;
}
XnStatus xnvSessionManager_Initialize(XnVObjectID id, XnContext* pContext, const XnChar* strUseAsFocus,
									  XnNodeHandle tracker, XnNodeHandle focusGenerator)
{
	xn::HandsGenerator* pTracker = NULL;
	xn::GestureGenerator* pFocusGenerator = NULL;
	xn::Context* pxnContext = NULL;

	XnVSessionManager* pManager = NULL;
	XnStatus rc = g_SessionManagers.Get(id, pManager);
	if (rc != XN_STATUS_OK)
	{
		return XN_STATUS_NITE_UNEXPECTED_TYPE;
	}
	pxnContext = XN_NEW(xn::Context, pContext);
	if (tracker != 0)
	{
		pTracker = XN_NEW(xn::HandsGenerator, tracker);
	}
	if (focusGenerator != 0)
	{
		pFocusGenerator = XN_NEW(xn::GestureGenerator, focusGenerator);
	}

	rc = pManager->Initialize(pxnContext, strUseAsFocus, pTracker, pFocusGenerator);

	XN_DELETE(pxnContext);
	if (tracker != 0)
	{
		XN_DELETE(pTracker);
	}
	if (focusGenerator != 0)
	{
		XN_DELETE(pFocusGenerator);
	}

	return rc;
}
XnStatus xnvSessionManager_Update(XnVObjectID id, XnContext* pContext)
{
	XnVSessionManager* pManager = NULL;
	XnStatus rc = g_SessionManagers.Get(id, pManager);
	if (rc != XN_STATUS_OK)
	{
		return XN_STATUS_NITE_UNEXPECTED_TYPE;
	}
	xn::Context* pxnContext = XN_NEW(xn::Context, pContext);
	pManager->Update(pxnContext);
	XN_DELETE(pxnContext);
	return XN_STATUS_OK;
}
XnStatus xnvSessionManager_GetFocusPoint(XnVObjectID id, XnPoint3D* pFocus)
{
	XnVSessionManager* pManager = NULL;
	XnStatus rc = g_SessionManagers.Get(id, pManager);
	if (rc != XN_STATUS_OK)
	{
		return XN_STATUS_NITE_UNEXPECTED_TYPE;
	}

	return pManager->GetFocusPoint(*pFocus);
}

XnStatus xnvSessionManager_AddGestureByGenerator(XnVObjectID id, XnNodeHandle gestureGenerator, const XnChar* strName, XnUInt32* pGestureID)
{
	XnVSessionManager* pManager = NULL;
	xn::GestureGenerator* pGestureGenerator = NULL;

	XnStatus rc = g_SessionManagers.Get(id, pManager);
	if (rc != XN_STATUS_OK)
	{
		return XN_STATUS_NITE_UNEXPECTED_TYPE;
	}

	if (gestureGenerator != 0)
	{
		pGestureGenerator = XN_NEW(xn::GestureGenerator, gestureGenerator);
	}

	*pGestureID = pManager->AddGesture(*pGestureGenerator, strName);

	if (gestureGenerator != 0)
	{
		XN_DELETE(pGestureGenerator);
	}

	if (*pGestureID == 0)
	{
		return XN_STATUS_NITE_NO_MATCHING_GESTURE;
	}
	return XN_STATUS_OK;
}
XnStatus xnvSessionManager_AddGestureByContext(XnVObjectID id, XnContext* pContext, const XnChar* strName, XnUInt32* pGestureID)
{
	XnVSessionManager* pManager = NULL;
	xn::Context* pxnContext = NULL;
	XnStatus rc = g_SessionManagers.Get(id, pManager);
	if (rc != XN_STATUS_OK)
	{
		return XN_STATUS_NITE_UNEXPECTED_TYPE;
	}

	pxnContext = XN_NEW(xn::Context, pContext);
	*pGestureID = pManager->AddGesture(*pxnContext, strName);
	XN_DELETE(pxnContext);

	if (*pGestureID == 0)
	{
		return XN_STATUS_NITE_NO_MATCHING_GESTURE;
	}
	return XN_STATUS_OK;
}

XnStatus xnvSessionManager_RemoveGesture(XnVObjectID id, XnUInt32 nGestureID)
{
	XnVSessionManager* pManager = NULL;
	XnStatus rc = g_SessionManagers.Get(id, pManager);
	if (rc != XN_STATUS_OK)
	{
		return XN_STATUS_NITE_UNEXPECTED_TYPE;
	}

	pManager->RemoveGesture(nGestureID);
	return XN_STATUS_OK;

}
