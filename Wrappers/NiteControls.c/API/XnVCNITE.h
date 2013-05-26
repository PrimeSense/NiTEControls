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
#ifndef _XNV_C_NITE
#define _XNV_C_NITE

#include <XnOpenNI.h>

typedef void* XnVObjectID;

#ifdef XNV_NITE_CONTROLS_C_EXPORTS
#define XNV_NITE_CONTROLS_C_API XN_API_EXPORT
#else
#define XNV_NITE_CONTROLS_C_API XN_API_IMPORT
#endif


#if __cplusplus
extern "C"
{
#endif

	// XnVSession Manager

XNV_NITE_CONTROLS_C_API XnVObjectID xnvSessionManager_Create(const XnChar* strName);
XNV_NITE_CONTROLS_C_API XnStatus xnvSessionManager_Destroy(XnVObjectID id);
XNV_NITE_CONTROLS_C_API XnStatus xnvSessionManager_Initialize(XnVObjectID id, XnContext* pContext,
													const XnChar* strUseAsFocus,
													XnNodeHandle tracker,
													XnNodeHandle focusGenerator);
XNV_NITE_CONTROLS_C_API XnStatus xnvSessionManager_Update(XnVObjectID id, XnContext* pContext);
XNV_NITE_CONTROLS_C_API XnStatus xnvSessionManager_AddGestureByGenerator(XnVObjectID id, XnNodeHandle gestureGenerator, const XnChar* strName, XnUInt32* pGestureID);
XNV_NITE_CONTROLS_C_API XnStatus xnvSessionManager_AddGestureByContext(XnVObjectID id, XnContext* pContext, const XnChar* strName, XnUInt32* pGestureID);
XNV_NITE_CONTROLS_C_API XnStatus xnvSessionManager_RemoveGesture(XnVObjectID id, XnUInt32 nGestureID);

/* SessionGenerator */
typedef void (XN_CALLBACK_TYPE *SessionFocusProgressCB)(const XnChar* strFocus,
														const XnPoint3D* ptPosition, XnFloat fProgress,
														void* pCookie);
typedef void (XN_CALLBACK_TYPE *SessionStartCB)(const XnPoint3D* ptPosition, void* pCookie);
typedef void (XN_CALLBACK_TYPE *SessionEndCB)(void* pCookie);

XNV_NITE_CONTROLS_C_API XnStatus xnvSessionGenerator_LosePoint(XnVObjectID id, XnUInt32 pt);
XNV_NITE_CONTROLS_C_API XnStatus xnvSessionGenerator_LosePoints(XnVObjectID id);
XNV_NITE_CONTROLS_C_API XnStatus xnvSessionGenerator_EndSession(XnVObjectID id);
XNV_NITE_CONTROLS_C_API XnStatus xnvSessionGenerator_ForceSession(XnVObjectID id, const XnPoint3D* pFocus);
XNV_NITE_CONTROLS_C_API XnStatus xnvSessionGenerator_TrackPoint(XnVObjectID id, const XnPoint3D* pPosition);
XNV_NITE_CONTROLS_C_API XnStatus xnvSessionGenerator_IsInSession(XnVObjectID id, XnBool* pnInSession);
XNV_NITE_CONTROLS_C_API XnStatus xnvSessionGenerator_GetFocusPoint(XnVObjectID id, XnPoint3D* pFocus);
XNV_NITE_CONTROLS_C_API XnStatus xnvSessionGenerator_RegisterToSessionStart(XnVObjectID id,
																		SessionStartCB CB,
																		void* pCookie,
																		XnCallbackHandle* pHandle);
XNV_NITE_CONTROLS_C_API XnStatus xnvSessionGenerator_RegisterToSessionEnd(XnVObjectID id, SessionEndCB CB,
															  void* pCookie,
															  XnCallbackHandle* pHandle);
XNV_NITE_CONTROLS_C_API XnStatus xnvSessionGenerator_RegisterToSessionFocusProgress(XnVObjectID id,
																				SessionFocusProgressCB CB,
																				void* pCookie,
																				XnCallbackHandle* pHandle);
XNV_NITE_CONTROLS_C_API XnStatus xnvSessionGenerator_UnregisterFromSessionStart(XnVObjectID, XnCallbackHandle hCB);
XNV_NITE_CONTROLS_C_API XnStatus xnvSessionGenerator_UnregisterFromSessionEnd(XnVObjectID id, XnCallbackHandle hCB);
XNV_NITE_CONTROLS_C_API XnStatus xnvSessionGenerator_UnregisterFromSessionFocusProgress(XnVObjectID id,
																		XnCallbackHandle hCB);
/**/
// XnVBroadcaster
XNV_NITE_CONTROLS_C_API XnVObjectID xnvBroadcaster_Create(const XnChar* strName);
XNV_NITE_CONTROLS_C_API XnStatus xnvBroadcaster_Destroy(XnVObjectID id);
// XnVFlowRouter
XNV_NITE_CONTROLS_C_API XnVObjectID xnvFlowRouter_Create(const XnChar* strName);
XNV_NITE_CONTROLS_C_API XnStatus xnvFlowRouter_Destroy(XnVObjectID id);
XNV_NITE_CONTROLS_C_API XnStatus xnvFlowRouter_SetActive(XnVObjectID id, XnVObjectID messageListener);
XNV_NITE_CONTROLS_C_API XnStatus xnvFlowRouter_GetActive(XnVObjectID id, XnVObjectID* pMessageListener);

// XnVPointFilter
XNV_NITE_CONTROLS_C_API XnVObjectID xnvPointFilter_Create(const XnChar* strName);
XNV_NITE_CONTROLS_C_API XnStatus xnvPointFilter_Destroy(XnVObjectID id);
XNV_NITE_CONTROLS_C_API XnStatus xnvPointFilter_OverridePrimary(XnVObjectID id, XnUInt32 nNewPrimary);

// XnVPointArea
typedef void (XN_CALLBACK_TYPE*PointSilencedCB)(XnUInt32 nID, void* pCookie);
typedef void (XN_CALLBACK_TYPE*PointRevivedCB)(XnUInt32 nID, void* pCookie);
typedef void (XN_CALLBACK_TYPE*SilentPointRemovedCB)(XnUInt32 nID, void* pCookie);

XNV_NITE_CONTROLS_C_API XnVObjectID xnvPointArea_CreateByBoundingBox(XnBoundingBox3D* pArea, XnBool bRelative,
											 const XnChar* strName);
XNV_NITE_CONTROLS_C_API XnVObjectID xnvPointArea_CreateByPoints(const XnPoint3D* pMin, const XnPoint3D* pMax,
													  XnBool bRelative, const XnChar* strName);
XNV_NITE_CONTROLS_C_API XnStatus xnvPointArea_Destroy(XnVObjectID id);
XNV_NITE_CONTROLS_C_API XnStatus xnvPointArea_ChangeAreaByBoundingBox(XnVObjectID id, XnBoundingBox3D* pArea,
														XnBool bRelative);
XNV_NITE_CONTROLS_C_API XnStatus xnvPointArea_ChangeAreaByPoints(XnVObjectID id, const XnPoint3D* pMin,
												   const XnPoint3D* pMax, XnBool bRelative);
XNV_NITE_CONTROLS_C_API XnStatus xnvPointArea_RegisterToPointSilenced(XnVObjectID id, PointSilencedCB CB,
																	void* pCookie,
																	XnCallbackHandle* pHandle);
XNV_NITE_CONTROLS_C_API XnStatus xnvPointArea_RegisterToPointRevived(XnVObjectID id, PointRevivedCB CB,
																   void* pCookie,
																   XnCallbackHandle* pHandle);
XNV_NITE_CONTROLS_C_API XnStatus xnvPointArea_RegisterToSilentPointRemoved(XnVObjectID id,
																		 SilentPointRemovedCB CB,
																		 void* pCookie,
																		 XnCallbackHandle* pHandle);
XNV_NITE_CONTROLS_C_API XnStatus xnvPointArea_UnregisterFromPointSilenced(XnVObjectID id, XnCallbackHandle hCallback);
XNV_NITE_CONTROLS_C_API XnStatus xnvPointArea_UnregisterFromPointRevived(XnVObjectID id, XnCallbackHandle hCallback);
XNV_NITE_CONTROLS_C_API XnStatus xnvPointArea_UnregisterFromSilentPointRemoved(XnVObjectID id,
																 XnCallbackHandle hCallback);
XNV_NITE_CONTROLS_C_API XnStatus xnvPointArea_RemoveSilent(XnVObjectID id, XnUInt64 nDelay,
												 XnVObjectID sessionManagerId);

// XnVPointDenoiser
XNV_NITE_CONTROLS_C_API XnVObjectID xnvPointDenoiser_Create(XnFloat fDistanceThreshold, const XnChar* strName);
XNV_NITE_CONTROLS_C_API XnStatus xnvPointDenoiser_Destroy(XnVObjectID id);
XNV_NITE_CONTROLS_C_API XnStatus xnvPointDenoiser_GetDistanceThreshold(XnVObjectID id, XnFloat* pfDistanceThreshold);
XNV_NITE_CONTROLS_C_API XnStatus xnvPointDenoiser_SetDistanceThreshold(XnVObjectID id, XnFloat fDistanceThreshold);
XNV_NITE_CONTROLS_C_API XnStatus xnvPointDenoiser_GetCloseRatio(XnVObjectID id, XnFloat* pfCloseRatio);
XNV_NITE_CONTROLS_C_API XnStatus xnvPointDenoiser_GetFarRatio(XnVObjectID id, XnFloat* pfFarRatio);
XNV_NITE_CONTROLS_C_API XnStatus xnvPointDenoiser_SetCloseRatio(XnVObjectID id, XnFloat fCloseRatio);
XNV_NITE_CONTROLS_C_API XnStatus xnvPointDenoiser_SetFarRatio(XnVObjectID id, XnFloat fFarRatio);

// XnVCircleDetector
typedef enum XnVNoCircleReason
{
	NO_CIRCLE_ILLEGAL,
	NO_CIRCLE_NO_INPUT,
	NO_CIRCLE_BAD_POINTS,
	NO_CIRCLE_MANUAL
} XnVNoCircleReason;
typedef struct XnVCCircle
{
	XnPoint3D ptCenter;
	XnFloat fRadius;
} XnVCCircle;
typedef void (XN_CALLBACK_TYPE *CircleCB)(XnFloat fTimes, XnBool bConfident,
										  const XnVCCircle* pCircle,
										  void* pCookie);
typedef void (XN_CALLBACK_TYPE *NoCircleCB)(XnFloat fLastValue, XnVNoCircleReason eReason,
											void* pCookie);

XNV_NITE_CONTROLS_C_API XnVObjectID xnvCircleDetector_Create(const XnChar* strName);
XNV_NITE_CONTROLS_C_API XnStatus xnvCircleDetector_Destroy(XnVObjectID id);
XNV_NITE_CONTROLS_C_API XnStatus xnvCircleDetector_RegisterToCircle(XnVObjectID id, CircleCB CB,
																  void* pCookie,
																  XnCallbackHandle* pHandle);
XNV_NITE_CONTROLS_C_API XnStatus xnvCircleDetector_RegisterToNoCircle(XnVObjectID id, NoCircleCB CB,
																	void* pCookie,
																	XnCallbackHandle* pHandle);
XNV_NITE_CONTROLS_C_API XnStatus xnvCircleDetector_UnregisterFromCircle(XnVObjectID id, XnCallbackHandle hCB);
XNV_NITE_CONTROLS_C_API XnStatus xnvCircleDetector_UnregisterFromNoCircle(XnVObjectID id, XnCallbackHandle hCB);
XNV_NITE_CONTROLS_C_API XnStatus xnvCircleDetector_Reset(XnVObjectID id);
XNV_NITE_CONTROLS_C_API XnStatus xnvCircleDetector_SetMinimumPoints(XnVObjectID id, XnUInt32 nMinimumPoints);
XNV_NITE_CONTROLS_C_API XnStatus xnvCircleDetector_SetCloseToExistingRadius(XnVObjectID id,
																  XnFloat fCloseToExistingRadius);
XNV_NITE_CONTROLS_C_API XnStatus xnvCircleDetector_SetCloseEnough(XnVObjectID id, XnFloat fCloseEnough);
XNV_NITE_CONTROLS_C_API XnStatus xnvCircleDetector_SetMinRadius(XnVObjectID id, XnFloat fMinRadius);
XNV_NITE_CONTROLS_C_API XnStatus xnvCircleDetector_SetMaxRadius(XnVObjectID id, XnFloat fMaxRadius);
XNV_NITE_CONTROLS_C_API XnStatus xnvCircleDetector_SetExistingWeight(XnVObjectID id, XnFloat fExistingWeight);
XNV_NITE_CONTROLS_C_API XnStatus xnvCircleDetector_SetMaxErrors(XnVObjectID id, XnUInt32 nMaxErrors);
XNV_NITE_CONTROLS_C_API XnStatus xnvCircleDetector_GetMinimumPoints(XnVObjectID id, XnUInt32* pnMinimumPoints);
XNV_NITE_CONTROLS_C_API XnStatus xnvCircleDetector_GetCloseToExistingRadius(XnVObjectID id,
																  XnFloat* pfCloseToExistingRadius);
XNV_NITE_CONTROLS_C_API XnStatus xnvCircleDetector_GetCloseEnough(XnVObjectID id, XnFloat* pfCloseEnough);
XNV_NITE_CONTROLS_C_API XnStatus xnvCircleDetector_GetMinRadius(XnVObjectID id, XnFloat* pfMinRadius);
XNV_NITE_CONTROLS_C_API XnStatus xnvCircleDetector_GetMaxRadius(XnVObjectID id, XnFloat* pfMaxRadius);
XNV_NITE_CONTROLS_C_API XnStatus xnvCircleDetector_GetExistingWeight(XnVObjectID id, XnFloat* pfExistingWeight);
XNV_NITE_CONTROLS_C_API XnStatus xnvCircleDetector_GetMaxErrors(XnVObjectID id, XnUInt32* pnMaxErrors);

// XnVPushDetector
typedef void (XN_CALLBACK_TYPE *PushCB)(XnFloat fVelocity, XnFloat fAngle, void* pCookie);
typedef void (XN_CALLBACK_TYPE *StabilizedCB)(XnFloat fVelocity, void* pCookie);

XNV_NITE_CONTROLS_C_API XnVObjectID xnvPushDetector_Create(const XnChar* strName);
XNV_NITE_CONTROLS_C_API XnStatus xnvPushDetector_Destroy(XnVObjectID id);
XNV_NITE_CONTROLS_C_API XnStatus xnvPushDetector_RegisterPush(XnVObjectID id, PushCB pCB,
															void* pCookie,
															XnCallbackHandle* pHandle);
XNV_NITE_CONTROLS_C_API XnStatus xnvPushDetector_RegisterStabilized(XnVObjectID id, StabilizedCB pCB,
																  void* pCookie,
																  XnCallbackHandle* pHandle);
XNV_NITE_CONTROLS_C_API XnStatus xnvPushDetector_UnregisterPush(XnVObjectID id, XnCallbackHandle handle);
XNV_NITE_CONTROLS_C_API XnStatus xnvPushDetector_UnregisterStabilized(XnVObjectID id, XnCallbackHandle handle);
XNV_NITE_CONTROLS_C_API XnStatus xnvPushDetector_Reset(XnVObjectID id);
XNV_NITE_CONTROLS_C_API XnStatus xnvPushDetector_GetPushImmediateMinimumVelocity(XnVObjectID id, XnFloat* pfVelocity);
XNV_NITE_CONTROLS_C_API XnStatus xnvPushDetector_GetPushImmediateDuration(XnVObjectID id, XnUInt32* pnDuration);
XNV_NITE_CONTROLS_C_API XnStatus xnvPushDetector_GetPushImmediateOffset(XnVObjectID id, XnUInt32* pnOffset);
XNV_NITE_CONTROLS_C_API XnStatus xnvPushDetector_GetPushPreviousMinimumVelocity(XnVObjectID id, XnFloat* pfVelocity);
XNV_NITE_CONTROLS_C_API XnStatus xnvPushDetector_GetPushPreviousDuration(XnVObjectID id, XnUInt32* pnDuration);
XNV_NITE_CONTROLS_C_API XnStatus xnvPushDetector_GetPushPreviousOffset(XnVObjectID id, XnUInt32* pnOffset);
XNV_NITE_CONTROLS_C_API XnStatus xnvPushDetector_GetPushMaximumAngleBetweenImmediateAndZ(XnVObjectID id, XnFloat* pfAngle);
XNV_NITE_CONTROLS_C_API XnStatus xnvPushDetector_GetPushMinimumAngleBetweenImmediateAndPrevious(XnVObjectID id, XnFloat* pfAngle);
XNV_NITE_CONTROLS_C_API XnStatus xnvPushDetector_GetStableMaximumVelocity(XnVObjectID id, XnFloat* pfVelocity);
XNV_NITE_CONTROLS_C_API XnStatus xnvPushDetector_GetStableDuration(XnVObjectID id, XnUInt32* pnDuration);
XNV_NITE_CONTROLS_C_API XnStatus xnvPushDetector_SetPushImmediateMinimumVelocity(XnVObjectID id, XnFloat fVelocity);
XNV_NITE_CONTROLS_C_API XnStatus xnvPushDetector_SetPushImmediateDuration(XnVObjectID id, XnUInt32 nDuration);
XNV_NITE_CONTROLS_C_API XnStatus xnvPushDetector_SetPushImmediateOffset(XnVObjectID id, XnUInt32 nOffset);
XNV_NITE_CONTROLS_C_API XnStatus xnvPushDetector_SetPushPreviousMinimumVelocity(XnVObjectID id, XnFloat fVelocity);
XNV_NITE_CONTROLS_C_API XnStatus xnvPushDetector_SetPushPreviousDuration(XnVObjectID id, XnUInt32 nDuration);
XNV_NITE_CONTROLS_C_API XnStatus xnvPushDetector_SetPushPreviousOffset(XnVObjectID id, XnUInt32 nOffset);
XNV_NITE_CONTROLS_C_API XnStatus xnvPushDetector_SetPushMaximumAngleBetweenImmediateAndZ(XnVObjectID id,
																		   XnFloat fAngle);
XNV_NITE_CONTROLS_C_API XnStatus xnvPushDetector_SetPushMinimumAngleBetweenImmediateAndPrevious(XnVObjectID id,
																				  XnFloat fAngle);
XNV_NITE_CONTROLS_C_API XnStatus xnvPushDetector_SetStableMaximumVelocity(XnVObjectID id, XnFloat fVelocity);
XNV_NITE_CONTROLS_C_API XnStatus xnvPushDetector_SetStableDuration(XnVObjectID id, XnUInt32 nDuration);

// XnVSelectableSlider1D
#include "XnVDirection.h"
typedef void (XN_CALLBACK_TYPE *ItemHover1DCB)(XnInt32 nItemIndex, void* pCookie);
typedef void (XN_CALLBACK_TYPE *ItemSelect1DCB)(XnInt32 nItemIndex, XnVDirection nDirection,
												void* pCookie);
typedef void (XN_CALLBACK_TYPE *Scroll1DCB)(XnFloat fScrollValue, void* pCookie);
typedef void (XN_CALLBACK_TYPE *ValueChange1DCB)(XnFloat fValue, void* pCookie);
typedef void (XN_CALLBACK_TYPE *OffAxisMovement1DCB)(XnVDirection dir, void* pCookie);

XNV_NITE_CONTROLS_C_API XnVObjectID xnvSelectableSlider1D_Create(XnInt32 nCount,
										 XnVAxis eAxis, XnBool bRecenter,
										 XnFloat fPrimarySliderSize, XnFloat fFirstValue,
										 const XnChar* strName);

XNV_NITE_CONTROLS_C_API XnStatus xnvSelectableSlider1D_Destroy(XnVObjectID id);
XNV_NITE_CONTROLS_C_API XnStatus xnvSelectableSlider1D_RegisterItemHover(XnVObjectID id,
																	   ItemHover1DCB pCB,
																	   void* pCookie,
																	   XnCallbackHandle* pHandle);
XNV_NITE_CONTROLS_C_API XnStatus xnvSelectableSlider1D_RegisterItemSelect(XnVObjectID id,
																		ItemSelect1DCB pCB,
																		void* pCookie,
																		XnCallbackHandle* pHandle);
XNV_NITE_CONTROLS_C_API XnStatus xnvSelectableSlider1D_RegisterScroll(XnVObjectID id, Scroll1DCB pCB,
																	void* pCookie,
																	XnCallbackHandle* pHandle);
XNV_NITE_CONTROLS_C_API XnStatus xnvSelectableSlider1D_RegisterValueChange(XnVObjectID id,
																		 ValueChange1DCB pCB,
																		 void* pCookie,
																		 XnCallbackHandle* pHandle);
XNV_NITE_CONTROLS_C_API XnStatus xnvSelectableSlider1D_RegisterOffAxisMovement(XnVObjectID id,
																			 OffAxisMovement1DCB pCB,
																			 void* pCookie,
																			 XnCallbackHandle* pHandle);
XNV_NITE_CONTROLS_C_API XnStatus xnvSelectableSlider1D_UnregisterItemHover(XnVObjectID id, XnCallbackHandle handle);
XNV_NITE_CONTROLS_C_API XnStatus xnvSelectableSlider1D_UnregisterItemSelect(XnVObjectID id, XnCallbackHandle handle);
XNV_NITE_CONTROLS_C_API XnStatus xnvSelectableSlider1D_UnregisterScroll(XnVObjectID id, XnCallbackHandle handle);
XNV_NITE_CONTROLS_C_API XnStatus xnvSelectableSlider1D_UnregisterValueChange(XnVObjectID id, XnCallbackHandle handle);
XNV_NITE_CONTROLS_C_API XnStatus xnvSelectableSlider1D_UnregisterOffAxisMovement(XnVObjectID id,
																   XnCallbackHandle handle);
XNV_NITE_CONTROLS_C_API XnStatus xnvSelectableSlider1D_Reposition(XnVObjectID id, const XnPoint3D* pCenter);
XNV_NITE_CONTROLS_C_API XnStatus xnvSelectableSlider1D_GetCenter(XnVObjectID id, XnPoint3D* pCenter);
XNV_NITE_CONTROLS_C_API XnStatus xnvSelectableSlider1D_GetItemCount(XnVObjectID id, XnUInt32* pnItems);
XNV_NITE_CONTROLS_C_API XnStatus xnvSelectableSlider1D_SetItemCount(XnVObjectID id, XnUInt32 nItems);
XNV_NITE_CONTROLS_C_API XnStatus xnvSelectableSlider1D_GetSliderSize(XnVObjectID id, XnFloat* pfSliderSize);
XNV_NITE_CONTROLS_C_API XnStatus xnvSelectableSlider1D_SetSliderSize(XnVObjectID id, XnFloat fSliderSize);
XNV_NITE_CONTROLS_C_API XnStatus xnvSelectableSlider1D_SetValueChangeOnOffAxis(XnVObjectID id, XnBool bReport);
XNV_NITE_CONTROLS_C_API XnStatus xnvSelectableSlider1D_GetValueChangeOnOffAxis(XnVObjectID id, XnBool* pbReport);

// XnVSelectableSlider2D
typedef void (XN_CALLBACK_TYPE *ItemHover2DCB)(XnInt32 nXIndex, XnInt32 nYIndex, void* pCookie);
typedef void (XN_CALLBACK_TYPE *ItemSelect2DCB)(XnInt32 nXIndex, XnInt32 nYIndex, XnVDirection eDir,
												void* pCookie);
typedef void (XN_CALLBACK_TYPE *OffAxisMovement2DCB)(XnVDirection eDir, void* pCookie);
typedef void (XN_CALLBACK_TYPE *ValueChange2DCB)(XnFloat fXValue, XnFloat fYValue, void* pCookie);
typedef void (XN_CALLBACK_TYPE *Scroll2DCB)(XnFloat fXValue, XnFloat fYValue, void* pCookie);

XNV_NITE_CONTROLS_C_API XnVObjectID xnvSelectableSlider2D_Create(XnInt32 nXItems, XnInt32 nYItems,
										 XnFloat fSliderSizeX, XnFloat fSliderSizeY,
										 const XnChar* strName);
XNV_NITE_CONTROLS_C_API XnStatus xnvSelectableSlider2D_Destroy(XnVObjectID id);

XNV_NITE_CONTROLS_C_API XnStatus xnvSelectableSlider2D_RegisterItemHover(XnVObjectID id,
																	   ItemHover2DCB CB,
																	   void* pCookie,
																	   XnCallbackHandle* pHandle);
XNV_NITE_CONTROLS_C_API XnStatus xnvSelectableSlider2D_RegisterValueChange(XnVObjectID id,
																		 ValueChange2DCB CB,
																		 void* pCookie,
																		 XnCallbackHandle* pHandle);
XNV_NITE_CONTROLS_C_API XnStatus xnvSelectableSlider2D_RegisterItemSelect(XnVObjectID id,
																		ItemSelect2DCB CB,
																		void* pCookie,
																		XnCallbackHandle* pHandle);
XNV_NITE_CONTROLS_C_API XnStatus xnvSelectableSlider2D_RegisterOffAxisMovement(XnVObjectID id,
																			 OffAxisMovement2DCB CB,
																			 void* pCookie,
																			 XnCallbackHandle* pHandle);
XNV_NITE_CONTROLS_C_API XnStatus xnvSelectableSlider2D_UnregisterItemHover(XnVObjectID id, XnCallbackHandle hCB);
XNV_NITE_CONTROLS_C_API XnStatus xnvSelectableSlider2D_UnregisterValueChange(XnVObjectID id, XnCallbackHandle hCB);
XNV_NITE_CONTROLS_C_API XnStatus xnvSelectableSlider2D_UnregisterItemSelect(XnVObjectID id, XnCallbackHandle hCB);
XNV_NITE_CONTROLS_C_API XnStatus xnvSelectableSlider2D_UnregisterOffAxisMovement(XnVObjectID id,
																   XnCallbackHandle hCB);
XNV_NITE_CONTROLS_C_API XnStatus xnvSelectableSlider2D_Reposition(XnVObjectID id, const XnPoint3D* pCenter);
XNV_NITE_CONTROLS_C_API XnStatus xnvSelectableSlider2D_GetCenter(XnVObjectID id, XnPoint3D* pCenter);
XNV_NITE_CONTROLS_C_API XnStatus xnvSelectableSlider2D_GetSliderSize(XnVObjectID id, XnFloat* pfSliderXSize,
										 XnFloat* pfSliderYSize);
XNV_NITE_CONTROLS_C_API XnStatus xnvSelectableSlider2D_SetSliderSize(XnVObjectID id, XnFloat fSliderXSize,
													   XnFloat fSliderYSize);
XNV_NITE_CONTROLS_C_API XnStatus xnvSelectableSlider2D_GetItemXCount(XnVObjectID id, XnUInt32* pnItems);
XNV_NITE_CONTROLS_C_API XnStatus xnvSelectableSlider2D_GetItemYCount(XnVObjectID id, XnUInt32* pnItems);
XNV_NITE_CONTROLS_C_API XnStatus xnvSelectableSlider2D_SetItemCount(XnVObjectID id, XnUInt32 nItemXCount,
													  XnUInt32 nItemYCount);
XNV_NITE_CONTROLS_C_API XnStatus xnvSelectableSlider2D_SetItemXCount(XnVObjectID id, XnUInt32 nItemXCount);
XNV_NITE_CONTROLS_C_API XnStatus xnvSelectableSlider2D_SetItemYCount(XnVObjectID id, XnUInt32 nItemYCount);
XNV_NITE_CONTROLS_C_API XnStatus xnvSelectableSlider2D_SetValueChangeOnOffAxis(XnVObjectID id, XnBool bReport);
XNV_NITE_CONTROLS_C_API XnStatus xnvSelectableSlider2D_GetValueChangeOnOffAxis(XnVObjectID id, XnBool* pbReport);

// XnVSteadyDetector
typedef void (XN_CALLBACK_TYPE *SteadyCB)(XnUInt32 nId, XnFloat fStdDev, void* pCookie);
typedef void (XN_CALLBACK_TYPE *NotSteadyCB)(XnUInt32 nId, XnFloat fStdDev, void* pCookie);

XNV_NITE_CONTROLS_C_API XnVObjectID xnvSteadyDetector_Create(XnUInt32 nCooldownFrames,
												   XnUInt32 nDetectionDuration,
												   XnFloat fMaximumAllowedVelocity,
												   const XnChar* strName);
XNV_NITE_CONTROLS_C_API XnStatus xnvSteadyDetector_Destroy(XnVObjectID id);
XNV_NITE_CONTROLS_C_API XnStatus xnvSteadyDetector_RegisterSteady(XnVObjectID id, SteadyCB CB,
																void* pCookie,
																XnCallbackHandle* pHandle);
XNV_NITE_CONTROLS_C_API XnStatus xnvSteadyDetector_UnregisterSteady(XnVObjectID id, XnCallbackHandle hCB);
XNV_NITE_CONTROLS_C_API XnStatus xnvSteadyDetector_RegisterNotSteady(XnVObjectID id, NotSteadyCB CB,
														   void* pCookie, XnCallbackHandle* pHandle);
XNV_NITE_CONTROLS_C_API XnStatus xnvSteadyDetector_UnregisterNotSteady(XnVObjectID id, XnCallbackHandle hCB);
XNV_NITE_CONTROLS_C_API XnStatus xnvSteadyDetector_Reset(XnVObjectID id);
XNV_NITE_CONTROLS_C_API XnStatus xnvSteadyDetector_GetDetectionDuration(XnVObjectID id, XnUInt32* pnDuration);
XNV_NITE_CONTROLS_C_API XnStatus xnvSteadyDetector_SetDetectionDuration(XnVObjectID id, XnUInt32 nDuration);
XNV_NITE_CONTROLS_C_API XnStatus xnvSteadyDetector_GetMaximumStdDevForSteady(XnVObjectID id, XnFloat* pfStdDev);
XNV_NITE_CONTROLS_C_API XnStatus xnvSteadyDetector_SetMaximumStdDevForSteady(XnVObjectID id, XnFloat fStdDev);
XNV_NITE_CONTROLS_C_API XnStatus xnvSteadyDetector_GetMinimumStdDevForNotSteady(XnVObjectID id, XnFloat* pfStdDev);
XNV_NITE_CONTROLS_C_API XnStatus xnvSteadyDetector_SetMinimumStdDevForNotSteady(XnVObjectID id, XnFloat fStdDev);

// XnVSwipeDetector
typedef void (XN_CALLBACK_TYPE *SwipeCB)(XnFloat fVelocity, XnFloat fAngle, void* pCookie);
typedef void (XN_CALLBACK_TYPE *GeneralSwipeCB)(XnVDirection eDir, XnFloat fVelocity, XnFloat fAngle,
												void* pCookie);

XNV_NITE_CONTROLS_C_API XnVObjectID xnvSwipeDetector_Create(XnBool bUseSteadyBeforeSwipe, const XnChar* strName);
XNV_NITE_CONTROLS_C_API XnStatus xnvSwipeDetector_Destroy(XnVObjectID id);
XNV_NITE_CONTROLS_C_API XnStatus xnvSwipeDetector_Reset(XnVObjectID id);
XNV_NITE_CONTROLS_C_API XnStatus xnvSwipeDetector_RegisterSwipeUp(XnVObjectID id, SwipeCB pCB,
																void* pCookie,
																XnCallbackHandle* pHandle);
XNV_NITE_CONTROLS_C_API XnStatus xnvSwipeDetector_RegisterSwipeDown(XnVObjectID id, SwipeCB pCB,
																  void* pCookie,
																  XnCallbackHandle* pHandle);
XNV_NITE_CONTROLS_C_API XnStatus xnvSwipeDetector_RegisterSwipeLeft(XnVObjectID id, SwipeCB pCB,
																  void* pCookie,
																  XnCallbackHandle* pHandle);
XNV_NITE_CONTROLS_C_API XnStatus xnvSwipeDetector_RegisterSwipeRight(XnVObjectID id, SwipeCB pCB,
																   void* pCookie,
																   XnCallbackHandle* pHandle);
XNV_NITE_CONTROLS_C_API XnStatus xnvSwipeDetector_RegisterSwipe(XnVObjectID id, GeneralSwipeCB pCB,
															  void* pCookie,
															  XnCallbackHandle* pHandle);
XNV_NITE_CONTROLS_C_API XnStatus xnvSwipeDetector_UnregisterSwipeUp(XnVObjectID id, XnCallbackHandle handle);
XNV_NITE_CONTROLS_C_API XnStatus xnvSwipeDetector_UnregisterSwipeDown(XnVObjectID id, XnCallbackHandle handle);
XNV_NITE_CONTROLS_C_API XnStatus xnvSwipeDetector_UnregisterSwipeLeft(XnVObjectID id, XnCallbackHandle handle);
XNV_NITE_CONTROLS_C_API XnStatus xnvSwipeDetector_UnregisterSwipeRight(XnVObjectID id, XnCallbackHandle handle);
XNV_NITE_CONTROLS_C_API XnStatus xnvSwipeDetector_UnregisterSwipe(XnVObjectID id, XnCallbackHandle handle);
XNV_NITE_CONTROLS_C_API XnStatus xnvSwipeDetector_SetMotionSpeedThreshold(XnVObjectID id, XnFloat fThreshold);
XNV_NITE_CONTROLS_C_API XnStatus xnvSwipeDetector_GetMotionSpeedThreshold(XnVObjectID id, XnFloat* pfThreshold);
XNV_NITE_CONTROLS_C_API XnStatus xnvSwipeDetector_SetMotionTime(XnVObjectID id, XnUInt32 nTime);
XNV_NITE_CONTROLS_C_API XnStatus xnvSwipeDetector_GetMotionTime(XnVObjectID id, XnUInt32* pnTime);
XNV_NITE_CONTROLS_C_API XnStatus xnvSwipeDetector_SetXAngleThreshold(XnVObjectID id, XnFloat fAngle);
XNV_NITE_CONTROLS_C_API XnStatus xnvSwipeDetector_GetXAngleThreshold(XnVObjectID id, XnFloat* pfAngle);
XNV_NITE_CONTROLS_C_API XnStatus xnvSwipeDetector_SetYAngleThreshold(XnVObjectID id, XnFloat fAngle);
XNV_NITE_CONTROLS_C_API XnStatus xnvSwipeDetector_GetYAngleThreshold(XnVObjectID id, XnFloat* pfAngle);
XNV_NITE_CONTROLS_C_API XnStatus xnvSwipeDetector_SetSteadyMaxStdDev(XnVObjectID id, XnFloat fStdDev);
XNV_NITE_CONTROLS_C_API XnStatus xnvSwipeDetector_GetSteadyMaxStdDev(XnVObjectID id, XnFloat* pfStdDev);
XNV_NITE_CONTROLS_C_API XnStatus xnvSwipeDetector_SetSteadyDuration(XnVObjectID id, XnUInt32 nDuration);
XNV_NITE_CONTROLS_C_API XnStatus xnvSwipeDetector_GetSteadyDuration(XnVObjectID id, XnUInt32* pnDuration);
XNV_NITE_CONTROLS_C_API XnStatus xnvSwipeDetector_SetUseSteady(XnVObjectID id, XnBool bUse);
XNV_NITE_CONTROLS_C_API XnStatus xnvSwipeDetector_GetUseSteady(XnVObjectID id, XnBool* pbUse);

// XnVWaveDetector
typedef void (XN_CALLBACK_TYPE *WaveCB)(void* pCookie);

XNV_NITE_CONTROLS_C_API XnVObjectID xnvWaveDetector_Create(const XnChar* strName);
XNV_NITE_CONTROLS_C_API XnStatus xnvWaveDetector_Destroy(XnVObjectID id);
XNV_NITE_CONTROLS_C_API XnStatus xnvWaveDetector_RegisterWave(XnVObjectID id, WaveCB pCB,
															void* pCookie,
															XnCallbackHandle* pHandle);
XNV_NITE_CONTROLS_C_API XnStatus xnvWaveDetector_UnregisterWave(XnVObjectID id, XnCallbackHandle handle);
XNV_NITE_CONTROLS_C_API XnStatus xnvWaveDetector_Reset(XnVObjectID id);
XNV_NITE_CONTROLS_C_API XnStatus xnvWaveDetector_SetFlipCount(XnVObjectID id, XnInt32 nFlipCount);
XNV_NITE_CONTROLS_C_API XnStatus xnvWaveDetector_SetMinLength(XnVObjectID id, XnInt32 nMinLength);
XNV_NITE_CONTROLS_C_API XnStatus xnvWaveDetector_SetMaxDeviation(XnVObjectID id, XnInt32 nMaxDeviation);
XNV_NITE_CONTROLS_C_API XnStatus xnvWaveDetector_GetFlipCount(XnVObjectID id, XnInt32* pnFlipCount);
XNV_NITE_CONTROLS_C_API XnStatus xnvWaveDetector_GetMinLength(XnVObjectID id, XnInt32* pnMinLength);
XNV_NITE_CONTROLS_C_API XnStatus xnvWaveDetector_GetMaxDeviation(XnVObjectID id, XnInt32* pnMaxDeviation);

// XnVMessageGenerator
XNV_NITE_CONTROLS_C_API XnStatus xnvMessageGenerator_AddListener(XnVObjectID id, XnVObjectID listener);
XNV_NITE_CONTROLS_C_API XnStatus xnvMessageGenerator_RemoveListener(XnVObjectID id, XnVObjectID listener);
XNV_NITE_CONTROLS_C_API const XnChar* xnvMessageGenerator_GetName(XnVObjectID id);
XNV_NITE_CONTROLS_C_API XnStatus xnvMessageGenerator_Generate(XnVObjectID id, XnVObjectID pMessage);

// XnVMessageListener
typedef void (XN_CALLBACK_TYPE* ActivateCB)(void* pCookie);
typedef void (XN_CALLBACK_TYPE* DeactivateCB)(void* pCookie);
typedef void (XN_CALLBACK_TYPE* UpdateCB)(XnVObjectID pMessage, void* pCookie);

XNV_NITE_CONTROLS_C_API XnStatus xnvMessageListener_Update(XnVObjectID id, XnVObjectID pMessage);
XNV_NITE_CONTROLS_C_API XnStatus xnvMessageListener_RegisterActivate(XnVObjectID id, ActivateCB CB,
																   void* pCookie,
																   XnCallbackHandle* pHandle);
XNV_NITE_CONTROLS_C_API XnStatus xnvMessageListener_RegisterDeactivate(XnVObjectID id, DeactivateCB CB,
																	 void* pCookie,
																	 XnCallbackHandle* pHandle);
XNV_NITE_CONTROLS_C_API XnStatus xnvMessageListener_UnregisterActivate(XnVObjectID id, XnCallbackHandle hCB);
XNV_NITE_CONTROLS_C_API XnStatus xnvMessageListener_UnregisterDeactivate(XnVObjectID id, XnCallbackHandle hCB);
XNV_NITE_CONTROLS_C_API XnStatus xnvMessageListener_RegisterUpdate(XnVObjectID id, UpdateCB CB,
																 void* pCookie,
																 XnCallbackHandle* pHandle);
XNV_NITE_CONTROLS_C_API XnStatus xnvMessageListener_UnregisterUpdate(XnVObjectID id, XnCallbackHandle hCB);

// XnVPointControl
#include "XnVHandPointContext.h"
typedef void (XN_CALLBACK_TYPE*PointCreateCB)(const XnVHandPointContext* pContext, void* pCookie);
typedef void (XN_CALLBACK_TYPE*PointUpdateCB)(const XnVHandPointContext* pContext, void* pCookie);
typedef void (XN_CALLBACK_TYPE*PointDestroyCB)(XnUInt32 nID, void* pCookie);
typedef void (XN_CALLBACK_TYPE*PrimaryPointCreateCB)(const XnVHandPointContext* pContext,
													 const XnPoint3D* ptFocus, void* pCookie);
typedef void (XN_CALLBACK_TYPE*PrimaryPointUpdateCB)(const XnVHandPointContext* pContext, void* pCookie);
typedef void (XN_CALLBACK_TYPE*PrimaryPointReplaceCB)(XnUInt32 nOldId,
													  const XnVHandPointContext* pContext, void* pCookie);
typedef void (XN_CALLBACK_TYPE*PrimaryPointDestroyCB)(XnUInt32 nID, void* pCookie);
typedef void (XN_CALLBACK_TYPE*NoPointsCB)(void* pCookie);

XNV_NITE_CONTROLS_C_API XnVObjectID xnvPointControl_Create(const XnChar* strName);
XNV_NITE_CONTROLS_C_API XnStatus xnvPointControl_Destroy(XnVObjectID id);
XNV_NITE_CONTROLS_C_API XnStatus xnvPointControl_RegisterPointCreate(XnVObjectID id, PointCreateCB CB,
																   void* pCookie,
																   XnCallbackHandle* pHandle);
XNV_NITE_CONTROLS_C_API XnStatus xnvPointControl_RegisterPointUpdate(XnVObjectID id, PointUpdateCB CB,
																   void* pCookie,
																   XnCallbackHandle* pHandle);
XNV_NITE_CONTROLS_C_API XnStatus xnvPointControl_RegisterPointDestroy(XnVObjectID id, PointDestroyCB CB,
																	void* pCookie,
																	XnCallbackHandle* pHandle);
XNV_NITE_CONTROLS_C_API XnStatus xnvPointControl_RegisterPrimaryPointCreate(XnVObjectID id,
																		  PrimaryPointCreateCB CB,
																		  void* pCookie,
																		  XnCallbackHandle* pHandle);
XNV_NITE_CONTROLS_C_API XnStatus xnvPointControl_RegisterPrimaryPointUpdate(XnVObjectID id,
																		  PrimaryPointUpdateCB CB,
																		  void* pCookie,
																		  XnCallbackHandle* pHandle);
XNV_NITE_CONTROLS_C_API XnStatus xnvPointControl_RegisterPrimaryPointReplace(XnVObjectID id,
																		   PrimaryPointReplaceCB CB,
																		   void* pCookie,
																		   XnCallbackHandle* pHandle);
XNV_NITE_CONTROLS_C_API XnStatus xnvPointControl_RegisterPrimaryPointDestroy(XnVObjectID id,
																		   PrimaryPointDestroyCB CB,
																		   void* pCookie,
																		   XnCallbackHandle* pHandle);
XNV_NITE_CONTROLS_C_API XnStatus xnvPointControl_RegisterNoPoints(XnVObjectID id, NoPointsCB CB,
																void* pCookie,
																XnCallbackHandle* pHandle);
XNV_NITE_CONTROLS_C_API XnStatus xnvPointControl_UnregisterPointCreate(XnVObjectID id, XnCallbackHandle hCB);
XNV_NITE_CONTROLS_C_API XnStatus xnvPointControl_UnregisterPointUpdate(XnVObjectID id, XnCallbackHandle hCB);
XNV_NITE_CONTROLS_C_API XnStatus xnvPointControl_UnregisterPointDestroy(XnVObjectID id, XnCallbackHandle hCB);
XNV_NITE_CONTROLS_C_API XnStatus xnvPointControl_UnregisterPrimaryPointCreate(XnVObjectID id, XnCallbackHandle hCB);
XNV_NITE_CONTROLS_C_API XnStatus xnvPointControl_UnregisterPrimaryPointUpdate(XnVObjectID id, XnCallbackHandle hCB);
XNV_NITE_CONTROLS_C_API XnStatus xnvPointControl_UnregisterPrimaryPointReplace(XnVObjectID id, XnCallbackHandle hCB);
XNV_NITE_CONTROLS_C_API XnStatus xnvPointControl_UnregisterPrimaryPointDestroy(XnVObjectID id, XnCallbackHandle hCB);
XNV_NITE_CONTROLS_C_API XnStatus xnvPointControl_UnregisterNoPoints(XnVObjectID id, XnCallbackHandle hCB);
XNV_NITE_CONTROLS_C_API XnStatus xnvPointControl_GetPrimaryID(XnVObjectID id, XnUInt32* pnPrimary);

#if __cplusplus
};
#endif

#endif // _XNV_C_NITE
