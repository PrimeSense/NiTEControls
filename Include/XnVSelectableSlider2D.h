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
#ifndef _XNV_SELECTABLE_SLIDER_2D_H_
#define _XNV_SELECTABLE_SLIDER_2D_H_

#include <XnOpenNI.h>
#include "XnVNiteDefs.h"
#include "XnVPointControl.h"
#include "XnVDirection.h"
#include "XnVSlider2D.h"
#include "XnVMultiItem2D.h"
#include "XnVSlider1D.h"
#include "XnVMultiItem1D.h"

class XnVIntIntSpecificEvent;
class XnVFloatFloatSpecificEvent;
class XnVItemSelectSpecificEvent;
class XnVDirectionSpecificEvent;

/**
* A Selectable slider 2D is a 2D slider (on the x/y plane) with a number of cells and borders in both axis,
* where the items can be selected by moving off axis (in the z axis).
*
* Item (0,0) is the bottom-left one.
*/
class XNV_NITE_CONTROLS_API XnVSelectableSlider2D :
	public XnVPointControl
{
public:
	/**
	* Type for hover event callback. Receives the 2 indices of the hovered item
	*/
	typedef void (XN_CALLBACK_TYPE *ItemHoverCB)(XnInt32 nXIndex, XnInt32 nYIndex, void* pUserCxt);
	/**
	* Type for select event callback. Receives the 2 indices of the selected item and the direction of the selection.
	*/
	typedef void (XN_CALLBACK_TYPE *ItemSelectCB)(XnInt32 nXIndex, XnInt32 nYIndex, XnVDirection eDir, void* pUserCxt);
	/**
	* Type for off axis event callback. Receives the direction of the off axis movement
	*/
	typedef void (XN_CALLBACK_TYPE *OffAxisMovementCB)(XnVDirection eDir, void* pUserCxt);
	/**
	* Type for value change event callback.
	* Receives a number between 0 and 1 for each axis, indicating where in the slider the point is
	*/
	typedef void (XN_CALLBACK_TYPE *ValueChangeCB)(XnFloat fXValue, XnFloat fYValue, void* pUserCxt);

	/**
	* Creation. Gets the number of items in each axis for the slider, the width of the border (on all sides),
	* its axis, and more.
	*
	* @param	[in]	nXItems				Number of item rows in the slider
	* @param	[in]	nYItems				Number of item columns in the slider
	* @param	[in]	fSliderSizeX		Size of the slider in the x-coordinate (in mm)
	* @param	[in]	fSliderSizeY		Size of the slider in the y-coordinate (in mm)
	* @param	[in]	fBorderWidth		Size reserved from the slider for scrolling
	* @param	[in]	strName				Name of the control, for log purposes.
	*/
	XnVSelectableSlider2D(XnInt32 nXItems, XnInt32 nYItems,
		XnFloat fSliderSizeX = ms_fDefaultSliderSize, XnFloat fSliderSizeY = ms_fDefaultSliderSize,
		const XnChar* strName = "XnVSelectableSlider2D");
	virtual ~XnVSelectableSlider2D();

	/**
	* Called when the primary point is created.
	*
	* @param	[in]	pContext	The hand context of the newly created primary point
	* @param	[in]	ptFocus		The point in which the session has started.
	*/
	void OnPrimaryPointCreate(const XnVHandPointContext* pContext, const XnPoint3D& ptFocus);
	/**
	* Called when the primary point is updated.
	* This will cause the algorithm to manage the slider.
	*
	* @param	[in]	pContext	The hand context of the updated primary point
	*/
	void OnPrimaryPointUpdate(const XnVHandPointContext* pContext);
	/**
	* Called when the primary point is destroyed.
	*
	* @param	[in]	nID	the ID of the last primary point
	*/
	void OnPrimaryPointDestroy(XnUInt32 nID);

	/**
	* Register for the hover event
	* 
	* @param	[in]	cxt	User's context
	* @param	[in]	CB	The Callback to call when the event is invoked.
	*
	* @return	A handle, to allow unregistration.
	*/
	XnCallbackHandle RegisterItemHover(void* cxt, ItemHoverCB CB);
	/**
	* Register for the value change event
	* 
	* @param	[in]	cxt	User's context
	* @param	[in]	CB	The Callback to call when the event is invoked.
	*
	* @return	A handle, to allow unregistration.
	*/
	XnCallbackHandle RegisterValueChange(void* cxt, ValueChangeCB CB);
	/**
	* Register for the selection event
	* 
	* @param	[in]	cxt	User's context
	* @param	[in]	CB	The Callback to call when the event is invoked.
	*
	* @return	A handle, to allow unregistration.
	*/
	XnCallbackHandle RegisterItemSelect(void* cxt, ItemSelectCB CB);
	/**
	* Register for the off-axis event
	* 
	* @param	[in]	cxt	User's context
	* @param	[in]	CB	The Callback to call when the event is invoked.
	*
	* @return	A handle, to allow unregistration.
	*/
	XnCallbackHandle RegisterOffAxisMovement(void* cxt, OffAxisMovementCB CB);

	/**
	* Unregister from the hover event
	*
	* @param	[in]	hCB	The handle provided on registration.
	*/
	void UnregisterItemHover(XnCallbackHandle hCB);
	/**
	* Unregister from the value change event
	*
	* @param	[in]	hCB	The handle provided on registration.
	*/
	void UnregisterValueChange(XnCallbackHandle hCB);
	/**
	* Unregister from the selection event
	*
	* @param	[in]	hCB	The handle provided on registration.
	*/
	void UnregisterItemSelect(XnCallbackHandle hCB);
	/**
	* Unregister from the off-axis event
	*
	* @param	[in]	hCB	The handle provided on registration.
	*/
	void UnregisterOffAxisMovement(XnCallbackHandle hCB);

	static const XnFloat ms_fDefaultSliderSize;	// = 450

	/**
	 * Reposition the slider
	 *
	 * @param	[in]	ptCenter	The new center point for the slider.
	 */
	void Reposition(const XnPoint3D& ptCenter);
	void GetCenter(XnPoint3D& ptCenter) const;

	/**
	 * Get the slider sizes
	 *
	 * @param	[out]	fSliderXSize	The size of the X-Slider
	 * @param	[out]	fSliderYSize	The size of the Y-Slider
	 */
	void GetSliderSize(XnFloat& fSliderXSize, XnFloat& fSliderYSize) const;
	/**
	 * Change the slider sizes
	 *
	 * @param	[in]	fSliderXSize	The new size of the X-Slider
	 * @param	[in]	fSliderYSize	The new size of the Y-Slider
	 */
	void SetSliderSize(XnFloat fSliderXSize, XnFloat fSliderYSize);

	/**
	* Get the current number of item rows
	*
	* @return	The current number of rows
	*/
	XnUInt32 GetItemXCount() const;
	/**
	* Get the current number of item columns
	*
	* @return	The current number of columns
	*/
	XnUInt32 GetItemYCount() const;
	/**
	* Change the current number of items rows and columns
	*
	* @param	[in]	nItemXCount	New number of rows
	* @param	[in]	nItemYCount	New number of columns
	*/
	void SetItemCount(XnUInt32 nItemXCount, XnUInt32 nItemYCount);
	/**
	* Change the current number of items rows
	*
	* @param	[in]	nItemXCount	New number of rows
	*/
	void SetItemXCount(XnUInt32 nItemXCount);
	/**
	* Change the current number of items columns
	*
	* @param	[in]	nItemYCount	New number of columns
	*/
	void SetItemYCount(XnUInt32 nItemYCount);

	/**
	* Change behavior: Should the SelectableSlider keep sending ValueChange events when checking OffAxis
	*
	* @param	[in]	bReport	New mode. default is off
	*/
	void SetValueChangeOnOffAxis(XnBool bReport);
	/**
	* Get current behavior
	*/
	XnBool GetValueChangeOnOffAxis() const;

protected:
	void UpdateSlider(XnFloat fXValue, XnFloat fYValue);

	void PointDeleted(XnBool bReason);
	void PointMoved(const XnPoint3D& pt, XnFloat fTime);

	// Invoke events
	void ItemHover(XnInt32 nXIndex, XnInt32 nYIndex);
	void ValueChange(XnFloat fXValue, XnFloat fYValue);
	void ItemSelect(XnVDirection eDir);
	void OffAxisMovement(XnVDirection eDir);

	// Callbacks
	static void XN_CALLBACK_TYPE Slider_ValueChange(XnFloat fXValue, XnFloat fYValue, void* pContext);
	static void XN_CALLBACK_TYPE Slider_OffAxis(XnVDirection eDir, void* cxt);
	static void XN_CALLBACK_TYPE Hysteresis_ItemSelected(XnInt32 nXItem, XnInt32 nYItem, void* pContext);

	static void XN_CALLBACK_TYPE SecondarySlider_OffAxisMovement(XnVDirection eDir, void* cxt);
	static void XN_CALLBACK_TYPE SecondarySlider_ValueChange(XnFloat fValue, void* cxt);
	static void XN_CALLBACK_TYPE OffAxisHysteresis_ItemSelected(XnInt32 nItem, void* cxt);

	static const XnFloat ms_fSecondarySliderSize;
	XnBool m_bValueChangeWithoutBorders;

	XnBool m_bPositioned;
	XnInt32 m_nXItems, m_nYItems;

	XnBool m_bValueChangeOnOffAxis;

	XnFloat m_fSliderSizeX;
	XnFloat m_fSliderSizeY;
	XnVSlider2D* m_pSlider;
	XnVMultiItem2D* m_pHysteresis;

	XnBool m_bInOffAxis;
	XnVSlider1D* m_pSecondarySlider;
	XnVMultiItem1D* m_pOffAxisHysteresis;

	XnPoint3D m_ptCenter;

	XnInt32 m_nXLastHover, m_nYLastHover;

	XnCallbackHandle m_hHysteresisItemSelect;
	XnCallbackHandle m_hSliderValueChange;
	XnCallbackHandle m_hSliderOffAxis;

	XnCallbackHandle m_hOffAxisHysteresisItemSelect;
	XnCallbackHandle m_hSecondarySliderValueChange;
	XnCallbackHandle m_hSecondarySliderOffAxis;

	XnVIntIntSpecificEvent* m_pItemHoverCBs;
	XnVFloatFloatSpecificEvent* m_pValueChangeCBs;
	XnVItemSelectSpecificEvent* m_pItemSelectCBs;
	XnVDirectionSpecificEvent* m_pOffAxisMovementCBs;
};

#endif // _XNV_SELECTABLE_SLIDER_2D_H_
