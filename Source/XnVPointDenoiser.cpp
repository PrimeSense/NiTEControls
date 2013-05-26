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
#include "XnVPointDenoiser.h"
#include "XnVPointMessage.h"
#include "XnV3DVector.h"
#include <XnOSCpp.h>
#include <XnHash.h>

#define XNV_SMOOTHER_AVERAGE_SIZE 3
struct XnVDenoisingLocalContext
{
	XnPoint3D ptBuffer[XNV_SMOOTHER_AVERAGE_SIZE];
	XnUInt32 nCount;
	XnUInt32 nNextIndex;
};

XN_DECLARE_DEFAULT_HASH(XnUInt32, XnVDenoisingLocalContext*, XnVIntLocalHash);


const XnFloat XnVPointDenoiser::ms_fDefaultDistanceThreshold = 10;

const XnFloat XnVPointDenoiser::ms_fDefaultCloseRatio = .0f;
const XnFloat XnVPointDenoiser::ms_fDefaultFarRatio = 1.0f;

XnVPointDenoiser::XnVPointDenoiser(XnFloat fDistanceThreshold, const XnChar* strName) :
	XnVPointFilter(strName),
	m_fDistanceThreshold(fDistanceThreshold),
	m_fCloseRatio(ms_fDefaultCloseRatio),
	m_fFarRatio(ms_fDefaultFarRatio)
{
	m_pActivePoints = XN_NEW(XnVIntLocalHash);
}

XnVPointDenoiser::~XnVPointDenoiser()
{
	Clear();

	XN_DELETE(m_pActivePoints);
}

void XnVPointDenoiser::Update(XnVMessage* pMessage)
{
	XnVPointControl::Update(pMessage); // build m_DenoisedHands in CBs

	GenerateReplaced(pMessage, m_DenoisedHands);
	m_DenoisedHands.ClearLists();
} // XnVPointDenoiser::Update

void XnVPointDenoiser::OnPrimaryPointCreate(const XnVHandPointContext* pContext, const XnPoint3D& ptSessionStarter)
{
	m_DenoisedHands.SetFocusPoint(ptSessionStarter);
}

void XnVPointDenoiser::OnPointCreate(const XnVHandPointContext* pContext)
{
	XnVDenoisingLocalContext* pLocalContext = GetLocalContext(pContext->nID);

	if (pLocalContext == NULL)
	{
		pLocalContext = XN_NEW(XnVDenoisingLocalContext);
		if (pLocalContext == NULL)
		{
			return;
		}
		
		m_DenoisedHands.Add(pContext);

		pLocalContext->nCount = 0;
		pLocalContext->nNextIndex = 0;

		m_pActivePoints->Set(pContext->nID, pLocalContext);

	}
	m_DenoisedHands.MarkActive(pContext->nID);
	m_DenoisedHands.MarkNew(pContext->nID);
} // XnVPointDenoiser::OnPointCreate

void XnVPointDenoiser::OnPointUpdate(const XnVHandPointContext* pContext)
{
	XnVDenoisingLocalContext* pLocalContext = GetLocalContext(pContext->nID);
	if (pLocalContext == NULL)
	{
		return;
	}
	XnVHandPointContext* pDenoisedContext = m_DenoisedHands.GetContext(pContext->nID);

	pDenoisedContext->fTime = pContext->fTime;
	pLocalContext->ptBuffer[pLocalContext->nNextIndex] = pContext->ptPosition;
	pLocalContext->nNextIndex = (pLocalContext->nNextIndex+1)%XNV_SMOOTHER_AVERAGE_SIZE;
	pLocalContext->nCount++;

	XnUInt32 nAverageCount = XN_MIN(XNV_SMOOTHER_AVERAGE_SIZE, pLocalContext->nCount);
	XnV3DVector ptAveragePoint(0, 0, 0);

	for (XnUInt32 i = 0; i < nAverageCount; ++i)
	{
		ptAveragePoint += pLocalContext->ptBuffer[i];
	}
	if (nAverageCount > 0)
	{
		ptAveragePoint /= XnFloat(nAverageCount);
	}

	UpdatePointDenoise(pDenoisedContext->ptPosition, ptAveragePoint);

	m_DenoisedHands.MarkActive(pContext->nID);
} // XnVPointDenoiser::OnPointUpdate

