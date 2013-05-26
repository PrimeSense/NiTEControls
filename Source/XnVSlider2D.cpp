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
#include "XnVSlider2D.h"
#include "XnVPointBuffer.h"
#include "XnV3DVector.h"
#include "XnVNiteEvents.h"

const XnFloat XnVSlider2D::ms_fOffAxisDefaultMininumVelocity = 0.17f;
const XnFloat XnVSlider2D::ms_fOffAxisDefaultDetectionAngle = 60.0f;
const XnUInt32 XnVSlider2D::ms_nDefaultTimeForOffAxisDetection = 350;

XnVSlider2D::XnVSlider2D(const XnPoint3D& ptMin ,const XnPoint3D& ptMax) :
	m_ptMin(ptMin),
	m_ptMax(ptMax)
{
	m_pValueChangeCBs = XN_NEW(XnVFloatFloatSpecificEvent);
	m_pOffAxisMovementCBs = XN_NEW(XnVDirectionSpecificEvent);

	m_fSizeX = fabs(ptMax.X - ptMin.X);
	m_fSizeY = fabs(ptMax.Y - ptMin.Y);

	m_pPointBuffer = XN_NEW(XnVPointBuffer);

	m_fOffAxisDetectionAngle = ms_fOffAxisDefaultDetectionAngle;
	m_fOffAxisDetectionVelocity = ms_fOffAxisDefaultMininumVelocity;
	m_nOffAxisDetectionTime = ms_nDefaultTimeForOffAxisDetection;
} // XnVSlider2D::XnVSlider2D

XnVSlider2D::XnVSlider2D(const XnPoint3D& ptInitialPosition, XnFloat fSliderXLength, XnFloat fSliderYLength,
						 XnFloat fInitialXValue, XnFloat fInitialYValue)
{
	m_pValueChangeCBs = XN_NEW(XnVFloatFloatSpecificEvent);
	m_pOffAxisMovementCBs = XN_NEW(XnVDirectionSpecificEvent);

	m_fSizeX = fSliderXLength;
	m_fSizeY = fSliderYLength;

	m_ptMin.X = ptInitialPosition.X - fSliderXLength*fInitialXValue;
	m_ptMin.Y = ptInitialPosition.Y - fSliderYLength*fInitialYValue;

	m_ptMax.X = m_ptMin.X + m_fSizeX;
	m_ptMax.Y = m_ptMax.Y + m_fSizeY;

	m_pPointBuffer = XN_NEW(XnVPointBuffer);

	m_fOffAxisDetectionAngle = ms_fOffAxisDefaultDetectionAngle;
	m_fOffAxisDetectionVelocity = ms_fOffAxisDefaultMininumVelocity;
	m_nOffAxisDetectionTime = ms_nDefaultTimeForOffAxisDetection;
} // XnVSlider2D::XnVSlider2D

XnVSlider2D::~XnVSlider2D()
{
	if (m_pPointBuffer != NULL)
	{
		XN_DELETE(m_pPointBuffer);
	}

	XN_DELETE(m_pValueChangeCBs);
	XN_DELETE(m_pOffAxisMovementCBs);
} // XnVSlider2D::~XnVSlider2D

XnInt32 XnVSlider2D::CheckForOffAxisMovement(const XnPoint3D& pt, XnFloat fTime)
{
	XnFloat fOffAxisDelta;
	XnFloat fPrimaryAxisDelta;

	if (m_pPointBuffer->GetAvailableTimespan() < m_nOffAxisDetectionTime)
	{
		return 0;
	}

	XnV3DVector ptAverageVelocity = m_pPointBuffer->GetAverageVelocityByTime(m_nOffAxisDetectionTime, fTime);

	fOffAxisDelta = ptAverageVelocity.Z;
	ptAverageVelocity.Z = 0;
	fPrimaryAxisDelta = ptAverageVelocity.Magnitude();

	if (fabs(fOffAxisDelta) > m_fOffAxisDetectionVelocity)
	{

		XnFloat fSlope = fOffAxisDelta / fPrimaryAxisDelta;
		// It's okay for PrimaryAxisDelta to be 0
		// slope is the tangent of the angle
		XnFloat fAngle = atan(fSlope) * (180.0f / XnVMathCommon::PI);
		// If PrimaryAxisDelta was 0, Slope was +/- Inifinity, so
		// angle will be +/- 90 degrees, which is correct for us
		if (fabs(fAngle) > m_fOffAxisDetectionAngle)
		{
			// Angle passed the threshold
			return (fOffAxisDelta > 0) ? 1 : -1;
		}
	}

	return 0;
} // XnVSlider2D::CheckForOffAxisMovement

