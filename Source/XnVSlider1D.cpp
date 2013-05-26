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
#include <XnVSlider1D.h>
#include "XnVPointBuffer.h"
#include "XnVMathCommon.h"
#include "XnVNiteEvents.h"

const XnFloat XnVSlider1D::ms_fDefaultMinimumOffAxisVelocity = 0.15f;
const XnFloat XnVSlider1D::ms_fOffAxisDefaultDetectionAngle = 60.0f;
const XnUInt32 XnVSlider1D::ms_nDefaultTimeForOffAxisDetection = 350;

XnVSlider1D::XnVSlider1D(XnVAxis eAxis, const XnPoint3D& ptInitialPosition, XnPoint3D ptMinPoint, XnPoint3D ptMaxPoint,
						 XnFloat fMinOutput, XnFloat fMaxOutput, XnFloat fOffAxisDetectionAngle,
						 XnFloat fOffAxisDetectionMinimumVelocity) :
	m_pPointBuffer(NULL)
{
	m_pValueChangeCBs = XN_NEW(XnVFloatSpecificEvent);
	m_pOffAxisMovementCBs = XN_NEW(XnVDirectionSpecificEvent);

	XnFloat fSliderLength;
	XnFloat fInitialValue;
	m_nOffAxisDetectionTime = ms_nDefaultTimeForOffAxisDetection;

	InitializeFromPoint(ptInitialPosition, ptMinPoint, ptMaxPoint, eAxis, fSliderLength, fInitialValue);

	// All absolute sliders are not draggable!
	Initialize(eAxis, false, ptInitialPosition, fSliderLength, fInitialValue, fMinOutput, fMaxOutput,
		fOffAxisDetectionAngle, fOffAxisDetectionMinimumVelocity);
} // XnVSlider1D::XnVSlider1D

// No "Draggable" compatibility constructors
XnVSlider1D::XnVSlider1D(XnVAxis eAxis, const XnPoint3D& ptInitialPosition, XnFloat fSliderLength,
						 XnFloat fInitialValue, XnFloat fMinOutput, XnFloat fMaxOutput,
						 XnFloat fOffAxisDetectionAngle, XnFloat fOffAxisDetectionMinimumVelocity) :
	m_pPointBuffer(NULL)
{
	m_pValueChangeCBs = XN_NEW(XnVFloatSpecificEvent);
	m_pOffAxisMovementCBs = XN_NEW(XnVDirectionSpecificEvent);

	m_nOffAxisDetectionTime = ms_nDefaultTimeForOffAxisDetection;

	Initialize(eAxis, false, ptInitialPosition, fSliderLength, fInitialValue, fMinOutput, fMaxOutput,
		fOffAxisDetectionAngle, fOffAxisDetectionMinimumVelocity);
} // XnVSlider1D::XnVSlider1D

// Normal ctors
XnVSlider1D::XnVSlider1D(XnVAxis eAxis, XnBool bDraggable, const XnPoint3D& ptInitialPosition,
						 XnFloat fSliderLength, XnFloat fInitialValue, XnFloat fMinOutput, XnFloat fMaxOutput,
						 XnFloat fOffAxisDetectionAngle, XnFloat fOffAxisDetectionMinimumVelocity) :
	m_pPointBuffer(NULL)
{
	m_pValueChangeCBs = XN_NEW(XnVFloatSpecificEvent);
	m_pOffAxisMovementCBs = XN_NEW(XnVDirectionSpecificEvent);

	m_nOffAxisDetectionTime = ms_nDefaultTimeForOffAxisDetection;

	Initialize(eAxis, bDraggable, ptInitialPosition, fSliderLength, fInitialValue, fMinOutput, fMaxOutput,
		fOffAxisDetectionAngle, fOffAxisDetectionMinimumVelocity);
} // XnVSlider1D::XnVSlider1D

XnVSlider1D::~XnVSlider1D()
{
	if (m_pPointBuffer != NULL)
	{
		XN_DELETE(m_pPointBuffer);
	}

	XN_DELETE(m_pValueChangeCBs);
	XN_DELETE(m_pOffAxisMovementCBs);

} // XnVSlider1D::~XnVSlider1D

void XnVSlider1D::Reinitialize(XnVAxis eAxis, const XnPoint3D& ptInitialPoint, const XnPoint3D& ptMinPoint,
							   const XnPoint3D& ptMaxPoint, XnFloat fMinOutput, XnFloat fMaxOutput)
{
	XnFloat fSliderLength;
	XnFloat fInitialValue;

	InitializeFromPoint(ptInitialPoint, ptMinPoint, ptMaxPoint, eAxis, fSliderLength, fInitialValue);

	// All absolute sliders are not draggable!
	Initialize(eAxis, false, ptInitialPoint, fSliderLength, fInitialValue, fMinOutput, fMaxOutput,
		ms_fOffAxisDefaultDetectionAngle, ms_fDefaultMinimumOffAxisVelocity);
} // XnVSlider1D::Reinitialize

