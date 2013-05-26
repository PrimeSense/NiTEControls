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
#ifndef _XNV_POINT_TRACKER_H_
#define _XNV_POINT_TRACKER_H_

#include <XnCppWrapper.h>
#include "XnVMessageGenerator.h"
#include "XnVMultipleHands.h"

class XnVIntList;

/**
* A XnVPointTracker is a Context Filter, which is a Context Control and a Message Generator.
* It receives Context Messages (as a Context Control), and creates Point Messages to send on
*/
class XNV_NITE_CONTROLS_API XnVPointTracker :
	public XnVMessageGenerator
{
public:
	/**
	* Constructor. Create a new Point Tracker
	*
	* @param	[in]	strName	A name, for Log use
	*/
	XnVPointTracker(const XnChar* strName = "XnVPointTracker");
	virtual ~XnVPointTracker();
	/**
	* Start tracking, with the first hand at a certain position
	*
	* @param	[in]	ptPosition	The initial position of the first hand
	*/
	virtual XnStatus StartTracking(const XnPoint3D& ptPosition);
	/**
	* Stop tracking a certain hand
	*
	* @param	[in]	nID	The ID of the hand that should no longer be tracked
	*/
	virtual XnStatus StopTracking(const XnUInt32 nID);
	/**
	* Stop tracking all existing hands
	*/
	virtual XnStatus StopTracking();

	/**
	* Clear state
	*/
	virtual void Clear();

	virtual void Update(const xn::Context* pContext) = 0;

protected:
	XnVIntList* m_pLostPoints;
};

#endif

