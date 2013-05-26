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
#ifndef _XNV_SESSION_LISTENER_H_
#define _XNV_SESSION_LISTENER_H_

#include "XnVNiteDefs.h"

/**
* A Session Listener will register to a Session Generator, which will call it when a Session event happens
*/
class XNV_NITE_CONTROLS_API XnVSessionListener
{
public:
	/**
	 * Type for callbacks when a focus gesture has started to be recognized
	 */
	typedef void (XN_CALLBACK_TYPE *OnFocusStartDetectedCB)(const XnChar* strFocus, const XnPoint3D& ptPosition, XnFloat fProgress, void* UserCxt);
	/**
	* Type for callbacks when a session has started
	*/
	typedef void (XN_CALLBACK_TYPE *OnSessionStartCB)(const XnPoint3D& ptPosition, void* UserCxt);
	/**
	* Type for callbacks when a session has ended
	*/
	typedef void (XN_CALLBACK_TYPE *OnSessionEndCB)(void* UserCxt);

	virtual ~XnVSessionListener() {}

	virtual void OnSessionStart(const XnPoint3D& ptPosition) = 0;
	virtual void OnSessionEnd() = 0;
	virtual void OnFocusStartDetected(const XnChar* strFocus, const XnPoint3D& ptPosition, XnFloat fProgress) = 0;
};


#endif // _XNV_SESSION_LISTENER_H_
