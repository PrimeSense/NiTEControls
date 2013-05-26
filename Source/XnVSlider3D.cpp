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
#include <math.h>
#include <XnVSlider3D.h>
#include "XnVNiteEvents.h"

XN_DECLARE_EVENT_3ARG(XnVValueChange3DSpecificEvent, XnVValueChange3DEvent, XnFloat, fValueX, XnFloat, fValueY, XnFloat, fValueZ);

XnVSlider3D::XnVSlider3D(const XnPoint3D& ptMin ,const XnPoint3D& ptMax) :
	m_ptMin(ptMin),
	m_ptMax(ptMax)
{
	m_pValueChangeCBs = XN_NEW(XnVValueChange3DSpecificEvent);

	m_fSizeX = fabs(ptMax.X - ptMin.X);
	m_fSizeY = fabs(ptMax.Y - ptMin.Y);
	m_fSizeZ = fabs(ptMax.Z - ptMin.Z);
} // XnVSlider3D::XnVSlider3D

XnVSlider3D::~XnVSlider3D()
{
	XN_DELETE(m_pValueChangeCBs);
} // XnVSlider3D::~XnVSlider3D

XnStatus XnVSlider3D::Update(const XnPoint3D& pt)
{
	XnFloat fX = XN_MIN(1.0f, XN_MAX((pt.X - m_ptMin.X) / m_fSizeX, 0.0f));
	XnFloat fY = XN_MIN(1.0f, XN_MAX((pt.Y - m_ptMin.Y) / m_fSizeY, 0.0f));
	XnFloat fZ = XN_MIN(1.0f, XN_MAX((pt.Z - m_ptMin.Z) / m_fSizeZ, 0.0f));

	ValueChange(fX, fY, fZ);

	return XN_STATUS_OK;
} // XnVSlider3D::Update

XnCallbackHandle XnVSlider3D::RegisterValueChange(void* cxt, XnVSlider3D::ValueChangeCB pCB) // Add CB to list
{
	XnCallbackHandle handle;
	m_pValueChangeCBs->Register(pCB, cxt, &handle);
	return handle;
} // XnVSlider3D::RegisterValueChange

void XnVSlider3D::UnregisterValueChange(XnCallbackHandle handle)
{
	m_pValueChangeCBs->Unregister(handle);
} // XnVSlider3D::UnregisterValueChange

// Event broadcast
void XnVSlider3D::ValueChange(XnFloat fXValue, XnFloat fYValue, XnFloat fZValue) // go over list and call all functions
{
	m_pValueChangeCBs->Raise(fXValue, fYValue, fZValue);
} // XnVSlider3D::ValueChange
