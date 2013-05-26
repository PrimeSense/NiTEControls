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
#ifndef _XNV_SESSION_GENERATOR_H_
#define _XNV_SESSION_GENERATOR_H_

#include "XnVMessageGenerator.h"
#include "XnVSessionListener.h"

class XnVSessionListenerList;
/**
* A Session Generator generates Session Messages and calls SessionListener's callbacks
*/
class XNV_NITE_CONTROLS_API XnVSessionGenerator :
	public XnVMessageGenerator
{
public:
	/**
	* Constructor. Create a new Session Generator
	*
	* @param	[in]	strName	A name, for Log use
	*/
	XnVSessionGenerator(const XnChar* strName = "XnVSessionGenerator");
	virtual ~XnVSessionGenerator();

	/**
	* Add a listener for Session events
	*
	* @param	[in]	pListener	The listener to be called on session events
	*
	* @return A handle, allowing unregistering.
	*/
	XnVHandle RegisterSession(XnVSessionListener* pListener);
	/**
	* Add a functions to be called on Session events, with a specific context
	*
	* @param	[in]	cxt		The context to use when calling the functions
	* @param	[in]	StartCB	The function to call when a new Session has started
	* @param	[in]	EndCB	The function to call when the Session has ended
	* @param	[in]	MidCB	The (optional) function to call when a focus gesture started
	*
	* @return A handle, allowing unregistering.
	*/
	XnVHandle RegisterSession(void* cxt, XnVSessionListener::OnSessionStartCB StartCB, XnVSessionListener::OnSessionEndCB EndCB, XnVSessionListener::OnFocusStartDetectedCB MidCB = NULL);

	/**
	* Unregister a previously registered Session Listener
	*
	* @param	[in]	pSessionListener	The session listener that was registered
	*/
	void UnregisterSession(XnVSessionListener* pSessionListener);
	/**
	* Unregister previously registered Session Listener or functions, by handle
	*
	* @param	[in]	nHandle	The handle received on registration
	*/
	void UnregisterSession(XnVHandle nHandle);

	/**
	* Tells if currently in Session
	*
	* @return	TRUE if in Session (Started but never Stopped), FALSE otherwise
	*/
	XnBool IsInSession() const;

	/**
	 * Stop tracking a specific point
	 *
	 * @param	[in]	nID	The ID of the point to stop track
	 */
	virtual XnStatus LosePoint(XnUInt32 nID) = 0;
	/**
	* Stop tracking all points
	*/
	virtual XnStatus LosePoints() = 0;

	virtual XnStatus EndSession() = 0;
	virtual XnStatus ForceSession(const XnPoint3D& ptFocus) = 0;
	virtual XnStatus TrackPoint(const XnPoint3D& pt) = 0;

	XnStatus GetFocusPoint(XnPoint3D& ptFocus) const;
protected:
	virtual void SessionStart(const XnPoint3D& ptPosition);
	virtual void SessionMidGesture(const XnChar* strFocus, const XnPoint3D& ptFocus, XnFloat fProgress);
	virtual void SessionStop();

	XnBool m_bInSession;
	XnPoint3D m_ptFocusPoint;
	XnPoint3D m_ptStartTrackingPoint;

	XnVSessionListenerList* m_pSessionListeners;
};

#endif // _XNV_SESSION_GENERATOR_H_