void XnVSlider1D::Reinitialize(XnVAxis eAxis, const XnPoint3D& ptInitialPoint, XnFloat fSliderLength,
							   XnFloat fInitialValue, XnFloat fMinOutput, XnFloat fMaxOutput)
{
	Initialize(eAxis, false, ptInitialPoint, fSliderLength, fInitialValue, fMinOutput, fMaxOutput,
		ms_fOffAxisDefaultDetectionAngle, ms_fDefaultMinimumOffAxisVelocity);
} // XnVSlider1D::Reinitialize

void XnVSlider1D::InitializeFromPoint(const XnPoint3D& ptInitialPosition, const XnPoint3D& ptMinPoint,
									  const XnPoint3D& ptMaxPoint, XnBool eAxis, XnFloat& fSliderLength,
									  XnFloat& fInitialValue)
{
	XnFloat fInitialMajorAxisPosition;
	switch (eAxis)
	{
	case AXIS_X:
		fSliderLength = ptMaxPoint.X - ptMinPoint.X;
		fInitialMajorAxisPosition = ptInitialPosition.X - ptMinPoint.X;
		break;
	case AXIS_Y:
		fSliderLength = ptMaxPoint.Y - ptMinPoint.Y;
		fInitialMajorAxisPosition = ptInitialPosition.Y - ptMinPoint.Y;
		break;
	case AXIS_Z:
		fSliderLength = ptMaxPoint.Z - ptMinPoint.Z;
		fInitialMajorAxisPosition = ptInitialPosition.Z - ptMinPoint.Z;
		break;
	default:
		return;
	}

	fInitialValue = fInitialMajorAxisPosition / fSliderLength;
} // XnVSlider1D::InitializeFromPoint

XnFloat XnVSlider1D::ValueAtPosition(const XnPoint3D& pt)
{
	XnFloat fMajorAxisPosition;
	switch (m_eAxis)
	{
	case AXIS_X:
		fMajorAxisPosition = pt.X; break;
	case AXIS_Y:
		fMajorAxisPosition = pt.Y; break;
	case AXIS_Z:
		fMajorAxisPosition = pt.Z; break;
	default:
		return -1;
	}

	XnFloat fRelativePosition = (fMajorAxisPosition - m_fMinOutputMajorAxisPosition)
		/ (m_fMaxOutputMajorAxisPosition - m_fMinOutputMajorAxisPosition);
	XnFloat fTempOutput = m_fOutputMinimum + ((m_fOutputMaximum - m_fOutputMinimum) * fRelativePosition);

	return XN_MAX(m_fOutputMinimum, XN_MIN(m_fOutputMaximum, fTempOutput));
} // XnVSlider1D::ValueAtPosition

// Value Change
XnCallbackHandle XnVSlider1D::RegisterValueChange(void* cxt, ValueChangeCB pCB)
{
	XnCallbackHandle handle;
	m_pValueChangeCBs->Register(pCB, cxt, &handle);	
	return handle;
} // XnVSlider1D::RegisterValueChange

void XnVSlider1D::UnregisterValueChange(XnCallbackHandle handle)
{
	m_pValueChangeCBs->Unregister(handle);
} // XnVSlider1D::UnregisterValueChange

// OffAxisMovement
XnCallbackHandle XnVSlider1D::RegisterOffAxisMovement(void* cxt, OffAxisMovementCB pCB)
{
	XnCallbackHandle handle;
	m_pOffAxisMovementCBs->Register(pCB, cxt, &handle);
	return handle;
} // XnVSlider1D::RegisterOffAxisMovement
void XnVSlider1D::UnregisterOffAxisMovement(XnCallbackHandle handle)
{
	m_pOffAxisMovementCBs->Unregister(handle);
} // XnVSlider1D::UnregisterOffAxisMovement

void XnVSlider1D::ValueChange(XnFloat nValue) // 0..1
{
	m_pValueChangeCBs->Raise(nValue);
} // XnVSlider1D::ValueChange
void XnVSlider1D::OffAxisMovement(XnVDirection eDir)
{
	m_pOffAxisMovementCBs->Raise(eDir);
} // XnVSlider1D::OffAxisMovement

