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
#include <XnVSteadyDetector.h>
#include <XnVHandPointContext.h>
#include "XnVNiteLog.h"
#include "XnVPointBuffer.h"
#include "XnVNiteStatus.h"
#include "XnVNiteEvents.h"

struct XnVSteadyState
{
	XnUInt32 nCurrentCooldownFrames;
	XnBool bCurrentSteady;
	XnVPointBuffer* pPoints;
};

XN_DECLARE_DEFAULT_HASH(XnUInt32, XnVSteadyState*, XnVSteadyStates);
XN_DECLARE_EVENT_2ARG(XnVUintFloatSpecificEvent, XnVUIntFloatEvent, XnUInt32, id, XnFloat, fVal);

const XnFloat XnVSteadyDetector::ms_fDefaultMaximumStdDevForSteady = 0.01f;
const XnFloat XnVSteadyDetector::ms_fDefaultMinimumStdDevForNotSteady = 0.02f;
const XnUInt32 XnVSteadyDetector::ms_nDefaultDetectionDuration = 200;
const XnUInt32 XnVSteadyDetector::ms_nDefaultInitialCooldown = 0;

XnVSteadyDetector::XnVSteadyDetector(XnUInt32 nCooldownFrames,
									 XnUInt32 nDetectionDuration,
									 XnFloat fMaximumStdDevForSteady,
									 const XnChar* strName) :
	XnVPointControl(strName),
	m_nDetectionDuration(nDetectionDuration),
	m_fMaximumStdDevForSteady(fMaximumStdDevForSteady),
	m_fMaximumVarianceForSteady(fMaximumStdDevForSteady*fMaximumStdDevForSteady),
	m_fMinimumStdDevForNotSteady(ms_fDefaultMinimumStdDevForNotSteady),
	m_fMinimumVarianceForNotSteady(ms_fDefaultMinimumStdDevForNotSteady*ms_fDefaultMinimumStdDevForNotSteady),
	m_nInitialCooldownFrames(nCooldownFrames)
{
	m_pSteadyStates = XN_NEW(XnVSteadyStates);
	m_pSteadyCBs = XN_NEW(XnVUintFloatSpecificEvent);
	m_pNotSteadyCBs = XN_NEW(XnVUintFloatSpecificEvent);
} // XnVSteadyDetector::XnVSteadyDetector

XnVSteadyDetector::~XnVSteadyDetector()
{
	for (XnVSteadyStates::Iterator iter = m_pSteadyStates->begin(); iter != m_pSteadyStates->end(); ++iter)
	{
		XN_DELETE(iter.Value()->pPoints);
		XN_DELETE(iter.Value());
	}
	m_pSteadyStates->Clear();

	XN_DELETE(m_pSteadyStates);
	XN_DELETE(m_pSteadyCBs);
	XN_DELETE(m_pNotSteadyCBs);
} // XnVSteadyDetector::~XnVSteadyDetector

void XnVSteadyDetector::OnPointCreate(const XnVHandPointContext* cxt)
{
	Reset(cxt->nID);
	DetectSteady(cxt->nID, cxt->ptPosition, cxt->fTime);
} // XnVSteadyDetector::OnPrimaryPointCreate

void XnVSteadyDetector::OnPointUpdate(const XnVHandPointContext* cxt)
{
	DetectSteady(cxt->nID, cxt->ptPosition, cxt->fTime);
} // XnVSteadyDetector::OnPrimaryPointUpdate

void XnVSteadyDetector::Reset()
{
	XnAutoCSLocker local(m_hListenerCS);
	for (XnVSteadyStates::Iterator iter = m_pSteadyStates->begin(); iter != m_pSteadyStates->end(); ++iter)
	{
		iter.Value()->nCurrentCooldownFrames = m_nInitialCooldownFrames;
		iter.Value()->bCurrentSteady = FALSE;
		iter.Value()->pPoints->Reset();
	}
} // XnVSteadyDetector::Reset

void XnVSteadyDetector::Reset(XnUInt32 id)
{
	XnAutoCSLocker local(m_hListenerCS);
	XnVSteadyState* pState = NULL;
	m_pSteadyStates->Get(id, pState);
	if (pState != NULL)
	{
		pState->pPoints->Reset();
	}
	else
	{
		pState = XN_NEW(XnVSteadyState);
		pState->pPoints = XN_NEW(XnVPointBuffer);
		m_pSteadyStates->Set(id, pState);
	}
	pState->bCurrentSteady = FALSE;
	pState->nCurrentCooldownFrames = m_nInitialCooldownFrames;
}

