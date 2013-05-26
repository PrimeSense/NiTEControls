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
#include "XnVBuiltInGesture.h"
#include "XnVNiteLog.h"

void XN_CALLBACK_TYPE XnVBuiltInGesture::GestureRecognized(xn::GestureGenerator& generator, const XnChar* strGesture, const XnPoint3D* pIDPosition, const XnPoint3D* pEndPosition, void* pCookie)
{
	XnVBuiltInGesture* pGesture = (XnVBuiltInGesture*)pCookie;

	XnStringsHash::ConstIterator item(pGesture->m_AvailableGestures.end());
	XnStatus rc = pGesture->m_AvailableGestures.Find(strGesture, item);

	if (rc == XN_STATUS_OK && item != pGesture->m_AvailableGestures.end())
	{
		pGesture->Recognized(strGesture, *pIDPosition, *pEndPosition);
	}
}
void XN_CALLBACK_TYPE XnVBuiltInGesture::GestureProgress(xn::GestureGenerator& generator, const XnChar* strGesture, const XnPoint3D* pPosition, XnFloat fProgress, void* pCookie)
{
	XnVBuiltInGesture* pGesture = (XnVBuiltInGesture*)pCookie;

	XnStringsHash::ConstIterator item(pGesture->m_AvailableGestures.end());
	XnStatus rc = pGesture->m_AvailableGestures.Find(strGesture, item);

	if (rc == XN_STATUS_OK && item != pGesture->m_AvailableGestures.end())
	{
		pGesture->StartRecognized(strGesture, *pPosition, fProgress);
	}
}



XnVBuiltInGesture::~XnVBuiltInGesture()
{
	m_GestureGenerator.UnregisterGestureCallbacks(m_hCallbacks);
	xnOSFree(m_strGesturesList);
}

XnVBuiltInGesture::XnVBuiltInGesture(xn::GestureGenerator gestureGenerator, const XnChar* strGestures) :
	XnVGesture("Built-In Gesture"),
	m_GestureGenerator(gestureGenerator),
	m_bActive(false)
{
	m_strGesturesList = xnOSStrDup(strGestures);
	XnChar* strCurrentGesture = m_strGesturesList;
	XnBool bLast = false;
	while (*strCurrentGesture != '\0')
	{
		// Replace first comma with NULL
		int nComma = 0;
		for (nComma = 0; strCurrentGesture[nComma] != '\0' && strCurrentGesture[nComma] != ','; ++nComma)
			;
		if (strCurrentGesture[nComma] == '\0')
			bLast = true;
		else
			strCurrentGesture[nComma] = '\0';

		// add current gesture
		m_AvailableGestures.Set(strCurrentGesture, NULL);
		// current gesture is next space (if exists)
		if (bLast)
			break;

		strCurrentGesture += nComma + 1;
	}

	m_GestureGenerator.RegisterGestureCallbacks(GestureRecognized, GestureProgress, this, m_hCallbacks);
//	StartGesture(NULL);
} // XnVBuiltInGesture::XnVBuiltInGesture

void XnVBuiltInGesture::StartGesture(XnBoundingBox3D* pbbArea)
{
	xnLogVerbose(XNV_NITE_MASK_SESSION, "Start Focus Module\n");

	for (XnStringsHash::ConstIterator iter = m_AvailableGestures.begin(); iter != m_AvailableGestures.end(); ++iter)
	{
		m_GestureGenerator.AddGesture(iter.Key(), pbbArea);
	}
	m_bActive = true;
} // XnVBuiltInGesture::StartGesture

void XnVBuiltInGesture::StopGesture()
{
	if (!m_bActive)
		return;

	xnLogVerbose(XNV_NITE_MASK_SESSION, "Stop Focus Module");
	
	for (XnStringsHash::ConstIterator iter = m_AvailableGestures.begin(); iter != m_AvailableGestures.end(); ++iter)
	{
		m_GestureGenerator.RemoveGesture(iter.Key());
	}
	m_bActive = false;
} // XnVBuiltInGesture::StopGesture

void XnVBuiltInGesture::Update(const xn::Context* pContext)
{
} // XnVBuiltInGesture::Update