XnStatus XnVSlider2D::Update(const XnPoint3D& pt, XnFloat fTime, XnBool bCheckOffAxis)
{
	if (m_pPointBuffer == NULL)
	{
		return XN_STATUS_NOT_INIT;
	}

	m_pPointBuffer->AddPoint(pt, fTime);

	if (bCheckOffAxis)
	{
		XnInt32 nOffAxisMovementSign = CheckForOffAxisMovement(pt, fTime);
		if (nOffAxisMovementSign > 0)
		{
			m_ptCurrentPosition = m_pPointBuffer->GetAveragePointByTime(m_nOffAxisDetectionTime, fTime, 1);
			OffAxisMovement(DIRECTION_BACKWARD);
		}
		else if (nOffAxisMovementSign < 0)
		{
			m_ptCurrentPosition = m_pPointBuffer->GetAveragePointByTime(m_nOffAxisDetectionTime, fTime, 1);
			OffAxisMovement(DIRECTION_FORWARD);
		}
	}

	XnFloat fX = XN_MIN(1.0f, XN_MAX((pt.X - m_ptMin.X) / m_fSizeX, 0.0f));
	XnFloat fY = XN_MIN(1.0f, XN_MAX((pt.Y - m_ptMin.Y) / m_fSizeY, 0.0f));

	ValueChange(fX, fY);

	return XN_STATUS_OK;

} // XnVSlider2D::Update

XnCallbackHandle XnVSlider2D::RegisterValueChange(void* cxt, XnVSlider2D::ValueChangeCB pCB) // Add CB to list
{
	XnCallbackHandle handle;
	m_pValueChangeCBs->Register(pCB, cxt, &handle);
	return handle;
} // XnVSlider2D::RegisterValueChange

void XnVSlider2D::UnregisterValueChange(XnCallbackHandle handle)
{
	m_pValueChangeCBs->Unregister(handle);
} // XnVSlider2D::UnregisterValueChange
XnCallbackHandle XnVSlider2D::RegisterOffAxisMovement(void *cxt, XnVSlider2D::OffAxisMovementCB pCB)
{
	XnCallbackHandle handle;
	m_pOffAxisMovementCBs->Register(pCB, cxt, &handle);
	return handle;
} // XnVSlider2D::RegisterOffAxisMovement

void XnVSlider2D::UnregisterOffAxisMovement(XnCallbackHandle handle)
{
	m_pOffAxisMovementCBs->Unregister(handle);
} // XnVSlider2D::UnregsiterOffAxisMovement

// Event broadcast
void XnVSlider2D::ValueChange(XnFloat fXValue, XnFloat fYValue) // go over list and call all functions
{
	m_pValueChangeCBs->Raise(fXValue, fYValue);
} // XnVSlider2D::ValueChange
void XnVSlider2D::OffAxisMovement(XnVDirection eDir)
{
	m_pOffAxisMovementCBs->Raise(eDir);
} // XnVSlider2D::OffAxisMovement


XnFloat  XnVSlider2D::GetOffAxisDetectionVelocity() const
{
	return m_fOffAxisDetectionVelocity;
}
XnFloat  XnVSlider2D::GetOffAxisDetectionAngle() const
{
	return m_fOffAxisDetectionAngle;
}
XnUInt32 XnVSlider2D::GetOffAxisDetectionTime() const
{
	return m_nOffAxisDetectionTime;
}

void XnVSlider2D::SetOffAxisDetectionVelocity(XnFloat fVelocity)
{
	m_fOffAxisDetectionVelocity = fVelocity;
}
void XnVSlider2D::SetOffAxisDetectionAngle(XnFloat fAngle)
{
	m_fOffAxisDetectionAngle = fAngle;
}
void XnVSlider2D::SetOffAxisDetectionTime(XnUInt32 nTime)
{
	m_nOffAxisDetectionTime = nTime;
}
