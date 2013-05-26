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
#ifndef _XNV_GESTURE_H_
#define _XNV_GESTURE_H_
#include <XnCppWrapper.h>
#include "XnVNiteDefs.h"
//#include "XnVMessageGenerator.h"

class XnVGestureRecognizeSpecificEvent;
class XnVGestureStartRecognizeSpecificEvent;

/**
* A XnVGesture is a Context Control, which identifies gestures. 
*/
class XNV_NITE_CONTROLS_API XnVGesture
{
public:
	/**
	* Type for a recognized Gesture.
	* It includes the ID of the recognized gesture,
	* the position in which it was identified and the position in which it ended.
	*/
	typedef void (XN_CALLBACK_TYPE* GestureRecognizedCB)(const XnChar* strGesture, const XnPoint3D& ptIDPosition, const XnPoint3D& ptEndPosition, void* cxt);
	/**
	* Type for callbacks that a gesture started to be recognized.
	*/
	typedef void (XN_CALLBACK_TYPE* GestureStartRecognizedCB)(const XnChar* strGesture, const XnPoint3D& ptPosition, XnFloat fProgress, void* cxt);

	/**
	* Constructor. Create a new Gesture
	*
	* @param	[in]	strName		Name of the control, for log purposes.
	*/
	XnVGesture(const XnChar* strName = "XnVGesture");
	virtual ~XnVGesture();

	/**
	* Start looking for a gesture, in a specific area (in Real-World coordinates)
	*
	* @param	[in]	pbbArea	The area in which to look for the gesture. NULL means all over
	*/
	virtual void StartGesture(XnBoundingBox3D* pbbArea = NULL) {}
	/**
	* Stop looking for the gesture
	*/
	virtual void StopGesture() {}

	/**
	* Call all callbacks registered to the 'Gesture Recognized' event
	*
	* @param	[in]	strGesture	The name of the recognized gesture
	* @param	[in]	ptIDPosition	The position in which the gesture was recognized
	* @param	[in]	ptEndPosition	The position in which the gesture has ended
	*/
	void Recognized(const XnChar* strGesture, const XnPoint3D& ptIDPosition, const XnPoint3D& ptEndPosition);
	/**
	* Call all callbacks registered to the 'Gesture Started' event
	*
	* @param	[in]	strGesture	The name of the started gesture
	* @param	[in]	ptPosition	The position in which the gesture started
	* @param	[in]	fProgress	How much of the gesture was already completed
	*/
	void StartRecognized(const XnChar* strGesture, const XnPoint3D& ptPosition, XnFloat fProgress);

	/**
	* Register a callback to the 'Gesture Recognized' event
	*
	* @param	[in]	cxt	User's context. The callback will be called with that context.
	* @param	[in]	CB	The Callback
	*
	* @return	A handle to the callback, to allow unregistering.
	*/
	XnCallbackHandle RegisterRecognize(void* cxt, GestureRecognizedCB CB);
	/**
	* Register a callback to the 'Gesture Started' event
	*
	* @param	[in]	cxt	User's context. The callback will be called with that context.
	* @param	[in]	CB	The Callback
	*
	* @return	A handle to the callback, to allow unregistering.
	*/
	XnCallbackHandle RegisterStartRecognize(void* cxt, GestureStartRecognizedCB CB);

	/**
	* Unregister a callback from the 'Gesture Recognized' event
	*
	* @param	[in]	hCB	The handle received when registering the callback.
	*/
	void UnregisterRecognize(XnCallbackHandle hCB);
	/**
	* Unregister a callback from the 'Gesture Started' event
	*
	* @param	[in]	hCB	The handle received when registering the callback.
	*/
	void UnregisterStartRecognize(XnCallbackHandle hCB);

	virtual void Update(const xn::Context* pContext) = 0;
private:
	XnVGestureStartRecognizeSpecificEvent* m_pStartRecognizeCBs;
	XnVGestureRecognizeSpecificEvent* m_pRecognizeCBs;
};

#endif
