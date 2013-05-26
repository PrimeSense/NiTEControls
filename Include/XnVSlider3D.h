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
#ifndef _XNV_SLIDER_3D_H_
#define _XNV_SLIDER_3D_H_

#include "XnVNiteDefs.h"

class XnVValueChange3DSpecificEvent;

/**
* This is a simple 3D slider. It receives a point,
* and normalizes it in the 3D space to numbers between 0 and 1 for each axis.
* The XnVSlider2D defines one event:
* - Its values have changed
*/
class XNV_NITE_CONTROLS_API XnVSlider3D
{
public:
	/**
	* Type for the value change event callback. The values are between 0 and 1.
	*/
	typedef void (XN_CALLBACK_TYPE *ValueChangeCB)(XnFloat fXValue, XnFloat fYValue, XnFloat fZValue, void* pUserCxt);

	/**
	* Creation. It receives 2 points, defining the 3D space.
	*/
	XnVSlider3D(const XnPoint3D& ptMin ,const XnPoint3D& ptMax);
	
	~XnVSlider3D();
	
	/**
	* The main function. It receives a point, normalizes it, and calculates the new values.
	*
	* @param	[in]	pt	The point to check in the slider.
	*/
	XnStatus Update(const XnPoint3D& pt);

	/**
	* Register for the value change event
	* 
	* @param	[in]	cxt	User's context
	* @param	[in]	CB	The Callback to call when the event is invoked.
	*
	* @return	A handle, to allow unregistration.
	*/
	XnCallbackHandle RegisterValueChange(void* cxt, ValueChangeCB CB); // Add CB to list
	/**
	* Unregister from the value change event
	*
	* @param	[in]	hCB	The handle provided on registration.
	*/
	void UnregisterValueChange(XnCallbackHandle hCB);
protected:
	// Event broadcast
	void ValueChange(XnFloat fXValue, XnFloat fYValue, XnFloat fZValue); // go over list and call all functions

	XnPoint3D m_ptMin, m_ptMax;
	XnFloat m_fSizeX, m_fSizeY, m_fSizeZ;

	XnVValueChange3DSpecificEvent* m_pValueChangeCBs;
}; // XnVSlider3D

#endif // _XNV_SLIDER_3D_H_
