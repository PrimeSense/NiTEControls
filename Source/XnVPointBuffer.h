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
#ifndef _XNV_POINT_BUFFER_H_
#define _XNV_POINT_BUFFER_H_

#include <XnOpenNI.h>
#include <XnList.h>

struct XnVTimedPoint
{
	void Set(const XnPoint3D& pt, XnUInt64 nTime)
	{
		m_ptPosition = pt;
		m_nTime = nTime;
	}

	XnPoint3D m_ptPosition;
	XnUInt64 m_nTime;
};

XN_DECLARE_LIST(XnVTimedPoint*, XnVPointList);


class XnVPointBuffer
{
public:
	XnVPointBuffer(XnUInt32 nSize = ms_nDefaultBufferSize);
	~XnVPointBuffer();

	void Reset();
	void AddPoint(const XnPoint3D& pt, XnFloat fTime);

	XnUInt32 Size() const;

	XnPoint3D GetAveragePointByFrames(XnUInt32 nFrames, XnUInt32 nOffset = 0) const;
	XnPoint3D GetAverageVelocityByFrames(XnUInt32 nFrames, XnUInt32 nOffset = 0) const;

	XnPoint3D GetAveragePointByTime(XnUInt32 nDuration, XnFloat fTime, XnUInt32 nOffset = 0) const;
	XnPoint3D GetAverageVelocityByTime(XnUInt32 nDuration, XnFloat fTime, XnUInt32 nOffset = 0) const;

	XnFloat GetVariance(XnUInt32 nDuration, XnFloat fTime, XnUInt32 nOffset = 0) const;

	XnUInt32 GetAvailableTimespan() const;

	XnVPointList::Iterator begin();
	XnVPointList::Iterator end();
	void RemoveAfter(XnVPointList::Iterator iter);
protected:
	static const XnUInt32 ms_nDefaultBufferSize;

	XnUInt32 m_nBufferSize;
	XnUInt32 m_nCurrentSize;
	XnUInt32 m_nNextAvailable;

	XnVTimedPoint* m_pPoints;
	XnVPointList m_PointList;
};
#endif