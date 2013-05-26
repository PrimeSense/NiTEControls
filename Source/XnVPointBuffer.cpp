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
#include "XnVPointBuffer.h"
#include "XnV3DVector.h"

const XnUInt32 XnVPointBuffer::ms_nDefaultBufferSize = 200;

XnVPointBuffer::XnVPointBuffer(XnUInt32 nSize) :
	m_nBufferSize(nSize),
	m_nCurrentSize(0),
	m_nNextAvailable(0)
{
	m_pPoints = XN_NEW_ARR(XnVTimedPoint, nSize);
} // XnVPointBuffer::XnVPointBuffer

XnVPointBuffer::~XnVPointBuffer()
{
	XN_DELETE_ARR(m_pPoints);
} // XnVPointBuffer::~XnVPointBuffer

void XnVPointBuffer::Reset()
{
	m_PointList.Clear();
	m_nCurrentSize = 0;
	m_nNextAvailable = 0;
} // XnVPointBuffer::Reset

void XnVPointBuffer::AddPoint(const XnPoint3D &pt, XnFloat fTime)
{
	XnUInt64 nNow = XnUInt64(fTime*1000);

	if (m_nCurrentSize == m_nBufferSize)
	{
		// Remove oldest, insert new instead
		XnVTimedPoint* pPoint = *m_PointList.rbegin();
		m_PointList.Remove(m_PointList.rbegin());

		pPoint->Set(pt, nNow);
		m_PointList.AddFirst(pPoint);
	}
	else
	{
		m_pPoints[m_nNextAvailable].Set(pt, nNow);
		m_PointList.AddFirst(&(m_pPoints[m_nNextAvailable]));
		m_nCurrentSize++;
		m_nNextAvailable++;
		if (m_nNextAvailable == m_nBufferSize)
		{
			m_nNextAvailable = 0;
		}
	}
} // XnVPointBuffer::AddPoint

XnUInt32 XnVPointBuffer::Size() const
{
	return m_nCurrentSize;
} // XnVPointBuffer::Size

XnPoint3D XnVPointBuffer::GetAveragePointByFrames(XnUInt32 nFrames, XnUInt32 nOffset) const
{
	XnPoint3D ptResult = xnCreatePoint3D(0,0,0);

	if (m_nCurrentSize <= nOffset)
	{
		return ptResult;
	}

	if (nOffset + nFrames > m_nCurrentSize)
	{
		nFrames = m_nCurrentSize - nOffset;
	}
	XnVPointList::ConstIterator iter = m_PointList.begin();
	
	for (unsigned int i = 0; i < nOffset; ++i)
	{
		iter++;
	}

	for (unsigned int i = 0; i < nFrames; ++i, ++iter)
	{
		XnPoint3D& point = ((XnVTimedPoint*)*iter)->m_ptPosition;
		ptResult.X += point.X;
		ptResult.Y += point.Y;
		ptResult.Z += point.Z;
	}

	ptResult.X /= nFrames;
	ptResult.Y /= nFrames;
	ptResult.Z /= nFrames;

	return ptResult;
} // XnVPointBuffer::GetAveragePointByFrames

XnPoint3D XnVPointBuffer::GetAveragePointByTime(XnUInt32 nDuration, XnFloat fTime, XnUInt32 nOffset) const
{
	XnUInt64 nNow = XnUInt64(fTime*1000);

	XnUInt32 nStop = XnUInt32(nNow - nOffset);
	XnUInt32 nStart = nStop - nDuration;

	XnV3DVector vResult;
	XnUInt32 nCounter = 0;

	XnVPointList::ConstIterator iter = m_PointList.begin();

	for (iter = m_PointList.begin(); iter != m_PointList.end(); ++iter)
	{
		XnVTimedPoint* pTimedPoint = *iter;

		if (pTimedPoint->m_nTime > nStop)
		{
			continue;
		}

		if (pTimedPoint->m_nTime < nStart)
		{
			break;
		}
		vResult += pTimedPoint->m_ptPosition;
		nCounter++;
	}

	if (nCounter > 0)
	{
		vResult /= XnFloat(nCounter);
	}

	return vResult;
} // XnVPointBuffer::GetAveragePointByTime