XnStatus XnVSteadyDetector::DetectSteady(XnUInt32 id, const XnPoint3D& pt, XnFloat fTime)
{
	XnVSteadyState* pState = NULL;
	m_pSteadyStates->Get(id, pState);
	if (pState == NULL)
	{
		// Shouldn't happen. Try to recreate state
		Reset(id);
		m_pSteadyStates->Get(id, pState);
		if (pState == NULL)
		{
			return XN_STATUS_ERROR;
		}

	}

	if (pState->nCurrentCooldownFrames > 0)
	{
		pState->nCurrentCooldownFrames--;
		return XN_STATUS_NITE_COOLDOWN;
	}

	pState->pPoints->AddPoint(pt, fTime);
	// Make sure we have enough points
	if (pState->pPoints->GetAvailableTimespan() < m_nDetectionDuration)
	{
		return XN_STATUS_NITE_NOT_ENOUGH_TIME;
	}

	// Find current variance
	XnFloat fVariance = pState->pPoints->GetVariance(m_nDetectionDuration, fTime);
	fVariance /= m_nDetectionDuration*m_nDetectionDuration;

	if (!pState->bCurrentSteady && fVariance < m_fMaximumVarianceForSteady)
	{
		XnFloat fStdDev = sqrt(fVariance);
		xnLogVerbose(XNV_NITE_MASK_EVENTS, "Steady Detector %s [0x%08x], ID: %d: StdDev %5.3f under StdDev threshold %5.3f in the last %d ms",
 			GetListenerName(), this, id, fStdDev, m_fMaximumStdDevForSteady, m_nDetectionDuration);
 		OnSteadyDetected(id, fStdDev);
		pState->bCurrentSteady = TRUE;
	}
	else if (pState->bCurrentSteady && fVariance > m_fMinimumVarianceForNotSteady)
	{
		XnFloat fStdDev = sqrt(fVariance);
		xnLogVerbose(XNV_NITE_MASK_EVENTS, "Steady Detector %s [0x%08x], ID: %d: StdDev %5.3f over StdDev threshold %5.3f in the last %d ms",
			GetListenerName(), this, id, fStdDev, m_fMinimumStdDevForNotSteady, m_nDetectionDuration);
		OnNotSteadyDetected(id, fStdDev);
		pState->bCurrentSteady = FALSE;
	}

	return XN_STATUS_OK;
} // XnVSteadyDetector::DetectSteady

void XnVSteadyDetector::OnSteadyDetected(XnUInt32 id, XnFloat fStdDev)
{
	m_pSteadyCBs->Raise(id, fStdDev);
} // XnVSteadyDetector::OnSteadyDetected

XnCallbackHandle XnVSteadyDetector::RegisterSteady(void* cxt, XnVSteadyDetector::SteadyCB Steady)
{
	XnCallbackHandle handle;
	m_pSteadyCBs->Register(Steady, cxt, &handle);
	return handle;
} // XnVSteadyDetector::RegisterSteady

void XnVSteadyDetector::UnregisterSteady(XnCallbackHandle handle)
{
	m_pSteadyCBs->Unregister(handle);
} // XnVSteadyDetector::UnregisterSteady
void XnVSteadyDetector::OnNotSteadyDetected(XnUInt32 id, XnFloat fStdDev)
{
	m_pNotSteadyCBs->Raise(id, fStdDev);
}
XnCallbackHandle XnVSteadyDetector::RegisterNotSteady(void* cxt, XnVSteadyDetector::NotSteadyCB NotSteady)
{
	XnCallbackHandle handle;
	m_pNotSteadyCBs->Register(NotSteady, cxt, &handle);
	return handle;
}
void XnVSteadyDetector::UnregisterNotSteady(XnCallbackHandle handle)
{
	m_pNotSteadyCBs->Unregister(handle);
}
XnUInt32 XnVSteadyDetector::GetDetectionDuration() const
{
	return m_nDetectionDuration;
}
XnFloat XnVSteadyDetector::GetMaximumStdDevForSteady() const
{
	return m_fMaximumStdDevForSteady;
}
XnFloat XnVSteadyDetector::GetMinimumStdDevForNotSteady() const
{
	return m_fMinimumStdDevForNotSteady;
}

void XnVSteadyDetector::SetDetectionDuration(XnUInt32 nDuration)
{
	XnAutoCSLocker local(m_hListenerCS);
	m_nDetectionDuration = nDuration;
}
void XnVSteadyDetector::SetMaximumStdDevForSteady(XnFloat fStdDev)
{
	XnAutoCSLocker local(m_hListenerCS);
	m_fMaximumStdDevForSteady = fStdDev;
	m_fMaximumVarianceForSteady = fStdDev*fStdDev;
}

void XnVSteadyDetector::SetMinimumStdDevForNotSteady(XnFloat fStdDev)
{
	XnAutoCSLocker local(m_hListenerCS);
	m_fMinimumStdDevForNotSteady = fStdDev;
	m_fMinimumVarianceForNotSteady = fStdDev*fStdDev;
}

// Deprecated
XnFloat XnVSteadyDetector::GetMaximumVelocity() const
{
	return GetMaximumStdDevForSteady();
}
void XnVSteadyDetector::SetMaximumVelocity(XnFloat fVelocity)
{
	SetMaximumStdDevForSteady(fVelocity);
}
