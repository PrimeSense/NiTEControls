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
#ifndef _XNV_NITE_STATUS_H_
#define _XNV_NITE_STATUS_H_

#include <XnCommon.h>

XN_PS_STATUS_MESSAGE_MAP_START(XN_ERROR_GROUP_EE_NITE)
XN_STATUS_MESSAGE(XN_STATUS_NITE_OUT_OF_RANGE, "Out of range")
XN_STATUS_MESSAGE(XN_STATUS_NITE_NOT_ENOUGH_TIME, "Not enough time for calculation")
XN_STATUS_MESSAGE(XN_STATUS_NITE_COOLDOWN, "Inside cooldown period")
XN_STATUS_MESSAGE(XN_STATUS_NITE_UNEXPECTED_DIRECTION, "Unexpected direction")
XN_STATUS_MESSAGE(XN_STATUS_NITE_NO_SUCH_POINT, "No such point")
XN_STATUS_MESSAGE(XN_STATUS_NITE_CANT_REMOVE_POINT, "Can't remove points")
XN_STATUS_MESSAGE(XN_STATUS_NITE_SHOULDNT_REMOVE_POINT, "Shouldn't remove points")
XN_STATUS_MESSAGE(XN_STATUS_NITE_ILLEGAL_INPUT, "Input is illegal")
XN_STATUS_MESSAGE(XN_STATUS_NITE_UNSUPPORTED, "Unsupported NITE feature")
XN_STATUS_MESSAGE(XN_STATUS_NITE_DEPTH_UNAVAILABLE, "No depth available!")

XN_STATUS_MESSAGE(XN_STATUS_NITE_NO_TRACKER, "No hand generator found. One is needed for session manager.")
XN_STATUS_MESSAGE(XN_STATUS_NITE_NO_GESTURE, "No gesture generator found. One is needed for session manager.")
XN_STATUS_MESSAGE(XN_STATUS_NITE_NO_MATCHING_GESTURE, "No gesture generator was found that supports all required gestures.")

XN_STATUS_MESSAGE(XN_STATUS_NITE_UNEXPECTED_TYPE, "Object is of not of expected type")
XN_PS_STATUS_MESSAGE_MAP_END(XN_ERROR_GROUP_EE_NITE)

// enum
// {
// 	XN_STATUS_NITE_OUT_OF_RANGE = 0x4000000,
// 	XN_STATUS_NITE_NOT_ENOUGH_TIME,
// 	XN_STATUS_NITE_COOLDOWN,
// 	XN_STATUS_NITE_UNEXPECTED_DIRECTION,
// 	XN_STATUS_NITE_NO_SUCH_POINT,
// 	XN_STATUS_NITE_CANT_REMOVE_POINT,
// 	XN_STATUS_NITE_SHOULDNT_REMOVE_POINT,
// 	XN_STATUS_NITE_ILLEGAL_INPUT,
// 	XN_STATUS_NITE_UNSUPPORTED,
// 	XN_STATUS_NITE_DEPTH_UNAVAILABLE,
// 	XN_STATUS_NITE_NO_TRACKER,
// 	XN_STATUS_NITE_NO_GESTURE,
// 	XN_STATUS_NITE_NO_MATCHING_GESTURE,
// 	XN_STATUS_NITE_UNEXPECTED_TYPE
// };


#endif