XnStatus XnVSlider1D::Update(const XnPoint3D& pt, XnFloat fTime, XnBool bCheckOffAxis)
{
	if (m_pPointBuffer == NULL)
	{
		return XN_STATUS_NOT_INIT;
	}

	m_pPointBuffer->AddPoint(pt, fTime);

	if (bCheckOffAxis)
	{
		XnVDirection eDir = CheckForOffAxisMovement(pt, fTime);

		if (eDir != DIRECTION_ILLEGAL)
		{
			// Set position and value to "Last Valid"
			// TODO: Do in a non-crappy way
			m_ptCurrentPosition = m_pPointBuffer->GetAveragePointByTime(m_nOffAxisDetectionTime, fTime, 1);
			m_fCurrentOutput = ValueAtPosition(m_ptCurrentPosition);

			OffAxisMovement(eDir);

			// don't update values
			return XN_STATUS_OK;
		}
	}
	m_ptCurrentPosition = pt;

	// calculate the current value
	// normalize the on-axis movement according to this scale:

	//TODO: Off-axis motion detection

	XnFloat fMajorAxisPosition;
	switch (m_eAxis)
	{
	case AXIS_X:
		fMajorAxisPosition = pt.X; break;
	case AXIS_Y:
		fMajorAxisPosition = pt.Y; break;
	case AXIS_Z:
		fMajorAxisPosition = pt.Z; break;
	default:
		return XN_STATUS_NITE_UNEXPECTED_DIRECTION;
	}

	XnFloat fRelativePosition = (fMajorAxisPosition - m_fMinOutputMajorAxisPosition)
		/ (m_fMaxOutputMajorAxisPosition - m_fMinOutputMajorAxisPosition);
	XnFloat fTempOutput = m_fOutputMinimum + ((m_fOutputMaximum - m_fOutputMinimum) * fRelativePosition);

	XnFloat fPreviousOutput = m_fCurrentOutput;

	// Handle dragging if enabled
	if ((fMajorAxisPosition > m_fMaxOutputMajorAxisPosition) && m_bIsDraggable)
	{
		XnFloat fPositionDelta = fMajorAxisPosition - m_fMaxOutputMajorAxisPosition;
		m_fMaxOutputMajorAxisPosition = fMajorAxisPosition;
		m_fMinOutputMajorAxisPosition += fPositionDelta;
	}
	else
	{
		if ((fMajorAxisPosition < m_fMinOutputMajorAxisPosition) && m_bIsDraggable)
		{
			XnFloat fPositionDelta = m_fMinOutputMajorAxisPosition - fMajorAxisPosition;
			m_fMaxOutputMajorAxisPosition = fMajorAxisPosition;
			m_fMaxOutputMajorAxisPosition -= fPositionDelta;
		}
	}

	m_fCurrentOutput = XN_MAX(m_fOutputMinimum, XN_MIN(m_fOutputMaximum, fTempOutput));

	ValueChange(m_fCurrentOutput);

	return XN_STATUS_OK;
} // XnVSlider1D::Update

void XnVSlider1D::LostPoint()
{
	m_pPointBuffer->Reset();
} // XnVSlider1D::LostPoint

void XnVSlider1D::Initialize(XnVAxis eAxis, XnBool bDraggable, const XnPoint3D& ptInitialPosition,
							 XnFloat fSliderLength, XnFloat fInitialValue, XnFloat fMinOutput,
							 XnFloat fMaxOutput, XnFloat fOffAxisDetectionAngle,
							 XnFloat fOffAxisDetectionMinimumVelocity)
{
	m_bIsDraggable = bDraggable;
	if (m_pPointBuffer == NULL)
	{
		m_pPointBuffer = XN_NEW(XnVPointBuffer, 100);
	}
	else
	{
		m_pPointBuffer->Reset();
	}

	m_fOffAxisDetectionAngle = fOffAxisDetectionAngle;
	m_fOffAxisDetectionMinimumVelocity = fOffAxisDetectionMinimumVelocity;

	m_eAxis = eAxis;
	m_ptCurrentPosition = ptInitialPosition;
	m_fCurrentOutput = fInitialValue;

	m_fOutputMaximum = fMaxOutput;
	m_fOutputMinimum = fMinOutput;

	switch (m_eAxis)
	{
	case AXIS_X:
		m_fMinOutputMajorAxisPosition = ptInitialPosition.X - fSliderLength * (fInitialValue - fMinOutput) / (fMaxOutput - fMinOutput);
		m_fMaxOutputMajorAxisPosition = ptInitialPosition.X + fSliderLength * (fMaxOutput - fInitialValue) / (fMaxOutput - fMinOutput);
		break;
	case AXIS_Y:
		m_fMinOutputMajorAxisPosition = ptInitialPosition.Y - fSliderLength * (fInitialValue - fMinOutput) / (fMaxOutput - fMinOutput);
		m_fMaxOutputMajorAxisPosition = ptInitialPosition.Y + fSliderLength * (fMaxOutput - fInitialValue) / (fMaxOutput - fMinOutput);
		break;
	case AXIS_Z:
		m_fMinOutputMajorAxisPosition = ptInitialPosition.Z - fSliderLength * (fInitialValue - fMinOutput) / (fMaxOutput - fMinOutput);
		m_fMaxOutputMajorAxisPosition = ptInitialPosition.Z + fSliderLength * (fMaxOutput - fInitialValue) / (fMaxOutput - fMinOutput);
		break;
	default:
		return;
	}

	// clamp current output to valid range - it may not have been valid till now
	m_fCurrentOutput = XN_MIN(XN_MAX(m_fCurrentOutput, m_fOutputMinimum), m_fOutputMaximum);
} // XnVSlider1D::Initialize

