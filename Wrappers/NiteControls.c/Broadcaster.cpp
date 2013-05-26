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
#include "MessageGenerator.h"
#include "MessageListener.h"
#include "XnVBroadcaster.h"

XN_DECLARE_DEFAULT_HASH(XnVObjectID, XnVBroadcaster*, BroadcastersHash);
BroadcastersHash g_Broadcasters;

XnVObjectID xnvBroadcaster_Create(const XnChar* strName)
{
	XnVBroadcaster* pBroadcaster = XN_NEW(XnVBroadcaster, strName);
	XnVObjectID id = pBroadcaster;//AllocateID();
	g_Broadcasters.Set(id, pBroadcaster);
	xnvMessageGenerator_AddToHash(id, pBroadcaster);
	xnvMessageListener_AddToHash(id, pBroadcaster);

	return id;
}

XnStatus xnvBroadcaster_Destroy(XnVObjectID id)
{
	xnvMessageGenerator_RemoveFromHash(id);
	xnvMessageListener_RemoveFromHash(id);

	XnVBroadcaster* pBroadcaster = NULL;
	XnStatus rc = g_Broadcasters.Get(id, pBroadcaster);
	if (rc != XN_STATUS_OK)
	{
		return XN_STATUS_NITE_UNEXPECTED_TYPE;
	}
	g_Broadcasters.Remove(id);

	XN_DELETE(pBroadcaster);

	return XN_STATUS_OK;
}