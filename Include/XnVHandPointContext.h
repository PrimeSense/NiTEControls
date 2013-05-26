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
#ifndef _XNV_HAND_POINT_CONTEXT_H_
#define _XNV_HAND_POINT_CONTEXT_H_

#include "XnVNiteDefs.h"

/**
* A XnVHandPointContext describes a single hand point
*/
typedef struct XNV_NITE_CONTROLS_API XnVHandPointContext
{
	/**
	* The current position of the hand
	*/
	XnPoint3D ptPosition;
	/**
	* A persistent ID for the hand, allowing tracking
	*/
	XnUInt32 nID;
	/**
	* The user to which this hand belongs
	*/
	XnUInt32 nUserID;
	/**
	* The time in which this hand was in this position
	*/
	XnFloat fTime;
	/**
	* The probability the point is actually there
	* 1 - Fresh update. Lower - no update (or delete) for some time
	*/
	XnFloat fConfidence;
} XnVHandPointContext;

#endif
