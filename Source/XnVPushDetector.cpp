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
#include "XnVPushDetector.h"
#include "XnVPointBuffer.h"
#include "XnVNiteLog.h"
#include "XnVNiteEvents.h"

XnVPushDetector::XnVPushDetector(const XnChar* strName) :
	XnVPointControl(strName),
	m_bPushDetected(false),
	m_nPushImmediateOffset(0),
	m_nPushImmediateDuration(240),
	m_nPushPreviousOffset(240),	// ms
	m_nPushPreviousDuration(150),
	m_fPushImmediateMinVelocity(0.33f),	// (m/s)
	m_fPushPreviousMinVelocity(0.17f),	// (m/s)
	m_fPushMinAngleImmediateAndPrevious(20.0f),
	m_fPushMaxAngleFromZ(30.0f),
	m_fStableMaxVelocity(0.13f),	// Speed (in (m/s)) under which to consider stable
	m_nStableDuration(360) // ms
{
	m_pPoints = XN_NEW(XnVPointBuffer);

	m_pPushCBs = XN_NEW(XnVFloatFloatSpecificEvent);
	m_pStabilizedCBs = XN_NEW(XnVFloatSpecificEvent);
} // XnVPushDetector::XnVPushDetector

XnVPushDetector::~XnVPushDetector()
{
	XN_DELETE(m_pPoints);

	XN_DELETE(m_pPushCBs);
	XN_DELETE(m_pStabilizedCBs);
} // XnVPushDetector::~XnVPushDetector

XnUInt32 XnVPushDetector::GetPushDuration() const
{
	return m_nPushImmediateDuration;
} // XnVPushDetector::GetPushDuration

void XnVPushDetector::OnPrimaryPointCreate(const XnVHandPointContext* pContext, const XnPoint3D& ptSessionStarter)
{
	m_bPushDetected = false;
	Reset();
} // XnVPushDetector::OnInputStart

void XnVPushDetector::OnPrimaryPointUpdate(const XnVHandPointContext* pContext)
{
	AddPoint(pContext->ptPosition, pContext->fTime);
} // XnVPushDetector::OnInputUpdate

void XnVPushDetector::Reset()
{
	m_pPoints->Reset();
} // XnVPushDetector::Reset

XnCallbackHandle XnVPushDetector::RegisterPush(void* cxt, PushCB pCB)
{
	XnCallbackHandle handle;
	m_pPushCBs->Register(pCB, cxt, &handle);
	return handle;
} // XnVPushDetector::RegisterPush

XnCallbackHandle XnVPushDetector::RegisterStabilized(void* cxt, StabilizedCB pCB)
{
	XnCallbackHandle handle;
	m_pStabilizedCBs->Register(pCB, cxt, &handle);
	return handle;
} // XnVPushDetector::RegisterStabilized

void XnVPushDetector::UnregisterPush(XnCallbackHandle handle)
{
	m_pPushCBs->Unregister(handle);
} // XnVPushDetector::UnregisterPush

void XnVPushDetector::UnregisterStabilized(XnCallbackHandle handle)
{
	m_pStabilizedCBs->Unregister(handle);
} // XnVPushDetector::UnregisterStabilized

XnFloat XnVPushDetector::AngleBetweenVectors(const XnV3DVector& v1, const XnV3DVector& v2)
{
	XnFloat n1 = v1.Magnitude();
	XnFloat n2 = v2.Magnitude();

	if (n1 < v1.GetTolerance() || n2 < v2.GetTolerance())
	{
		return 0;
	}

	return acos((v1/n1)|(v2/n2)) * (180.0f/XnVMathCommon::PI);
} // XnVPushDetector::AngleBetweenVectors

