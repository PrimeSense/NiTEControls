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
#include <XnVSwipeDetector.h>
#include <XnVHandPointContext.h>
#include "XnVPointBuffer.h"
#include "XnVNiteLog.h"
#include "XnVNiteStatus.h"
#include "XnVNiteEvents.h"
#include "XnV3DVector.h"

XN_DECLARE_EVENT_3ARG(XnVGeneralSwipeSpecificEvent, XnVGeneralSwipeEvent, XnVDirection, eDir, XnFloat, fVelocity, XnFloat, fAngle);


const XnFloat XnVSwipeDetector::ms_fDefaultMotionDetectionVelocity = 0.25f;
const XnUInt32 XnVSwipeDetector::ms_nDefaultMotionDetectionDuration = 350;

XnVSwipeDetector::XnVSwipeDetector(XnBool bUseSteady, const XnChar* strName) :
	XnVPointControl(strName),
	m_fMotionDetectionSpeed(ms_fDefaultMotionDetectionVelocity),
	m_nMotionDetectionTime(ms_nDefaultMotionDetectionDuration),
	m_pPendingEvent(NULL),
	m_ePendingDirection(DIRECTION_ILLEGAL),
	m_bInSteady(bUseSteady),
	m_bUseSteady(bUseSteady),
	m_bWaitingForSlowdown(false),
	m_fAngleYThreshold(20.0),
	m_fAngleXThreshold(25.0),
	m_Steady(XnVSteadyDetector::ms_nDefaultInitialCooldown, XnVSteadyDetector::ms_nDefaultDetectionDuration, XnVSteadyDetector::ms_fDefaultMaximumStdDevForSteady, "XnVSwipeDetector's internal Steady Detector")
{
	m_pMovementDetectionBuffer = XN_NEW(XnVPointBuffer);
	m_Steady.RegisterSteady(this, &Steady_Steady);

	m_pSwipeCBs = XN_NEW(XnVGeneralSwipeSpecificEvent);
	m_pSwipeUpCBs = XN_NEW(XnVFloatFloatSpecificEvent);
	m_pSwipeDownCBs = XN_NEW(XnVFloatFloatSpecificEvent);
	m_pSwipeLeftCBs = XN_NEW(XnVFloatFloatSpecificEvent);
	m_pSwipeRightCBs = XN_NEW(XnVFloatFloatSpecificEvent);
} // XnVSwipeDetector::XnVSwipeDetector

XnVSwipeDetector::~XnVSwipeDetector()
{
	XN_DELETE(m_pMovementDetectionBuffer);

	XN_DELETE(m_pSwipeCBs);
	XN_DELETE(m_pSwipeUpCBs);
	XN_DELETE(m_pSwipeDownCBs);
	XN_DELETE(m_pSwipeLeftCBs);
	XN_DELETE(m_pSwipeRightCBs);
} // XnVSwipeDetector::~XnVSwipeDetector

void XnVSwipeDetector::OnPrimaryPointCreate(const XnVHandPointContext* pContext, const XnPoint3D& ptFocus)
{
	if (m_bUseSteady)
	{
		m_bInSteady = true;
		m_Steady.Reset();
		m_Steady.OnPointCreate(pContext);
	}
	else
	{
		Reset();
		AddPoint(pContext->ptPosition, pContext->fTime);
	}
} // XnVSwipeDetector::OnPrimaryPointCreate

void XnVSwipeDetector::OnPrimaryPointUpdate(const XnVHandPointContext* pContext)
{
	if (m_bInSteady)
	{
		m_Steady.OnPointUpdate(pContext);
	}
	else
	{
		AddPoint(pContext->ptPosition, pContext->fTime);
	}
} // XnVSwipeDetector::OnPrimaryPointUpdate

void XnVSwipeDetector::OnPrimaryPointReplace(XnUInt32 nOldId, const XnVHandPointContext* pContext)
{
	if (m_bInSteady)
	{
		m_Steady.OnPointDestroy(nOldId);
		m_Steady.OnPointCreate(pContext);
	}
}
void XnVSwipeDetector::OnPrimaryPointDestroy(XnUInt32 nID)
{
	if (m_bInSteady)
	{
		m_Steady.OnPointDestroy(nID);
	}
}

