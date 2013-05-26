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
#ifndef _XNV_SESSION_MANAGER_H_
#define _XNV_SESSION_MANAGER_H_

#include "XnVMessageGenerator.h"
#include "XnVMultipleHands.h"
#include "XnVGesture.h"
#include "XnVPointTracker.h"
#include "XnVPointMessage.h"
#include "XnVSessionGenerator.h"
#include "XnV3DVector.h"
#include "XnVSteadyDetector.h"

class XnVGestureHash;
/**
* A XnVSessionManager manages the session state
* Possible states are:
* Not In Session - Looking for the main gesture. Changing to this state is a "SessionStop" event.
* In Session - Tracking hand points. Changing to this state is a "SessionStart" event.
*
* Initial mode is "Not in session"
*/
class XNV_NITE_CONTROLS_API XnVSessionManager :
	public XnVMessageListener,
	public XnVSessionGenerator
{
public:
	/**
	* Constructor. Create a new Session Manager
	*
	* @param	[in]	strName	A name, for Log use
	*/
	XnVSessionManager(const XnChar* strName = "XnVSessionManager");
	~XnVSessionManager();

	/**
	* Initialize the Session. Set the gestures to use for focus as a comma-separated list.
	* 
	* @param	[in]	pContext		The OpenNI context from which to take some values
	* @param	[in]	strUseAsFocus	A comma-separated list of gestures to use as the main focus gesture
	* @param	[in]	pTracker		The HandsGenerator to use for tracking
	* @param	[in]	pFocusGenerator	The GestureGenerator from which to take the focus gesture
	*
	* @return	XN_STATUS_OK on success
	*/
	XnStatus Initialize(xn::Context* pContext, const XnChar* strUseAsFocus,
						xn::HandsGenerator* pTracker = NULL, xn::GestureGenerator* pFocusGenerator = NULL);

	/**
	* Handle incoming Messages.
	* Depth Messages are dealt with by the other Update method
	* Point Messages (from Point Tracker) are dealt with by ProcessPoints
	*
	* @param	[in]	pMessage	incoming Message
	*/
	void Update(XnVMessage* pMessage);
	/**
	* Handle a context. Calls the Gesture and the Tracker
	*
	* @param	[in]	pContext	The context to process
	*/
	void Update(const xn::Context* pContext);
	/**
	* Handle Hand Points. Manage session state.
	*
	* @param	[in]	pHands	Points summary
	*/
	void ProcessPoints(XnVMultipleHands* pHands);

	/**
	* Replace Tracker used
	*
	* @param	[in]	pTracker	new Tracker
	*/
	void SetTracker(XnVPointTracker* pTracker);
	void SetTracker(xn::HandsGenerator generator);

	/**
	* Stop tracking a specific point
	*
	* @param	[in]	nID	The ID of the point to stop track
	*/
	XnStatus LosePoint(XnUInt32 nID);
	/**
	* Stop tracking all points
	*/
	XnStatus LosePoints();

	/**
	 * Stop the current session. All points will be lost.
	 */
	XnStatus EndSession();
	/**
	 * Force a session to start, with a specific focus position.
	 *
	 * @param	[in]	ptFocus	The position to consider as focus point
	 */
	XnStatus ForceSession(const XnPoint3D& ptFocus);
	/**
	 * Start tracking a new hand from a specific position
	 *
	 * @param	[in]	pt	The position from which to start tracking
	 */
	XnStatus TrackPoint(const XnPoint3D& pt);

	/**
	 * Clear the multi-thread queue
	 */
	void ClearQueue();

	/**
	 * Add user-defined gesture to be used as a focus gesture
	 *
	 * @param	[in]	pGesture	User-defined gesture
	 *
	 * @return	ID to allow removal
	 */
	XnUInt32 AddGesture(XnVGesture* pGesture);
	/**
	* Add gesture to be used as a focus gesture, by gesture generator (OpenNI) and name
	*
	* @param	[in]	generator	Generator with the wanted gesture
	* @param	[in]	strName		Name of the wanted gesture in the generator
	*
	* @return	ID to allow removal
	*/
	XnUInt32 AddGesture(xn::GestureGenerator& generator, const XnChar* strName);
	/**
	* Add user-defined gesture to be used as a focus gesture, by OpenNI context and name
	*
	* @param	[in]	context		OpenNI context
	* @param	[in]	strName		Name of gesture, to be searched in all gesture generators
	*
	* @return	ID to allow removal
	*/
	XnUInt32 AddGesture(xn::Context& context, const XnChar* strName);
	/**
	 * Remove a gesture from being a focus gesture
	 *
	 * @param	[in]	nId		ID of the gesture to be removed, as received when adding it.
	 */
	void RemoveGesture(XnUInt32 nId);
protected:

	void SessionStop();

	XnStatus FindTracker(xn::Context* pContext, xn::HandsGenerator* pTracker, xn::HandsGenerator& tracker);

	static void XN_CALLBACK_TYPE Gesture_Recognized(const XnChar* strGesture, const XnPoint3D& ptIDPosition, const XnPoint3D& ptEndPosition, void* cxt);
	static void XN_CALLBACK_TYPE Gesture_StartRecognized(const XnChar* strGesture, const XnPoint3D& pos, XnFloat fProgress, void* cxt);
	void GestureRecognized(const XnChar* strGesture, const XnPoint3D& ptIDPosition, const XnPoint3D& ptEndPosition);
	void GestureStartRecognized(const XnChar* strGesture, const XnPoint3D& pos, XnFloat fProgress);

	XnUInt64 GetTime();

	void EnableGestures(XnBoundingBox3D* pROI = NULL);
	void DisableGestures();
	void UpdateGestures(const xn::Context* pContext);

	XnBool m_bGesturesEnabled;

	void ClearGestureHash(XnVGestureHash& hash);
	XnVGestureHash* m_pMainGestures;
	XnUInt32 m_nNextGestureID;

	XnUInt32 AddGesture(XnVGesture* pGesture, XnBool bAuto, XnUInt32 nHintID);

	XnUInt32 AddGesture(xn::Context& context, const XnChar* strName, const XnStringsHash& hash);
	XnUInt32 AddGesture(xn::GestureGenerator& generator, const XnChar* strName, const XnStringsHash& hash);

	void RemoveGesture(XnUInt32 nId, XnVGestureHash& hash);

	XnVPointTracker* m_pTracker;
	XnBool m_bAutoTracker;

	enum
	{
		SMGR_ILLEGAL		= 0x0000,
		SMGR_NOT_IN_SESSION	= 0x0001,
		SMGR_IN_SESSION     = 0x0002,
		SMGR_START_SESSION	= 0x0004,
	} m_eSessionState;

	inline XnBool IsInSession() const {return (m_eSessionState & SMGR_IN_SESSION) != 0;}
	inline XnBool IsNotInSession() const {return (m_eSessionState & SMGR_NOT_IN_SESSION) != 0;}

	XnCallbackHandle m_hGesture;
	XnCallbackHandle m_hGestureMid;

	const xn::Context* m_pContext;
};

#endif
