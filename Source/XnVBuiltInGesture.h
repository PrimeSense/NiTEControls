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
#ifndef _XNV_BUILT_IN_GESTURE_H_
#define _XNV_BUILT_IN_GESTURE_H_

#include <XnCppWrapper.h>
#include <XnStringsHash.h>
#include "XnVGesture.h"

class XnVBuiltInGesture :
	public XnVGesture
{
public:
	XnVBuiltInGesture(xn::GestureGenerator gestureGenerator, const XnChar* strGestures);
	~XnVBuiltInGesture();

	void Update(const xn::Context* pContext);

	void StartGesture(XnBoundingBox3D* pbbArea = NULL);
	void StopGesture();
protected:
	static void XN_CALLBACK_TYPE GestureRecognized(xn::GestureGenerator& generator, const XnChar* strGesture, const XnPoint3D* pIDPosition, const XnPoint3D* pEndPosition, void* pCookie);
	static void XN_CALLBACK_TYPE GestureProgress(xn::GestureGenerator& generator, const XnChar* strGesture, const XnPoint3D* pPosition, XnFloat fProgress, void* pCookie);

	xn::GestureGenerator m_GestureGenerator;
	XnCallbackHandle m_hCallbacks;
	XnChar* m_strGesturesList;

	XnStringsHash m_AvailableGestures;
	XnBool m_bActive;
};

#endif
