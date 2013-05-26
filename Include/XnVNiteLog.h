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
#ifndef _XNV_NITE_LOG_H_
#define _XNV_NITE_LOG_H_

#include <XnLog.h>

/**
* Creation of Generators and Listeners, and their destruction
*/
#define XNV_NITE_MASK_CREATE	"XNV_NITE_MASK_CREATE"
/**
* Any Generator sending Point Create/Destroy and Input Start/Stop, and any PointListener receiving such events.
* If multi-threaded environment is used, adding to the queue also uses this log mask.
*/
#define XNV_NITE_MASK_FLOW		"XNV_NITE_MASK_FLOW"
/**
* Connection and disconnection of Listeners to Generators, and changing active controls in Flow Routers
*/
#define XNV_NITE_MASK_CONNECT	"XNV_NITE_MASK_CONNECT"

/**
* Creation and destruction of points
*/
#define XNV_NITE_MASK_POINTS	"XNV_NITE_MASK_POINTS"

/**
 * Session state changes
 */
#define XNV_NITE_MASK_SESSION	"XNV_NITE_MASK_SESSION"
/**
 * Access to the multi-thread Queue
 */
#define XNV_NITE_MASK_MT_QUEUE	"XNV_NITE_MASK_MT_QUEUE"

/**
 * Events in specific controls and detectors
 */
#define XNV_NITE_MASK_EVENTS	"XNV_NITE_MASK_EVENTS"

#endif