XnPoint3D XnVPointBuffer::GetAverageVelocityByFrames(XnUInt32 nFrames, XnUInt32 nOffset) const
{
	XnV3DVector ptResult(0,0,0);

	if (m_nCurrentSize <= nOffset || nFrames <= 1 || m_nCurrentSize <= 1)
	{
		return ptResult;
	}

	if (m_nCurrentSize < nOffset + nFrames)
	{
		nFrames = m_nCurrentSize - nOffset;
	}

	XnVPointList::ConstIterator liter = m_PointList.begin();
	XnVPointList::ConstIterator fiter = m_PointList.begin();

	for (XnUInt32 i = 0; i < nOffset; ++i)
	{
		++fiter;
		++liter;
	}
	for (XnUInt32 i = 1; i < nFrames; ++i)
	{
		++liter;
	}

	XnPoint3D& ptFirst = (*fiter)->m_ptPosition;
	XnPoint3D& ptLast = (*liter)->m_ptPosition;

	ptResult = xnCreatePoint3D((ptFirst.X-ptLast.X)/nFrames,
								(ptFirst.Y-ptLast.Y)/nFrames,
								(ptFirst.Z-ptLast.Z)/nFrames);

	return ptResult;
} // XnVPointBuffer::GetAverageVelocityByFrames

XnPoint3D XnVPointBuffer::GetAverageVelocityByTime(XnUInt32 nDuration, XnFloat fTime, XnUInt32 nOffset) const
{
	XnUInt64 nNow = XnUInt64(fTime*1000);

	XnUInt32 nStop = XnUInt32(nNow - nOffset);
	XnUInt32 nStart = nStop - nDuration;

	XnUInt32 nCounter = 0;
	XnVTimedPoint* pFirst =  NULL;
	XnVTimedPoint* pLast = NULL;

	for (XnVPointList::ConstIterator iter = m_PointList.begin(); iter != m_PointList.end(); ++iter)
	{
		XnVTimedPoint* pTimedPoint = *iter;

		if (pTimedPoint->m_nTime > nStop)
		{
			continue;
		}

		if (pTimedPoint->m_nTime < nStart)
		{
			break;
		}
		if (pLast == NULL)
		{
			pLast = pTimedPoint;
		}
		pFirst = pTimedPoint;
		nCounter++;
	}

	if (pLast == NULL || pFirst == NULL || pLast == pFirst)
	{
		return XnV3DVector(0,0,0);
	}

	XnV3DVector vResult = pLast->m_ptPosition;
	vResult -= pFirst->m_ptPosition;
	vResult /= XnFloat(pLast->m_nTime-pFirst->m_nTime);

	return vResult;
} // XnVPointBuffer::GetAverageVelocityByTime

// V(x) = E(x2)-E2(x)
XnFloat XnVPointBuffer::GetVariance(XnUInt32 nDuration, XnFloat fTime, XnUInt32 nOffset) const
{
	XnUInt64 nNow = XnUInt64(fTime*1000);

	XnUInt32 nStop = XnUInt32(nNow - nOffset);
	XnUInt32 nStart = nStop - nDuration;

	XnUInt32 nCounter = 0;
	XnVTimedPoint* pFirst =  NULL;
	XnVTimedPoint* pLast = NULL;

	XnFloat vSx2 = 0;
	XnV3DVector vSx;

	for (XnVPointList::ConstIterator iter = m_PointList.begin(); iter != m_PointList.end(); ++iter)
	{
		XnVTimedPoint* pTimedPoint = *iter;

		if (pTimedPoint->m_nTime > nStop)
		{
			continue;
		}

		if (pTimedPoint->m_nTime < nStart)
		{
			break;
		}
		++nCounter;
		vSx += pTimedPoint->m_ptPosition;
		vSx2 += XnV3DVector(pTimedPoint->m_ptPosition).MagnitudeSquared();
	}

	if (nCounter == 0)
	{
		return 1000;
	}
	vSx /= XnFloat(nCounter);
	vSx2 /= XnFloat(nCounter);
	XnFloat fVar = vSx2 - vSx.MagnitudeSquared();

	return fVar; // /nDuration^2?
} // XnVPointBuffer::GetVariance

XnVPointList::Iterator XnVPointBuffer::begin()
{
	return m_PointList.begin();
} // XnVPointBuffer::begin

XnVPointList::Iterator XnVPointBuffer::end()
{
	return m_PointList.end();
} // XnVPointBuffer::end

void XnVPointBuffer::RemoveAfter(XnVPointList::Iterator last)
{
	for (XnVPointList::Iterator iter = m_PointList.rbegin(); iter != m_PointList.rend() && iter != last; iter = m_PointList.rbegin())
	{
		m_PointList.Remove(iter);
		m_nCurrentSize--;
	}
} // XnVPointBuffer::RemoveAfter

XnUInt32 XnVPointBuffer::GetAvailableTimespan() const
{
	if (m_PointList.Size() <= 1)
	{
		return 0;
	}

	XnVPointList::ConstIterator last = m_PointList.begin();
	XnVPointList::ConstIterator first = m_PointList.rbegin();

	XnUInt64 nLast = (*last)->m_nTime;
	XnUInt64 nFirst = (*first)->m_nTime;

	return XnUInt32(nLast - nFirst);
} // XnVPointBuffer::GetAvailableTimespan