void XnVSwipeDetector::Reset()
{
	XnAutoCSLocker local(m_hListenerCS);
	m_pMovementDetectionBuffer->Reset();
	m_pPendingEvent = NULL;
	m_ePendingDirection = DIRECTION_ILLEGAL;
} // XnVSwipeDetector::Reset

XnStatus XnVSwipeDetector::AddPoint(const XnPoint3D& pt, XnFloat fTime)
{
	m_pMovementDetectionBuffer->AddPoint(pt, fTime);
	if (m_pMovementDetectionBuffer->GetAvailableTimespan() < m_nMotionDetectionTime)
	{
		return XN_STATUS_NITE_NOT_ENOUGH_TIME;
	}

	XnV3DVector currentVelocity(m_pMovementDetectionBuffer->GetAverageVelocityByTime(m_nMotionDetectionTime, fTime));

	if (m_pPendingEvent != NULL)
	{
		XnFloat fMotionVelocity = currentVelocity.Magnitude();
		// we will wait for the final slowdown once our velocity is below
		// a certain threshold
		if (m_bWaitingForSlowdown)
		{
			if (fMotionVelocity <= m_fLowestVelocity)
			{
				m_fLowestVelocity = fMotionVelocity;
			}
			else
			{
				m_pPendingEvent->Raise(m_fPendingVelocity, m_fPendingAngle);
				m_pSwipeCBs->Raise(m_ePendingDirection, m_fPendingVelocity, m_fPendingAngle);
				m_pPendingEvent = NULL;
				m_ePendingDirection = DIRECTION_ILLEGAL;
				m_bWaitingForSlowdown = false;
				if (m_bUseSteady)
				{
					m_bInSteady = true;
					m_Steady.Reset();
				}
			}
		}
		else
		{
			m_bWaitingForSlowdown = true;
			m_fLowestVelocity = fMotionVelocity;
		} // if (m_bWaitingForSlowdown) else

		return XN_STATUS_OK;
	} // if (m_pPendingEvent != NULL)

	currentVelocity.Z = 0;
	XnFloat fMotionVelocity = currentVelocity.Magnitude();
	currentVelocity.Normalize();

	if (fMotionVelocity >= m_fMotionDetectionSpeed)
	{
		m_fPendingVelocity = fMotionVelocity;
		XnFloat fHowCloseToX = acos(DotProduct(currentVelocity, XnV3DVector(1, 0, 0))) * (180.0f/XnVMathCommon::PI);
		XnFloat fHowCloseToY = acos(DotProduct(currentVelocity, XnV3DVector(0, 1, 0))) * (180.0f/XnVMathCommon::PI);

		if (fabs(fabs(fHowCloseToX) - 180) < m_fAngleXThreshold)
		{
			m_pPendingEvent = m_pSwipeLeftCBs;
			m_ePendingDirection = DIRECTION_LEFT;
			m_fPendingAngle = fHowCloseToX;
			xnLogVerbose(XNV_NITE_MASK_EVENTS, "Swipe Detector %s [0x%08x]: Motion Velocity %5.2f m/s over threshold %5.2f in the last %d ms. Angle from X axis is %5.2f, within range [%5.2f-%5.2f] as LEFT",
				GetListenerName(), this,
				fMotionVelocity, m_fMotionDetectionSpeed, m_nMotionDetectionTime,
				fabs(fHowCloseToX), 180-m_fAngleXThreshold, 180+m_fAngleXThreshold);
		}

		if (fabs(fHowCloseToX) < m_fAngleXThreshold)
		{
			m_pPendingEvent = m_pSwipeRightCBs;
			m_ePendingDirection = DIRECTION_RIGHT;
			m_fPendingAngle = fHowCloseToX;
			xnLogVerbose(XNV_NITE_MASK_EVENTS, "Swipe Detector %s [0x%08x]: Motion Velocity %5.2f m/s over threshold %5.2f in the last %d ms. Angle from X axis is %5.2f, within range [%5.2f-%5.2f] as RIGHT",
				GetListenerName(), this,
				fMotionVelocity, m_fMotionDetectionSpeed, m_nMotionDetectionTime,
				fHowCloseToX, -m_fAngleXThreshold, m_fAngleXThreshold);
		}

		if (fabs(fabs(fHowCloseToY) - 180) < m_fAngleYThreshold)
		{
			m_pPendingEvent = m_pSwipeDownCBs;
			m_ePendingDirection = DIRECTION_DOWN;
			m_fPendingAngle = fHowCloseToY;
			xnLogVerbose(XNV_NITE_MASK_EVENTS, "Swipe Detector %s [0x%08x]: Motion Velocity %5.2f m/s over threshold %5.2f in the last %d ms. Angle from Y axis is %5.2f, within range [%5.2f-%5.2f] as DOWN",
				GetListenerName(), this,
				fMotionVelocity, m_fMotionDetectionSpeed, m_nMotionDetectionTime,
				fabs(fHowCloseToY), 180-m_fAngleYThreshold, 180+m_fAngleYThreshold);
		}

		if (fabs(fHowCloseToY) < m_fAngleYThreshold)
		{
			m_pPendingEvent = m_pSwipeUpCBs;
			m_ePendingDirection = DIRECTION_UP;
			m_fPendingAngle = fHowCloseToY;
			xnLogVerbose(XNV_NITE_MASK_EVENTS, "Swipe Detector %s [0x%08x]: Motion Velocity %5.2f m/s over threshold %5.2f in the last %d ms. Angle from Y axis is %5.2f, within range [%5.2f-%5.2f] as UP",
				GetListenerName(), this,
				fMotionVelocity, m_fMotionDetectionSpeed, m_nMotionDetectionTime,
				fHowCloseToY, -m_fAngleYThreshold, m_fAngleYThreshold);
		}
	} // if (fMotionVelocity >= m_fMotionDetectionSpeed)

	return XN_STATUS_OK;
} // XnVSwipeDetector::AddPoint

