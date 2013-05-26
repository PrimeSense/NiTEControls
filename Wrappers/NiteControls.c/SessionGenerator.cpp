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
#include "XnVSessionGenerator.h"
#include "MessageGenerator.h"

struct SessionState
{
	XnBool bState;
	XnVObjectID id;
};

XN_DECLARE_DEFAULT_HASH(XnVObjectID, SessionState*, SessionStateHash);
SessionStateHash g_SessionStates;


void XN_CALLBACK_TYPE SessionStarts(const XnPoint3D& ptPosition, void* UserCxt)
{
	SessionState* pState = (SessionState*)(UserCxt);
	pState->bState = TRUE;
}
void XN_CALLBACK_TYPE SessionEnds(void* UserCxt)
{
	SessionState* pState = (SessionState*)(UserCxt);
	pState->bState = FALSE;
}

XN_DECLARE_DEFAULT_HASH(XnVObjectID, XnVSessionGenerator*, SessionGeneratorsHash);
SessionGeneratorsHash g_SessionGenerators;

XnVSessionGenerator* xnvSessionGenerator_GetFromHash(XnVObjectID id)
{
	XnVSessionGenerator* pGenerator = NULL;
	XnStatus rc = g_SessionGenerators.Get(id, pGenerator);
	if (rc != XN_STATUS_OK)
	{
		return NULL;
	}
	return pGenerator;
}
void xnvSessionGenerator_AddToHash(XnVObjectID id, XnVSessionGenerator* pGenerator)
{
	g_SessionGenerators.Set(id, pGenerator);

	xnvMessageGenerator_AddToHash(id, pGenerator);

	SessionState* pState = new SessionState;
	pState->bState = FALSE;
	pState->id = id;
	g_SessionStates.Set(id, pState);
	pGenerator->RegisterSession(pState, SessionStarts, SessionEnds);
}

XnVSessionGenerator* xnvSessionGenerator_RemoveFromHash(XnVObjectID id)
{
	XnVSessionGenerator* pGenerator = NULL;

	xnvMessageGenerator_RemoveFromHash(id);

	XnStatus rc = g_SessionGenerators.Get(id, pGenerator);
	if (rc != XN_STATUS_OK)
	{
		return NULL;
	}
	g_SessionGenerators.Remove(id);
	SessionState* pState = NULL;
	g_SessionStates.Remove(id, pState);
	if (pState != NULL)
		delete pState;

	return pGenerator;
}

// XnVObjectID xnvSessionManager_Create(const XnChar* strName)
// {
// 	XnVSessionManager* pSessionManager = XN_NEW(XnVSessionManager, strName);
// 	XnVObjectID id = pSessionManager;//AllocateID();
// 	g_SessionManagers.Set(id, pSessionManager);
// 	xnvMessageGenerator_AddToHash(id, pSessionManager);
// 
// 	g_SessionStates.Set(id, false);
// 	pSessionManager->RegisterSession((void*)id, SessionStarts, SessionEnds);
// 
// 	return id;
// }
// XnStatus xnvSessionManager_Destroy(XnVObjectID id)
// {
// 	xnvMessageGenerator_RemoveFromHash(id);
// 
// 	XnVSessionManager* pManager = NULL;
// 	XnStatus rc = g_SessionManagers.Get(id, pManager);
// 	if (rc != XN_STATUS_OK)
// 	{
// 		return XN_STATUS_NITE_UNEXPECTED_TYPE;
// 	}
// 	g_SessionManagers.Remove(id, pManager);
// 	g_SessionStates.Remove(id);
// 
// 	XN_DELETE(pManager);
// 	return XN_STATUS_OK;
// }

