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
#ifndef _XNV_MESSAGE_GENERATOR_H_
#define _XNV_MESSAGE_GENERATOR_H_

#include "XnVMessageListener.h"
#include "XnVMessage.h"
#include "XnVHandle.h"

class XnVIntMessageListenerHash;
class XnStringsHash;

/**
* A XnVMessageGenerator knows how to generate messages.
* Message Listeners register to it, and receive its Messages.
*/
class XNV_NITE_CONTROLS_API XnVMessageGenerator
{
public:
	/**
	* Constructor. Create a new Message Generator
	*
	* @param	[in]	strName		Name of the control, for log purposes.
	*/
	XnVMessageGenerator(const XnChar* strName = "XnVMessageGenerator");
	virtual ~XnVMessageGenerator();

	/**
	* Add a Listener to the Generator
	*
	* @param	[in]	pListener	The listener to add
	*
	* @return	A handle to the listener, to allow removing it
	*/
	XnVHandle AddListener(XnVMessageListener* pListener);
	/**
	* Remove a listener from the generator
	*
	* @param	[in]	hListener	The handle of the listener to remove
	*
	* @return	The listener that was removed, or NULL if the handle was invalid
	*/
	XnVMessageListener* RemoveListener(XnVHandle hListener);
	/**
	* Remove a listener from the generator
	*
	* @param	[in]	pListener	The listener to remove
	*
	* @return	The listener that was removed, or NULL if the listener was never added
	*/
	XnVMessageListener* RemoveListener(XnVMessageListener* pListener);

	/**
	* Send the Message to all registered Listeners
	*
	* @param	[in]	pMessage	The Message to send
	*/
	void Generate(XnVMessage* pMessage);

	/**
	 * Clear the Multi-thread queue for all listeners
	 */
	void ClearAllQueues();

	/**
	 * Get the generator's name (for Log purposes)
	 *
	 * @return	The generator's name
	 */
	const XnChar* GetGeneratorName() const;
protected:
	void OpenNewSession(XnVMessageListener* pListener);
	void CloseOldSession(XnVMessageListener* pListener);

	XnVIntMessageListenerHash* m_phListeners;
	XnStringsHash* m_pProperties; // for last points
	XnVHandle m_hNextAvailable;

	XnChar* m_strGeneratorName;
};

#endif