void XnVSwipeDetector::SetMotionSpeedThreshold(XnFloat fThreshold)
{
	XnAutoCSLocker local(m_hListenerCS);
	m_fMotionDetectionSpeed = fThreshold;
} // XnVSwipeDetector::SetMotionSpeedThreshold

XnFloat XnVSwipeDetector::GetMotionSpeedThreshold() const
{
	return m_fMotionDetectionSpeed;
} // XnVSwipeDetector::GetMotionSpeedThreshold

void XnVSwipeDetector::SetMotionTime(XnUInt32 nThreshold)
{
	XnAutoCSLocker local(m_hListenerCS);
	m_nMotionDetectionTime = nThreshold;
} // XnVSwipeDetector::SetMotionTime

XnUInt32 XnVSwipeDetector::GetMotionTime() const
{
	return m_nMotionDetectionTime;
} // XnVSwipeDetector::GetMotionTime

void XnVSwipeDetector::SetXAngleThreshold(XnFloat fThreshold)
{
	XnAutoCSLocker local(m_hListenerCS);
	m_fAngleXThreshold = fThreshold;
}
XnFloat XnVSwipeDetector::GetXAngleThreshold() const
{
	return m_fAngleXThreshold;
}

void XnVSwipeDetector::SetYAngleThreshold(XnFloat fThreshold)
{
	XnAutoCSLocker local(m_hListenerCS);
	m_fAngleYThreshold = fThreshold;
}
XnFloat XnVSwipeDetector::GetYAngleThreshold() const
{
	return m_fAngleYThreshold;
}

void XnVSwipeDetector::SetSteadyMaxStdDev(XnFloat fStdDev)
{
	XnAutoCSLocker local(m_hListenerCS);
	m_Steady.SetMaximumStdDevForSteady(fStdDev);
}
XnFloat XnVSwipeDetector::GetSteadyMaxStdDev() const
{
	return m_Steady.GetMaximumStdDevForSteady();
}

