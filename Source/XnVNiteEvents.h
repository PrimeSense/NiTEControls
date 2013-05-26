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
#ifndef _XNV_NITE_EVENTS_H_
#define _XNV_NITE_EVENTS_H_

#include <XnOpenNI.h>
#include <XnEvent.h>
#include <XnHash.h>
#include "XnVNiteDefs.h"

XN_DECLARE_DEFAULT_HASH_DECL(XNV_NITE_CONTROLS_API, XnUInt32, XnUInt32, XnVIntHash);

XN_DECLARE_LIST_DECL(XNV_NITE_CONTROLS_API, XnUInt32, XnVIntList);

XN_DECLARE_EVENT_0ARG(XnVEvent, XnVEventIfc);

XN_DECLARE_EVENT_1ARG(XnVIntSpecificEvent, XnVIntEvent, XnInt32, nValue);
XN_DECLARE_EVENT_1ARG(XnVUintSpecificEvent, XnVUintEvent, XnUInt32, nValue);
XN_DECLARE_EVENT_1ARG(XnVFloatSpecificEvent, XnVFloatEvent, XnFloat, fValue);
XN_DECLARE_EVENT_1ARG(XnVDirectionSpecificEvent, XnVDirectionEvent, XnVDirection, eValue);

XN_DECLARE_EVENT_2ARG(XnVFloatFloatSpecificEvent, XnVFloatFloatEvent, XnFloat, fVal1, XnFloat, fVal2);
XN_DECLARE_EVENT_2ARG(XnVIntIntSpecificEvent, XnVIntIntEvent, XnInt32, nVal1, XnInt32, nVal2);

#endif // _XNV_NITE_EVENTS_H_