void XnVPointDenoiser::OnPointDestroy(XnUInt32 nID)
{
	XnVDenoisingLocalContext* pLocalContext = GetLocalContext(nID);
	if (pLocalContext == NULL)
	{
		return;
	}

	m_DenoisedHands.Remove(nID);
	m_DenoisedHands.MarkOld(nID);

	m_pActivePoints->Remove(nID);

	XN_DELETE(pLocalContext);
} // XnVPointDenoiser::OnPointDestroy

XnVDenoisingLocalContext* XnVPointDenoiser::GetLocalContext(XnUInt32 nID)
{
	XnVDenoisingLocalContext* pContext;
	XnStatus rc = m_pActivePoints->Get(nID, pContext);
	if (rc != XN_STATUS_OK)
	{
		return NULL;
	}

	return pContext;
} // XnVPointDenoiser::GetLocalContext

void XnVPointDenoiser::Clear()
{
	for (XnVIntLocalHash::Iterator iter = m_pActivePoints->begin(); iter != m_pActivePoints->end(); ++iter)
	{
		XnVDenoisingLocalContext* pContext = iter.Value();
		XN_DELETE(pContext);
	}
	m_pActivePoints->Clear();
	m_DenoisedHands.Clear();
} // XnVPointDenoiser::Clear

static XnFloat SoftThreshold(XnFloat x, XnFloat fThreshold)
{
	XnFloat fRes = XN_MAX(XnFloat(fabs(x) - fThreshold), 0.0f);
	return x > 0 ? fRes : -fRes;
}

void XnVPointDenoiser::UpdatePointDenoise(XnPoint3D &ptToChange, const XnPoint3D &ptDontChange)
{
	XnV3DVector vDir = XnV3DVector(ptDontChange) - XnV3DVector(ptToChange);
	XnFloat fLen = vDir.Normalize();
	XnFloat fSoftThreshold = SoftThreshold(fLen, m_fDistanceThreshold);
	

	XnV3DVector vToChange = ptToChange;
	XnV3DVector vDontChange = ptDontChange;

	if (fSoftThreshold == 0)
	{
		ptToChange = vToChange*(1-m_fCloseRatio) + vDontChange*m_fCloseRatio;
	}
	else
	{
		XnV3DVector vNewPoint = XnV3DVector(ptToChange) + (fSoftThreshold) * vDir;
		ptToChange = vToChange*(1-m_fFarRatio) + vNewPoint*m_fFarRatio;
	}
} // XnVPointDenoiser::UpdatePointDenoise

XnFloat XnVPointDenoiser::GetDistanceThreshold() const
{
	return m_fDistanceThreshold;
}
XnFloat XnVPointDenoiser::GetCloseRatio() const
{
	return m_fCloseRatio;
}
XnFloat XnVPointDenoiser::GetFarRatio() const
{
	return m_fFarRatio;
}

void XnVPointDenoiser::SetDistanceThreshold(XnFloat fDistanceThreshold)
{
	XnAutoCSLocker local(m_hListenerCS);
	m_fDistanceThreshold = fDistanceThreshold;
}

void XnVPointDenoiser::SetCloseRatio(XnFloat fCloseRatio)
{
	XnAutoCSLocker local(m_hListenerCS);
	if (fCloseRatio >= 0 && fCloseRatio <= 1)
	{
		m_fCloseRatio = fCloseRatio;
	}
} // XnVPointDenoiser::SetCloseRatio

void XnVPointDenoiser::SetFarRatio(XnFloat fFarRatio)
{
	XnAutoCSLocker local(m_hListenerCS);
	if (fFarRatio >= 0 && fFarRatio <= 1)
	{
		m_fFarRatio = fFarRatio;
	}
} // XnVPointDenoiser::SetFarRatio
