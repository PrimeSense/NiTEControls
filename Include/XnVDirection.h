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
#ifndef _XNV_DIRECTION_H_
#define _XNV_DIRECTION_H_

/**
 * Type for axis-aligned directions
 */
typedef enum XnVDirection
{
	DIRECTION_LEFT,
	DIRECTION_RIGHT,
	DIRECTION_UP,
	DIRECTION_DOWN,
	DIRECTION_BACKWARD,
	DIRECTION_FORWARD,
	DIRECTION_ILLEGAL
} XnVDirection;

/**
 * The available axes
 */
typedef enum XnVAxis
{
	AXIS_X,
	AXIS_Y,
	AXIS_Z,
	AXIS_ILLEGAL
} XnVAxis;

#if __cplusplus
#define INLINE_FUNC inline
#else
#define INLINE_FUNC
#endif

/**
 * Translate from a direction to the axis it follows
 */
INLINE_FUNC XnVAxis XnVDirectionAsAxis(XnVDirection eDir)
{
	switch (eDir)
	{
	case DIRECTION_LEFT:
	case DIRECTION_RIGHT:
		return AXIS_X;
	case DIRECTION_UP:
	case DIRECTION_DOWN:
		return AXIS_Y;
	case DIRECTION_BACKWARD:
	case DIRECTION_FORWARD:
		return AXIS_Z;
	default:
		return AXIS_ILLEGAL;
	}
}

/**
 * Translate from a direction to its name
 */
INLINE_FUNC const XnChar* XnVDirectionAsString(XnVDirection eDir)
{
	switch (eDir)
	{
	case DIRECTION_LEFT:
		return "Left";
	case DIRECTION_RIGHT:
		return "Right";
	case DIRECTION_UP:
		return "Up";
	case DIRECTION_DOWN:
		return "Down";
	case DIRECTION_BACKWARD:
		return "Backward";
	case DIRECTION_FORWARD:
		return "Forward";
	default:
		return "Unknown";
	}
}

#endif // _XNV_DIRECTION_H_
