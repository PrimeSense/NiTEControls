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
#include "XnVPointTracker.h"
#include "XnVNiteEvents.h"

XnVPointTracker::XnVPointTracker(const XnChar* strName) :
	XnVMessageGenerator(strName)
{
	m_pLostPoints = XN_NEW(XnVIntList);
}

XnVPointTracker::~XnVPointTracker()
{
	XN_DELETE(m_pLostPoints);
}

XnStatus XnVPointTracker::StartTracking(const XnPoint3D& ptPosition)
{
	return XN_STATUS_OK;
}

XnStatus XnVPointTracker::StopTracking(const XnUInt32 nID)
{
	m_pLostPoints->AddLast(nID);

	return XN_STATUS_OK;
} // XnVPointTracker::StopTracking

XnStatus XnVPointTracker::StopTracking()
{
	return XN_STATUS_OK;
}

void XnVPointTracker::Clear()
{}

