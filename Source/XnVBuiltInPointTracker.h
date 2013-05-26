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
#ifndef _XNV_BUILT_IN_POINT_TRACKER_H_
#define _XNV_BUILT_IN_POINT_TRACKER_H_

#include <XnCppWrapper.h>

#include "XnVPointTracker.h"

class XnVBuiltInPointTracker :
	public XnVPointTracker
{
public:
	XnVBuiltInPointTracker(xn::HandsGenerator handTracker);
	~XnVBuiltInPointTracker();

	void Update(const xn::Context* pContext);

	XnStatus StartTracking(const XnPoint3D& ptPosition);
	XnStatus StopTracking(const XnUInt32 nID);
	XnStatus StopTracking();

	void Clear();

protected:
	void LosePoints();
	void PointCreate(XnUInt32 nID, const XnPoint3D& ptPosition, XnDouble fTime);
	void PointUpdate(XnUInt32 nID, const XnPoint3D& ptPosition, XnDouble fTime);
	void PointDestroy(XnUInt32 nID);

	static void XN_CALLBACK_TYPE HandCreate(xn::HandsGenerator& generator, XnUserID nId, const XnPoint3D* pPosition, XnFloat fTime, void* pCookie);
	static void XN_CALLBACK_TYPE HandUpdate(xn::HandsGenerator& generator, XnUserID nId, const XnPoint3D* pPosition, XnFloat fTime, void* pCookie);
	static void XN_CALLBACK_TYPE HandDestroy(xn::HandsGenerator& generator, XnUserID nId, XnFloat fTime, void* pCookie);

	XnVMultipleHands m_Hands;
	xn::HandsGenerator m_HandsGenerator;
	XnCallbackHandle m_hCallbacks;

	XnUInt64 m_nLastUsed;
};
#endif