XnBool XnVPushDetector::IsPushDetected(const XnV3DVector& vImmediateVelocity, const XnV3DVector& vPreviousVelocity, XnFloat& fZAngle)
{
	// Check if current motion is too slow
	if (vImmediateVelocity.Magnitude() < m_fPushImmediateMinVelocity)
	{
		return false;
	}

	if (vPreviousVelocity.Magnitude() < m_fPushPreviousMinVelocity)
	{
		fZAngle = AngleBetweenVectors(vImmediateVelocity, XnV3DVector(0,0,-1));

		if (fZAngle < m_fPushMaxAngleFromZ)
		{
			xnLogVerbose(XNV_NITE_MASK_EVENTS,	"Push Detector %s [0x%08x]: "
												"Immediate Velocity %5.3f over threshold %5.3f in the last %d ms (%d ms offset), "
												"Previous  Velocity %5.3f under threshold %5.3f in the last %d ms (%d ms offset), "
												"Angle between Immediate and the Z Axis is %5.3f, under the threshold of %5.3f",
												GetListenerName(), this,
												vImmediateVelocity.Magnitude(), m_fPushImmediateMinVelocity, m_nPushImmediateDuration, m_nPushImmediateOffset,
												vPreviousVelocity.Magnitude(), m_fPushPreviousMinVelocity, m_nPushPreviousDuration, m_nPushPreviousOffset,
												fZAngle, m_fPushMaxAngleFromZ);
			return true;
		}
		return false;
	}

	XnFloat fAngle = AngleBetweenVectors(vPreviousVelocity, vImmediateVelocity);
	if (fAngle > m_fPushMinAngleImmediateAndPrevious)
	{
		fZAngle = AngleBetweenVectors(vImmediateVelocity, XnV3DVector(0,0,-1));
		if (fZAngle < m_fPushMaxAngleFromZ)
		{
			xnLogVerbose(XNV_NITE_MASK_EVENTS,	"Push Detector %s [0x%08x]: "
				"Immediate Velocity %5.3f over threshold %5.3f in the last %d ms (%d ms offset), "
				"Previous  Velocity %5.3f over threshold %5.3f in the last %d ms (%d ms offset), "
				"Angle between Immediate and the Z Axis is %5.3f, under the threshold of %5.3f, "
				"Angle between Immediate and Previous direction is %5.3f, over the threshold of %5.3f",
				GetListenerName(), this,
				vImmediateVelocity.Magnitude(), m_fPushImmediateMinVelocity, m_nPushImmediateDuration, m_nPushImmediateOffset,
				vPreviousVelocity.Magnitude(), m_fPushPreviousMinVelocity, m_nPushPreviousDuration, m_nPushPreviousOffset,
				fZAngle, m_fPushMaxAngleFromZ,
				fAngle, m_fPushMinAngleImmediateAndPrevious);
			return true;
		}
	}

	return false;
} // XnVPushDetector::IsPushDetected

XnBool XnVPushDetector::IsStabilized(XnFloat fTime, XnFloat& fVelocity)
{
	XnV3DVector vVelocity = m_pPoints->GetAverageVelocityByTime(m_nStableDuration, fTime);
	fVelocity = vVelocity.Magnitude();

	// Have the limit squared, so comparison is against MagnitudeSquared, which is more efficient
	if (fVelocity < m_fStableMaxVelocity)
	{
		xnLogVerbose(XNV_NITE_MASK_EVENTS, "Push Detector %s [0x%08x]: Velocity %5.3f under velocity threshold %5.3f in the last %d ms",
			GetListenerName(), this, vVelocity.Magnitude(), m_fStableMaxVelocity, m_nStableDuration);
		return true;
	}

	return false;
} // XnVPushDetector::IsStabilized

void XnVPushDetector::UpdateLines(XnFloat fTime)
{
	XnV3DVector vImmediateVelocity = m_pPoints->GetAverageVelocityByTime(m_nPushImmediateDuration, fTime, m_nPushImmediateOffset);
	XnV3DVector vPreviousVelocity = m_pPoints->GetAverageVelocityByTime(m_nPushPreviousDuration, fTime, m_nPushPreviousOffset);

	if (!m_bPushDetected)
	{
		XnFloat fZAngle = 0;
		if (IsPushDetected(vImmediateVelocity, vPreviousVelocity, fZAngle))
		{
			m_bPushDetected = true;
			PushDetected(vImmediateVelocity.Magnitude(), fZAngle);
		}
	}
	else
	{
		XnFloat fVelocity;
		if (IsStabilized(fTime, fVelocity))
		{
			m_bPushDetected = false;
			StabilizedDetected(fVelocity);
		}
	}
} // XnVPushDetector::UpdateLines

