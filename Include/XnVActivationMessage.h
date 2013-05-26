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
#ifndef _XNV_ACTIVATION_MESSAGE_H_
#define _XNV_ACTIVATION_MESSAGE_H_

#include <XnOpenNI.h>

#include "XnVCCMessage.h"

/**
* A XnVActivationMessage is a Control Message holding a boolean value, indicating activate/deactivate
*/
class XNV_NITE_CONTROLS_API XnVActivationMessage :
	public XnVCCMessage
{
public:
	/**
	* Constructor. Create a Activation Message with a boolean
	*
	* @param	[in]	bActivate	Indication of activate or deactivate
	*/
	XnVActivationMessage(XnBool bActivate);
	~XnVActivationMessage();

	/**
	* The default type for Activation Messages - "Activation"
	*/
	static const XnChar* ms_strType;
};

#endif //_XNV_ACTIVATION_MESSAGE_H_