XnStatus xnvSessionGenerator_LosePoint(XnVObjectID id, XnUInt32 pt)
{
	XnVSessionGenerator* pGenerator = NULL;
	XnStatus rc = g_SessionGenerators.Get(id, pGenerator);
	if (rc != XN_STATUS_OK)
	{
		return XN_STATUS_NITE_UNEXPECTED_TYPE;
	}
	return pGenerator->LosePoint(pt);
}
XnStatus xnvSessionGenerator_LosePoints(XnVObjectID id)
{
	XnVSessionGenerator* pGenerator = NULL;
	XnStatus rc = g_SessionGenerators.Get(id, pGenerator);
	if (rc != XN_STATUS_OK)
	{
		return XN_STATUS_NITE_UNEXPECTED_TYPE;
	}
	return pGenerator->LosePoints();
}
XnStatus xnvSessionGenerator_EndSession(XnVObjectID id)
{
	XnVSessionGenerator* pGenerator = NULL;
	XnStatus rc = g_SessionGenerators.Get(id, pGenerator);
	if (rc != XN_STATUS_OK)
	{
		return XN_STATUS_NITE_UNEXPECTED_TYPE;
	}
	return pGenerator->EndSession();
}
XnStatus xnvSessionGenerator_ForceSession(XnVObjectID id, const XnPoint3D* pFocus)
{
	XnVSessionGenerator* pGenerator = NULL;
	XnStatus rc = g_SessionGenerators.Get(id, pGenerator);
	if (rc != XN_STATUS_OK)
	{
		return XN_STATUS_NITE_UNEXPECTED_TYPE;
	}
	return pGenerator->ForceSession(*pFocus);
}
XnStatus xnvSessionGenerator_TrackPoint(XnVObjectID id, const XnPoint3D* pPosition)
{
	XnVSessionGenerator* pGenerator = NULL;
	XnStatus rc = g_SessionGenerators.Get(id, pGenerator);
	if (rc != XN_STATUS_OK)
	{
		return XN_STATUS_NITE_UNEXPECTED_TYPE;
	}
	return pGenerator->TrackPoint(*pPosition);
}
XnStatus xnvSessionGenerator_IsInSession(XnVObjectID id, XnBool* pnInSession)
{
	XN_VALIDATE_OUTPUT_PTR(pnInSession);
	XnVSessionGenerator* pGenerator = NULL;
	XnStatus rc = g_SessionGenerators.Get(id, pGenerator);
	if (rc != XN_STATUS_OK)
	{
		return XN_STATUS_NITE_UNEXPECTED_TYPE;
	}
	SessionState* pState = NULL;
	g_SessionStates.Get(id, pState);
	if (pState == NULL)
	{
		return XN_STATUS_ERROR;
	}
	*pnInSession = pState->bState;
	return XN_STATUS_OK;
}

XnStatus xnvSessionGenerator_GetFocusPoint(XnVObjectID id, XnPoint3D* pFocus)
{
	XnVSessionGenerator* pGenerator = NULL;
	XnStatus rc = g_SessionGenerators.Get(id, pGenerator);
	if (rc != XN_STATUS_OK)
	{
		return XN_STATUS_NITE_UNEXPECTED_TYPE;
	}

	return pGenerator->GetFocusPoint(*pFocus);
}
XN_DECLARE_DEFAULT_HASH(XnCallbackHandle, XnVHandle, HandlesHash);
XN_DECLARE_DEFAULT_HASH(XnVObjectID, HandlesHash*, SMHandlesHash);
SMHandlesHash g_SMHandlesHash;

struct XnVExternalSessionCBs
{
	SessionStartCB externalSessionStartCB;
	SessionEndCB externalSessionEndCB;
	SessionFocusProgressCB externalSessionFocusProgressCB;
	void* pCookie;
};
XN_DECLARE_DEFAULT_HASH(XnVHandle, XnVExternalSessionCBs*, ExternalSessionsCBs);
ExternalSessionsCBs g_ExternalSessionCBs;

typedef void (XN_CALLBACK_TYPE *OnFocusStartDetectedCB)(const XnChar* strFocus, const XnPoint3D& ptPosition, XnFloat fProgress, void* UserCxt);
typedef void (XN_CALLBACK_TYPE *OnSessionEndCB)(void* UserCxt);

void XN_CALLBACK_TYPE InternalSessionStartCB(const XnPoint3D& ptPosition, void* pCookie)
{
	XnVExternalSessionCBs* pExternalNoCB = (XnVExternalSessionCBs*)pCookie;
	if (pExternalNoCB->externalSessionStartCB != NULL)
	{
		(*pExternalNoCB->externalSessionStartCB)(&ptPosition, pExternalNoCB->pCookie);
	}
}
void XN_CALLBACK_TYPE InternalSessionEndCB(void* pCookie)
{
	XnVExternalSessionCBs* pExternalNoCB = (XnVExternalSessionCBs*)pCookie;
	if (pExternalNoCB->externalSessionEndCB != NULL)
	{
		(*pExternalNoCB->externalSessionEndCB)(pExternalNoCB->pCookie);
	}
}
void XN_CALLBACK_TYPE InternalSessionFocusProgressCB(const XnChar* strFocus, const XnPoint3D& ptPosition, XnFloat fProgress, void* pCookie)
{
	XnVExternalSessionCBs* pExternalNoCB = (XnVExternalSessionCBs*)pCookie;
	if (pExternalNoCB->externalSessionFocusProgressCB != NULL)
	{
		(*pExternalNoCB->externalSessionFocusProgressCB)(strFocus, &ptPosition, fProgress, pExternalNoCB->pCookie);
	}
}