void XnVPushDetector::AddPoint(const XnPoint3D& pt, XnFloat fTime)
{
	m_pPoints->AddPoint(pt, fTime);
	UpdateLines(fTime);
} // XnVPushDetector::AddPoint

// CBs
void XnVPushDetector::PushDetected(XnFloat fVelocity, XnFloat fAngle)
{
	m_pPushCBs->Raise(fVelocity, fAngle);
} // XnVPushDetector::PushDetected

void XnVPushDetector::StabilizedDetected(XnFloat fVelocity)
{
	m_pStabilizedCBs->Raise(fVelocity);
} // XnVPushDetector::StabilizedDetected

XnFloat XnVPushDetector::GetPushImmediateMinimumVelocity() const
{
	return m_fPushImmediateMinVelocity;
}
XnUInt32 XnVPushDetector::GetPushImmediateDuration() const
{
	return m_nPushImmediateDuration;
}
XnUInt32 XnVPushDetector::GetPushImmediateOffset() const
{
	return m_nPushImmediateOffset;
}
XnFloat XnVPushDetector::GetPushPreviousMinimumVelocity() const
{
	return m_fPushPreviousMinVelocity;
}
XnUInt32 XnVPushDetector::GetPushPreviousDuration() const
{
	return m_nPushPreviousDuration;
}
XnUInt32 XnVPushDetector::GetPushPreviousOffset() const
{
	return m_nPushPreviousOffset;
}

XnFloat XnVPushDetector::GetPushMaximumAngleBetweenImmediateAndZ() const
{
	return m_fPushMaxAngleFromZ;
}
XnFloat XnVPushDetector::GetPushMinimumAngleBetweenImmediateAndPrevious() const
{
	return m_fPushMinAngleImmediateAndPrevious;
}

XnFloat XnVPushDetector::GetStableMaximumVelocity() const
{
	return m_fStableMaxVelocity;
}
XnUInt32 XnVPushDetector::GetStableDuration() const
{
	return m_nStableDuration;
}

void XnVPushDetector::SetPushImmediateMinimumVelocity(XnFloat fVelocity)
{
	XnAutoCSLocker local(m_hListenerCS);
	m_fPushImmediateMinVelocity = fVelocity;
}
void XnVPushDetector::SetPushImmediateDuration(XnUInt32 nDuration)
{
	XnAutoCSLocker local(m_hListenerCS);
	m_nPushImmediateDuration = nDuration;
}
void XnVPushDetector::SetPushImmediateOffset(XnUInt32 nOffset)
{
	XnAutoCSLocker local(m_hListenerCS);
	m_nPushImmediateOffset = nOffset;
}

void XnVPushDetector::SetPushPreviousMinimumVelocity(XnFloat fVelocity)
{
	XnAutoCSLocker local(m_hListenerCS);
	m_fPushPreviousMinVelocity = fVelocity;
}
void XnVPushDetector::SetPushPreviousDuration(XnUInt32 nDuration)
{
	XnAutoCSLocker local(m_hListenerCS);
	m_nPushPreviousDuration = nDuration;
}
void XnVPushDetector::SetPushPreviousOffset(XnUInt32 nOffset)
{
	XnAutoCSLocker local(m_hListenerCS);
	m_nPushPreviousOffset = nOffset;
}

void XnVPushDetector::SetPushMaximumAngleBetweenImmediateAndZ(XnFloat fAngle)
{
	XnAutoCSLocker local(m_hListenerCS);
	m_fPushMaxAngleFromZ = fAngle;
}
void XnVPushDetector::SetPushMinimumAngleBetweenImmediateAndPrevious(XnFloat fAngle)
{
	XnAutoCSLocker local(m_hListenerCS);
	m_fPushMinAngleImmediateAndPrevious = fAngle;
}

void XnVPushDetector::SetStableMaximumVelocity(XnFloat fVelocity)
{
	XnAutoCSLocker local(m_hListenerCS);
	m_fStableMaxVelocity = fVelocity;
}
void XnVPushDetector::SetStableDuration(XnUInt32 nDuration)
{
	XnAutoCSLocker local(m_hListenerCS);
	m_nStableDuration = nDuration;
}
