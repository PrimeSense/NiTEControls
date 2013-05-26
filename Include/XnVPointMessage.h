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
#ifndef _XNV_POINT_MESSAGE_H_
#define _XNV_POINT_MESSAGE_H_

#include "XnVMessage.h"
#include "XnVMultipleHands.h"

/**
* A XnVPointMessage is a Message holding a Multi Hands
*/
class XNV_NITE_CONTROLS_API XnVPointMessage :
	public XnVMessage
{
public:
	/**
	* Constructor. Create a Point Message with a Multi Hands.
	*
	* @param	[in]	pHands	The Multi Hands to use in the Point Message
	*/
	XnVPointMessage(const XnVMultipleHands* pHands);
	~XnVPointMessage();

	/**
	* Create a new Message of the same type (Point Message), with a clone of the same data (the same Multi Hands)
	*
	* @return	The new message
	*/
	XnVMessage* Clone() const;

	/**
	* The default type for Point Messages - "Point"
	*/
	static const XnChar* ms_strType;
protected:
	const XnVMultipleHands* m_pHands;
};

#endif