void XnVSwipeDetector::SetSteadyDuration(XnUInt32 nDuration)
{
	XnAutoCSLocker local(m_hListenerCS);
	m_Steady.SetDetectionDuration(nDuration);
}
XnUInt32 XnVSwipeDetector::GetSteadyDuration() const
{
	return m_Steady.GetDetectionDuration();
}

void XnVSwipeDetector::SetUseSteady(XnBool bUse)
{
	XnAutoCSLocker local(m_hListenerCS);
	m_bUseSteady = bUse;
}

XnBool XnVSwipeDetector::GetUseSteady() const
{
	return m_bUseSteady;
}

XnCallbackHandle XnVSwipeDetector::RegisterSwipeUp(void* cxt, XnVSwipeDetector::SwipeCB pCB)
{
	XnCallbackHandle handle;
	m_pSwipeUpCBs->Register(pCB, cxt, &handle);
	return handle;
} // XnVSwipeDetector::RegisterSwipeUp

XnCallbackHandle XnVSwipeDetector::RegisterSwipeDown(void* cxt, XnVSwipeDetector::SwipeCB pCB)
{
	XnCallbackHandle handle;
	m_pSwipeDownCBs->Register(pCB, cxt, &handle);
	return handle;
} // XnVSwipeDetector::RegisterSwipeDown

XnCallbackHandle XnVSwipeDetector::RegisterSwipeLeft(void* cxt, XnVSwipeDetector::SwipeCB pCB)
{
	XnCallbackHandle handle;
	m_pSwipeLeftCBs->Register(pCB, cxt, &handle);
	return handle;
} // XnVSwipeDetector::RegisterSwipeLeft

XnCallbackHandle XnVSwipeDetector::RegisterSwipeRight(void* cxt, XnVSwipeDetector::SwipeCB pCB)
{
	XnCallbackHandle handle;
	m_pSwipeRightCBs->Register(pCB, cxt, &handle);
	return handle;
} // XnVSwipeDetector::RegisterSwipeRight

XnCallbackHandle XnVSwipeDetector::RegisterSwipe(void* cxt, XnVSwipeDetector::GeneralSwipeCB pCB)
{
	XnCallbackHandle handle;
	m_pSwipeCBs->Register(pCB, cxt, &handle);
	return handle;
} // XnVSwipeDetector::RegisterSwipe

void XnVSwipeDetector::UnregisterSwipeUp(XnCallbackHandle handle)
{
	m_pSwipeUpCBs->Unregister(handle);
} // XnVSwipeDetector::UnregisterSwipeUp

void XnVSwipeDetector::UnregisterSwipeDown(XnCallbackHandle handle)
{
	m_pSwipeDownCBs->Unregister(handle);
} // XnVSwipeDetector::UnregisterSwipeDown

void XnVSwipeDetector::UnregisterSwipeLeft(XnCallbackHandle handle)
{
	m_pSwipeLeftCBs->Unregister(handle);
} // XnVSwipeDetector::UnregisterSwipeLeft

void XnVSwipeDetector::UnregisterSwipeRight(XnCallbackHandle handle)
{
	m_pSwipeRightCBs->Unregister(handle);
} // XnVSwipeDetector::UnregisterSwipeRight

void XnVSwipeDetector::UnregisterSwipe(XnCallbackHandle handle)
{
	m_pSwipeCBs->Unregister(handle);
} // XnVSwipeDetector::UnregisterSwipe

void XN_CALLBACK_TYPE XnVSwipeDetector::Steady_Steady(XnUInt32 nId, XnFloat fVelocity, void* cxt)
{
	XnVSwipeDetector* pSwipeDetector = (XnVSwipeDetector*)cxt;

	if (nId != pSwipeDetector->GetPrimaryID())
	{
		return;
	}

	pSwipeDetector->m_bInSteady = false;
	pSwipeDetector->m_Steady.OnPointDestroy(nId);
	pSwipeDetector->Reset();
}

// Deprecated
XnFloat XnVSwipeDetector::GetSteadyMaxVelocity() const
{
	return GetSteadyMaxStdDev();
}
void XnVSwipeDetector::SetSteadyMaxVelocity(XnFloat fVelocity)
{
	SetSteadyMaxStdDev(fVelocity);
}