XnFloat XnVSlider1D::GetValue() const
{
	return m_fCurrentOutput;
} // XnVSlider1D::GetValue

const XnPoint3D& XnVSlider1D::GetPosition() const
{
	return m_ptCurrentPosition;
} // XnVSlider1D::GetPosition

XnBool XnVSlider1D::IsValid() const
{
	return true;
} // XnVSlider1D::IsValid

XnVAxis XnVSlider1D::GetAxis() const
{
	return m_eAxis;
} // XnVSlider1D::GetAxis


XnInt32 XnVSlider1D::CheckOffAxis(XnFloat fPrimaryAxisDelta, XnFloat fOffAxisDelta)
{
	if (fabs(fOffAxisDelta) > m_fOffAxisDetectionMinimumVelocity)
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
} // XnVSlider1D::CheckOffAxis

XnVDirection XnVSlider1D::CheckForOffAxisMovement(const XnPoint3D& pt, XnFloat fTime)
{
	if (m_pPointBuffer->GetAvailableTimespan() < m_nOffAxisDetectionTime)
	{
		return DIRECTION_ILLEGAL;
	}

	XnPoint3D ptAverageVelocity = m_pPointBuffer->GetAverageVelocityByTime(m_nOffAxisDetectionTime, fTime);

	XnInt32 nOffAxis;
	switch (m_eAxis)
	{
	case AXIS_X:
		nOffAxis = CheckOffAxis(ptAverageVelocity.X, ptAverageVelocity.Y);
		if (nOffAxis < 0)
		{
			return DIRECTION_DOWN;
		}
		else if (nOffAxis > 0)
		{
			return DIRECTION_UP;
		}
		nOffAxis = CheckOffAxis(ptAverageVelocity.X, ptAverageVelocity.Z);
		if (nOffAxis < 0)
		{
			return DIRECTION_FORWARD;
		}
		else if (nOffAxis > 0)
		{
			return DIRECTION_BACKWARD;
		}
		break;

	case AXIS_Y:
		nOffAxis = CheckOffAxis(ptAverageVelocity.Y, ptAverageVelocity.X);
		if (nOffAxis < 0)
		{
			return DIRECTION_LEFT;
		}
		else if (nOffAxis > 0)
		{
			return DIRECTION_RIGHT;
		}
		nOffAxis = CheckOffAxis(ptAverageVelocity.Y, ptAverageVelocity.Z);
		if (nOffAxis < 0)
		{
			return DIRECTION_FORWARD;
		}
		else if (nOffAxis > 0)
		{
			return DIRECTION_BACKWARD;
		}
		break;
	case AXIS_Z:
		nOffAxis = CheckOffAxis(ptAverageVelocity.Z, ptAverageVelocity.X);
		if (nOffAxis < 0)
		{
			return DIRECTION_LEFT;
		}
		else if (nOffAxis > 0)
		{
			return DIRECTION_RIGHT;
		}
		nOffAxis = CheckOffAxis(ptAverageVelocity.Z, ptAverageVelocity.Y);
		if (nOffAxis < 0)
		{
			return DIRECTION_DOWN;
		}
		else if (nOffAxis > 0)
		{
			return DIRECTION_UP;
		}
		break;
	default:
		return DIRECTION_ILLEGAL;
	} // switch (m_eAxis)

	// if we're here, it means that the thresholds weren't crossed
	return DIRECTION_ILLEGAL;
} // XnVSlider1D::CheckForOffAxisMovement

XnFloat  XnVSlider1D::GetOffAxisDetectionVelocity() const
{
	return m_fOffAxisDetectionMinimumVelocity;
}
XnFloat  XnVSlider1D::GetOffAxisDetectionAngle() const
{
	return m_fOffAxisDetectionAngle;
}
XnUInt32 XnVSlider1D::GetOffAxisDetectionTime() const
{
	return m_nOffAxisDetectionTime;
}

void XnVSlider1D::SetOffAxisDetectionVelocity(XnFloat fVelocity)
{
	m_fOffAxisDetectionMinimumVelocity = fVelocity;
}
void XnVSlider1D::SetOffAxisDetectionAngle(XnFloat fAngle)
{
	m_fOffAxisDetectionAngle = fAngle;
}
void XnVSlider1D::SetOffAxisDetectionTime(XnUInt32 nTime)
{
	m_nOffAxisDetectionTime = nTime;
}
