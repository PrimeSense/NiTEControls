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
#ifndef _XNV_POINT_FILTER_H_
#define _XNV_POINT_FILTER_H_

#include "XnVMessageGenerator.h"
#include "XnVPointControl.h"

/**
* A filter (listener/generator) which expects its messages to be PointMessages.
*/
class XNV_NITE_CONTROLS_API XnVPointFilter :
	public XnVMessageGenerator,
	public XnVPointControl
{
public:
	/**
	* Creation.
	*
	* @param	[in]	strName		Name of the control, for log purposes.
	*/
	XnVPointFilter(const XnChar* strName = "XnVPointFilter");

	/**
	 * Send the receives message as-is, allowing Point based callbacks.
	 */
	void Update(XnVMessage* pMessage);

	/**
	* Clear the Multi-threading queue
	*/
	void ClearQueue();
	/**
	 * Replace the point part of the message, and generate it
	 *
	 * @param	[in]	pMessage	The message now handling (expects a PointMessage or a ComplexMessage with an internal PointMessage)
	 * @param	[in]	newHands	The hands that are to replace the ones in the original message
	 */
	void GenerateReplaced(XnVMessage* pMessage, XnVMultipleHands& newHands);

	/**
	 * Change the primary ID. If the new primary doesn't exist, it will be turned off.
	 * This takes affect inside the GenerateReplaced function.
	 *
	 * @param	[in]	nNewPrimary	The ID that is to be the new primary. Use 0 to return to default
	 */
	void OverridePrimary(XnUInt32 nNewPrimary);
};

#endif // _XNV_POINT_FILTER_H_