XnStatus RegisterSession(XnVObjectID id, SessionStartCB StartCB, SessionEndCB EndCB, SessionFocusProgressCB ProgressCB, void* pCookie, XnCallbackHandle* pHandle)
{
	XN_VALIDATE_OUTPUT_PTR(pHandle);
	XnVSessionGenerator* pGenerator = NULL;
	XnStatus rc = g_SessionGenerators.Get(id, pGenerator);
	if (rc != XN_STATUS_OK)
	{
		return XN_STATUS_NITE_UNEXPECTED_TYPE;
	}

	HandlesHash* pHash = NULL;
	rc = g_SMHandlesHash.Get(id, pHash);
	if (rc != XN_STATUS_OK)
	{
		pHash = XN_NEW(HandlesHash);
		g_SMHandlesHash.Set(id, pHash);
	}
	*pHandle = NULL;
	if (StartCB != NULL)
		*pHandle = (void*)StartCB;
	else if (EndCB != NULL)
		*pHandle = (void*)EndCB;
	else if (ProgressCB != NULL)
		*pHandle = (void*)ProgressCB;
	else
		return XN_STATUS_ERROR;

	XnVExternalSessionCBs* pExternalsCBs = new XnVExternalSessionCBs;
	pExternalsCBs->externalSessionStartCB = StartCB;
	pExternalsCBs->externalSessionEndCB = EndCB;
	pExternalsCBs->externalSessionFocusProgressCB = ProgressCB;
	pExternalsCBs->pCookie = pCookie;

	XnVHandle handle = pGenerator->RegisterSession(pExternalsCBs, InternalSessionStartCB, InternalSessionEndCB, InternalSessionFocusProgressCB);
	pHash->Set(*pHandle, handle);
	return XN_STATUS_OK;
}

XnStatus xnvSessionGenerator_RegisterToSessionStart(XnVObjectID id, SessionStartCB CB, void* pCookie, XnCallbackHandle* pHandle)
{
	return RegisterSession(id, CB, NULL, NULL, pCookie, pHandle);
}
XnStatus xnvSessionGenerator_RegisterToSessionEnd(XnVObjectID id, SessionEndCB CB, void* pCookie, XnCallbackHandle* pHandle)
{
	return RegisterSession(id, NULL, CB, NULL, pCookie, pHandle);
}
XnStatus xnvSessionGenerator_RegisterToSessionFocusProgress(XnVObjectID id, SessionFocusProgressCB CB, void* pCookie, XnCallbackHandle* pHandle)
{
	return RegisterSession(id, NULL, NULL, CB, pCookie, pHandle);
}
XnStatus UnregisterSession(XnVObjectID id, XnCallbackHandle hCB)
{
	XnVSessionGenerator* pGenerator = NULL;
	XnStatus rc = g_SessionGenerators.Get(id, pGenerator);
	if (rc != XN_STATUS_OK)
	{
		return XN_STATUS_NITE_UNEXPECTED_TYPE;
	}
	HandlesHash* pHash = NULL;
	rc = g_SMHandlesHash.Get(id, pHash);
	if (rc != XN_STATUS_OK)
	{
		return rc;
	}
	XnVHandle handle;
	rc = pHash->Remove(hCB, handle);
	if (rc != XN_STATUS_OK)
	{
		return rc;
	}

	XnVExternalSessionCBs* pExternal = NULL;
	rc = g_ExternalSessionCBs.Remove(handle, pExternal);
	if (rc != XN_STATUS_OK)
	{
		return rc;
	}
	XN_DELETE(pExternal);

	pGenerator->UnregisterSession(handle);
	return XN_STATUS_OK;
}

XnStatus xnvSessionGenerator_UnregisterFromSessionStart(XnVObjectID id, XnCallbackHandle hCB)
{
	return UnregisterSession(id, hCB);
}
XnStatus xnvSessionGenerator_UnregisterFromSessionEnd(XnVObjectID id, XnCallbackHandle hCB)
{
	return UnregisterSession(id, hCB);
}
XnStatus xnvSessionGenerator_UnregisterFromSessionFocusProgress(XnVObjectID id, XnCallbackHandle hCB)
{
	return UnregisterSession(id, hCB);
}
