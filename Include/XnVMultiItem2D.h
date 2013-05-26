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
#ifndef _XNV_MULTI_ITEM_2D_H_
#define _XNV_MULTI_ITEM_2D_H_

#include "XnVNiteDefs.h"

class XnVSigmoid;
class XnVPointBuffer;
class XnVIntIntSpecificEvent;
/**
* The multi item is initialized with a number of items in each axis,
* and sends a select event when moving between them.
* On update it gets a number between 0 and 1 for each axis,
* and sees which cell that translates into.
* Selection here means being over the item.
* The XnVMultiItem2D defines one events:
* - An item was selected
*
*/
class XNV_NITE_CONTROLS_API XnVMultiItem2D
{
public:
	/**
	* Type for the selection event callback
	*/
	typedef void (XN_CALLBACK_TYPE *ItemSelectCB)(XnInt32 ItemXIndex, XnInt32 ItemYIndex, void* pUserCxt);

	/**
	* Constructor
	*
	* @param	[in]	nItemXCount			Horizontal items
	* @param	[in]	nItemYCount			Vertical items
	* @param	[in]	fHysteresisRatio	Additional size (from any side) it takes to exit a specific item.
	*/
	XnVMultiItem2D(XnInt32 nItemXCount, XnInt32 nItemYCount);

	~XnVMultiItem2D();

	/**
	* Update a value between 0 and 1 for each axis,
	* which will be translated into an indices of the current cell.
	*/
	XnStatus Update(XnFloat fXValue, XnFloat fYValue);
	/**
	* Lose the point. Clear internal buffer.
	*/
	void LostPoint();

	/**
	* Invoke the selection event
	*
	* @param	[in]	nItemXIndex	The X-coordinate of the item that is selected
	* @param	[in]	nItemYIndex	The Y-coordinate of the item that is selected
	*/
	void ItemSelected(XnInt32 nItemXIndex, XnInt32 nItemYIndex);

	/**
	* Register for the selection event
	* 
	* @param	[in]	cxt	User's context
	* @param	[in]	pCB	The Callback to call when the event is invoked.
	*
	* @return	A handle, to allow unregistration.
	*/
	XnCallbackHandle RegisterItemSelect(void* cxt, ItemSelectCB pCB);
	/**
	* Unregister from the selection event
	*
	* @param	[in]	handle	The handle provided on registration.
	*/
	void UnregisterItemSelect(XnCallbackHandle handle);

	/**
	* Get the current number of item rows
	*
	* @return	The current number of rows
	*/
	XnUInt32 GetItemXCount() const;
	/**
	* Change the current number of items rows
	*
	* @param	[in]	nItemXCount	New number of rows
	*/
	void SetItemXCount(XnUInt32 nItemXCount);
	/**
	* Get the current number of item columns
	*
	* @return	The current number of columns
	*/
	XnUInt32 GetItemYCount() const;
	/**
	* Change the current number of items columns
	*
	* @param	[in]	nItemYCount	New number of columns
	*/
	void SetItemYCount(XnUInt32 nItemYCount);

	/**
	* Change the current number of items rows and columns
	*
	* @param	[in]	nItemXCount	New number of rows
	* @param	[in]	nItemYCount	New number of columns
	*/
	void SetItemCount(XnUInt32 nItemXCount, XnUInt32 nItemYCount);

	/**
	* Get the minimal value in the X-coordinate
	*
	* @return	The minimal value
	*/
	XnFloat GetMinXValue() const;
	/**
	* Get the minimal value in the Y-coordinate
	*
	* @return	The minimal value
	*/
	XnFloat GetMinYValue() const;
	/**
	* Get the maximal value in the X-coordinate
	*
	* @return	The maximal value
	*/
	XnFloat GetMaxYValue() const;
	/**
	* Get the maximal value in the Y-coordinate
	*
	* @return	The maximal value
	*/
	XnFloat GetMaxXValue() const;

protected:
	void CalculateMinAndMax();

	XnBool IsValueInRange(XnFloat fXValue, XnFloat fYValue);

	XnUInt32 m_nItemXCount;
	XnUInt32 m_nItemYCount;

	XnFloat m_fMinXValue, m_fMaxXValue;
	XnFloat m_fMinYValue, m_fMaxYValue;

	XnInt32 m_nLastSelectedXIndex;
	XnInt32 m_nLastSelectedYIndex;

	XnVIntIntSpecificEvent* m_pItemSelectCBs;
}; // XnVMultiItem2D

#endif // _XNV_MULTI_ITEM_2D_H_
