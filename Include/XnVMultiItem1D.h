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
#ifndef _XNV_MULTI_ITEM_1D_H_
#define _XNV_MULTI_ITEM_1D_H_

#include "XnVNiteDefs.h"

class XnVIntSpecificEvent;
/**
* The multi item is initialized with a number of items,
* and sends a select event when moving between them.
* On update it gets a number between 0 and 1, and sees which cell that translates into.
* Selection here means being over the item.
* The XnVMultiItem1D defines one events:
* - An item was selected
*
*/
class XNV_NITE_CONTROLS_API XnVMultiItem1D
{
public:
	/**
	* Type for the selection event callback
	*/
	typedef void (XN_CALLBACK_TYPE *ItemSelectedCB)(XnInt32 ItemIndex, void* pUserCxt);

	/**
	* Constructor
	*
	* @param	[in]	nItemCount			Number of items
	* @param	[in]	fHysteresisRatio	Additional size (from either side) it takes to exit a specific item.
	*/
	XnVMultiItem1D(XnInt32 nItemCount);

	~XnVMultiItem1D();

	/**
	* Update a value between 0 and 1, which will be translated into an index of the current cell.
	*/
	XnStatus Update(XnFloat Value);
	/**
	 * Lose the point. Clear internal buffer.
	 */
	void LostPoint();

	/**
	 * Invoke the selection event
	 *
	 * @param	[in]	nItemIndex	The item that is selected
	 */
	void ItemSelect(XnInt32 nItemIndex);

	/**
	* Register for the selection event
	* 
	* @param	[in]	cxt	User's context
	* @param	[in]	pCB	The Callback to call when the event is invoked.
	*
	* @return	A handle, to allow unregistration.
	*/
	XnCallbackHandle RegisterItemSelect(void* cxt, ItemSelectedCB pCB);
	/**
	* Unregister from the selection event
	*
	* @param	[in]	handle	The handle provided on registration.
	*/
	void UnregisterItemSelect(XnCallbackHandle handle);

	/**
	 * Get the current number of items.
	 *
	 * @return	The current number of items
	 */
	XnUInt32 GetItemCount() const;
	/**
	 * Change the current number of items
	 *
	 * @param	[in]	nCount	New number of items
	 */
	void SetItemCount(XnUInt32 nCount);

	/**
	 * Get the minimal value.
	 *
	 * @return	The minimal value
	 */
	XnFloat GetMinValue() const;
	/**
	* Get the maximal value.
	*
	* @return	The maximal value
	*/
	XnFloat GetMaxValue() const;

protected:
	void CalculateMinAndMax();
	bool IsValueInRange(XnFloat Value);


	XnUInt32 m_nItemCount;
	XnFloat m_fMinValue;
	XnFloat m_fMaxValue;

	XnInt32 m_nLastSelectedIndex;

	XnVIntSpecificEvent* m_pItemSelectCBs;
}; // XnVMultiItem1D

#endif // _XNV_MULTI_ITEM_1D_H_
