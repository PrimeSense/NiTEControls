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
#include "XnVNiteEvents.h"
#include "XnVGesture.h"

XN_DECLARE_EVENT_3ARG(XnVGestureRecognizeSpecificEvent, XnVGestureRecognizeEvent, const XnChar*, strGesture, const XnPoint3D&, ptIDPos, const XnPoint3D&, ptEndPos);
XN_DECLARE_EVENT_3ARG(XnVGestureStartRecognizeSpecificEvent, XnVGestureStartRecognizeEvent, const XnChar*, strGesture, const XnPoint3D&, ptPos, XnFloat, fProgress);


XnVGesture::XnVGesture(const XnChar* strName)
{
	m_pRecognizeCBs = XN_NEW(XnVGestureRecognizeSpecificEvent);
	m_pStartRecognizeCBs = XN_NEW(XnVGestureStartRecognizeSpecificEvent);
}

XnVGesture::~XnVGesture()
{
	XN_DELETE(m_pRecognizeCBs);
	XN_DELETE(m_pStartRecognizeCBs);
}

void XnVGesture::Recognized(const XnChar* strGesture, const XnPoint3D& ptIDPosition, const XnPoint3D& ptEndPosition)
{
	m_pRecognizeCBs->Raise(strGesture, ptIDPosition, ptEndPosition);
} // XnVGesture::Recognized

void XnVGesture::StartRecognized(const XnChar* strGesture, const XnPoint3D& ptPosition, XnFloat fProgress)
{
	m_pStartRecognizeCBs->Raise(strGesture, ptPosition, fProgress);
} // XnVGesture::StartRecognized

XnCallbackHandle XnVGesture::RegisterRecognize(void* cxt, GestureRecognizedCB CB)
{
	XnCallbackHandle handle;
	m_pRecognizeCBs->Register(CB, cxt, &handle);
	return handle;
} // XnVGesture::RegisterRecognize

XnCallbackHandle XnVGesture::RegisterStartRecognize(void* cxt, GestureStartRecognizedCB CB)
{
	XnCallbackHandle handle;
	m_pStartRecognizeCBs->Register(CB, cxt, &handle);
	return handle;
} // XnVGesture::RegisterStartRecognize

void XnVGesture::UnregisterRecognize(XnCallbackHandle handle)
{
	m_pRecognizeCBs->Unregister(handle);
} // XnVGesture::UnregisterRecognize

void XnVGesture::UnregisterStartRecognize(XnCallbackHandle handle)
{
	m_pStartRecognizeCBs->Unregister(handle);
} // XnVGesture::UnregisterStartRecognize