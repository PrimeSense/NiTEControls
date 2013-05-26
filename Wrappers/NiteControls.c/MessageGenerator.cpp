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
#include "MessageGenerator.h"
#include "MessageListener.h"
#include "XnVMessageGenerator.h"

XN_DECLARE_DEFAULT_HASH(XnVObjectID, XnVMessageGenerator*, MessageGeneratorsHash);
MessageGeneratorsHash g_MessageGenerators;

void xnvMessageGenerator_AddToHash(XnVObjectID id, XnVMessageGenerator* pGenerator)
{
	g_MessageGenerators.Set(id, pGenerator);
}

XnVMessageGenerator* xnvMessageGenerator_GetFromHash(XnVObjectID id)
{
	XnVMessageGenerator* pGenerator = NULL;
	XnStatus rc = g_MessageGenerators.Get(id, pGenerator);
	if (rc != XN_STATUS_OK)
	{
		return NULL;
	}
	return pGenerator;
}

XnVMessageGenerator* xnvMessageGenerator_RemoveFromHash(XnVObjectID id)
{
	XnVMessageGenerator* pGenerator = NULL;
	XnStatus rc = g_MessageGenerators.Get(id, pGenerator);
	if (rc != XN_STATUS_OK)
	{
		return NULL;
	}
	g_MessageGenerators.Remove(id);
	return pGenerator;
}

XnStatus xnvMessageGenerator_AddListener(XnVObjectID id, XnVObjectID listener)
{
	XnVMessageGenerator* pGenerator = NULL;
	XnStatus rc = g_MessageGenerators.Get(id, pGenerator);
	if (rc != XN_STATUS_OK)
	{
		return XN_STATUS_NITE_UNEXPECTED_TYPE;
	}
	XnVMessageListener* pListener = xnvMessageListener_GetFromHash(listener);
	if (pListener == NULL)
	{
		return XN_STATUS_NITE_UNEXPECTED_TYPE;
	}

	pGenerator->AddListener(pListener);
	return XN_STATUS_OK;
}
XnStatus xnvMessageGenerator_RemoveListener(XnVObjectID id, XnVObjectID listener)
{
	XnVMessageGenerator* pGenerator = NULL;
	XnStatus rc = g_MessageGenerators.Get(id, pGenerator);
	if (rc != XN_STATUS_OK)
	{
		return XN_STATUS_NITE_UNEXPECTED_TYPE;
	}
	XnVMessageListener* pListener = xnvMessageListener_GetFromHash(listener);
	if (pListener == NULL)
	{
		return XN_STATUS_NITE_UNEXPECTED_TYPE;
	}
	if (pGenerator->RemoveListener(pListener) == NULL)
	{
		return XN_STATUS_ERROR;
	}
	return XN_STATUS_OK;
}
const XnChar* xnvMessageGenerator_GetName(XnVObjectID id)
{
	XnVMessageGenerator* pGenerator = NULL;
	XnStatus rc = g_MessageGenerators.Get(id, pGenerator);
	if (rc != XN_STATUS_OK)
	{
		return "None";
	}

	return pGenerator->GetGeneratorName();
}

XnStatus xnvMessageGenerator_Generate(XnVObjectID id, void* pMessage)
{
	XnVMessageGenerator* pGenerator = NULL;
	XnStatus rc = g_MessageGenerators.Get(id, pGenerator);
	if (rc != XN_STATUS_OK)
	{
		return XN_STATUS_NITE_UNEXPECTED_TYPE;
	}
	XnVMessage* pAsMessage = (XnVMessage*)pMessage;
	pGenerator->Generate(pAsMessage);
	
	return XN_STATUS_OK;
}
