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
package com.primesense.NiteControls;
import org.openni.OutArg;
import org.openni.Point3D;

class NativeMethods
{
	static {
        System.loadLibrary("NiteControls.jni");
	}

	// Broadcaster
	static native long xnvBroadcaster_Create(String name);//OK
	static native int xnvBroadcaster_Destroy(long id);

	// MessageGenerator
	static native int xnvMessageGenerator_AddListener(long id, long listener);//OK
	static native int xnvMessageGenerator_RemoveListener(long id, long listener);
	static native int xnvMessageGenerator_Generate(long id, long message);
	
	// MessageListener
 	static native int xnvMessageListener_Update(long id, long message);
	static native int xnvMessageListener_RegisterActivate(long id, Object obj, String callbackName, OutArg<Long> hCallback);
	static native int xnvMessageListener_RegisterDeactivate(long id, Object obj, String callbackName, OutArg<Long> hCallback);
	static native int xnvMessageListener_RegisterUpdate(long id, Object obj, String callbackName, OutArg<Long> hCallback);
	static native int xnvMessageListener_UnregisterActivate(long id, long hCallback);
	static native int xnvMessageListener_UnregisterDeactivate(long id, long hCallback);
	static native int xnvMessageListener_UnregisterUpdate(long id, long hCallback);

	// FlowRouter
	static native long xnvFlowRouter_Create(String name);
	static native int xnvFlowRouter_Destroy(long id);
	static native int xnvFlowRouter_SetActive(long id, long listener);
	static native int xnvFlowRouter_GetActive(long id, OutArg<Long> pListener);

	// PointControl
	static native long xnvPointControl_Create(String name);
	static native int xnvPointControl_Destroy(long id);
	static native int xnvPointControl_RegisterPointCreate(long id, Object obj, String callbackName, OutArg<Long> hCallback);
	static native int xnvPointControl_RegisterPointUpdate(long id, Object obj, String callbackName, OutArg<Long> hCallback);
	static native int xnvPointControl_RegisterPointDestroy(long id, Object obj, String callbackName, OutArg<Long> hCallback);
	static native int xnvPointControl_RegisterPrimaryPointCreate(long id, Object obj, String callbackName, OutArg<Long> hCallback);
	static native int xnvPointControl_RegisterPrimaryPointUpdate(long id, Object obj, String callbackName, OutArg<Long> hCallback);
	static native int xnvPointControl_RegisterPrimaryPointReplace(long id, Object obj, String callbackName, OutArg<Long> hCallback);
	static native int xnvPointControl_RegisterPrimaryPointDestroy(long id, Object obj, String callbackName, OutArg<Long> hCallback);
	static native int xnvPointControl_RegisterNoPoints(long id, Object obj, String callbackName, OutArg<Long> hCallback);
	static native int xnvPointControl_UnregisterPointCreate(long id, long hCallback);
	static native int xnvPointControl_UnregisterPointUpdate(long id, long hCallback);
	static native int xnvPointControl_UnregisterPointDestroy(long id, long hCallback);
	static native int xnvPointControl_UnregisterPrimaryPointCreate(long id, long hCallback);
	static native int xnvPointControl_UnregisterPrimaryPointUpdate(long id, long hCallback);
	static native int xnvPointControl_UnregisterPrimaryPointReplace(long id, long hCallback);
	static native int xnvPointControl_UnregisterPrimaryPointDestroy(long id, long hCallback);
	static native int xnvPointControl_UnregisterNoPoints(long id, long hCallback);
	static native int xnvPointControl_GetPrimaryID(long id, OutArg<Integer> pPrimary);

	// PushDetector
	static native long xnvPushDetector_Create(String name);
	static native int xnvPushDetector_Destroy(long id);
	static native int xnvPushDetector_RegisterPush(long id, Object obj, String callbackName, OutArg<Long> hCallback);
	static native int xnvPushDetector_RegisterStabilized(long id, Object obj, String callbackName, OutArg<Long> hCallback);
	static native int xnvPushDetector_UnregisterPush(long id, long hCallback);
	static native int xnvPushDetector_UnregisterStabilized(long id, long hCallback);
	static native int xnvPushDetector_Reset(long id);
	static native int xnvPushDetector_SetPushImmediateMinimumVelocity(long id, float velocity);
	static native int xnvPushDetector_SetPushImmediateDuration(long id, int duration);
	static native int xnvPushDetector_SetPushImmediateOffset(long id, int offset);
	static native int xnvPushDetector_SetPushPreviousMinimumVelocity(long id, float velocity);
	static native int xnvPushDetector_SetPushPreviousDuration(long id, int duration);
	static native int xnvPushDetector_SetPushPreviousOffset(long id, int offset);
	static native int xnvPushDetector_SetPushMaximumAngleBetweenImmediateAndZ(long id, float angle);
	static native int xnvPushDetector_SetPushMinimumAngleBetweenImmediateAndPrevious(long id, float angle);
	static native int xnvPushDetector_SetStableMaximumVelocity(long id, float velocity);
	static native int xnvPushDetector_SetStableDuration(long id, int duration);
	static native int xnvPushDetector_GetPushImmediateMinimumVelocity(long id, OutArg<Float> pVelocity);
	static native int xnvPushDetector_GetPushImmediateDuration(long id, OutArg<Integer> pDuration);
	static native int xnvPushDetector_GetPushImmediateOffset(long id, OutArg<Integer> pOffset);
	static native int xnvPushDetector_GetPushPreviousMinimumVelocity(long id, OutArg<Float> pVelocity);
	static native int xnvPushDetector_GetPushPreviousDuration(long id, OutArg<Integer> pDuration);
	static native int xnvPushDetector_GetPushPreviousOffset(long id, OutArg<Integer> pOffset);
	static native int xnvPushDetector_GetPushMaximumAngleBetweenImmediateAndZ(long id, OutArg<Float> pAngle);
	static native int xnvPushDetector_GetPushMinimumAngleBetweenImmediateAndPrevious(long id, OutArg<Float> pAngle);
	static native int xnvPushDetector_GetStableMaximumVelocity(long id, OutArg<Float> pVelocity);
	static native int xnvPushDetector_GetStableDuration(long id, OutArg<Integer> pDuration);

	// SelectableSlider1D
	static native long xnvSelectableSlider1D_Create(int items, int axis, boolean recenter, float sliderSize, float firstValue, String name);
	static native int xnvSelectableSlider1D_Destroy(long id);
	static native int xnvSelectableSlider1D_Reposition(long id, float x, float y, float z);
	static native int xnvSelectableSlider1D_GetCenter(long id, OutArg<Point3D> point);
	static native int xnvSelectableSlider1D_RegisterItemHover(long id, Object obj, String callbackName, OutArg<Long> hCallback);
	static native int xnvSelectableSlider1D_RegisterItemSelect(long id, Object obj, String callbackName, OutArg<Long> hCallback);
	static native int xnvSelectableSlider1D_RegisterValueChange(long id, Object obj, String callbackName, OutArg<Long> hCallback);
	static native int xnvSelectableSlider1D_RegisterOffAxisMovement(long id, Object obj, String callbackName, OutArg<Long> hCallback);
	static native int xnvSelectableSlider1D_UnregisterItemHover(long id, long hCallback);
	static native int xnvSelectableSlider1D_UnregisterItemSelect(long id, long hCallback);
	static native int xnvSelectableSlider1D_UnregisterValueChange(long id, long hCallback);
	static native int xnvSelectableSlider1D_UnregisterOffAxisMovement(long id, long hCallback);
	static native int xnvSelectableSlider1D_SetItemCount(long id, int items);
	static native int xnvSelectableSlider1D_GetItemCount(long id, OutArg<Integer> pItems);
	static native int xnvSelectableSlider1D_SetSliderSize(long id, float size);
	static native int xnvSelectableSlider1D_GetSliderSize(long id, OutArg<Float> pSize);
	static native int xnvSelectableSlider1D_SetValueChangeOnOffAxis(long id, boolean report);
	static native int xnvSelectableSlider1D_GetValueChangeOnOffAxis(long id, OutArg<Boolean> pReport);

	// SelectableSlider2D
	static native long xnvSelectableSlider2D_Create(int itemsX, int itemsY, float sliderSizeX, float sliderSizeY, String name);//OK
	static native int xnvSelectableSlider2D_Destroy(long id);
	static native int xnvSelectableSlider2D_Reposition(long id, float x, float y, float z);
	static native int xnvSelectableSlider2D_GetCenter(long id, OutArg<Point3D> point);
	static native int xnvSelectableSlider2D_RegisterItemHover(long id, Object obj, String callbackName, OutArg<Long> hCallback);//OK
	static native int xnvSelectableSlider2D_RegisterItemSelect(long id, Object obj, String callbackName, OutArg<Long> hCallback);//OK
	static native int xnvSelectableSlider2D_RegisterValueChange(long id, Object obj, String callbackName, OutArg<Long> hCallback);//OK
	static native int xnvSelectableSlider2D_RegisterOffAxisMovement(long id, Object obj, String callbackName, OutArg<Long> hCallback);
	static native int xnvSelectableSlider2D_UnregisterItemHover(long id, long hCallback);
	static native int xnvSelectableSlider2D_UnregisterItemSelect(long id, long hCallback);
	static native int xnvSelectableSlider2D_UnregisterValueChange(long id, long hCallback);
	static native int xnvSelectableSlider2D_UnregisterOffAxisMovement(long id, long hCallback);
	static native int xnvSelectableSlider2D_GetItemXCount(long id, OutArg<Integer> pItems);
	static native int xnvSelectableSlider2D_SetItemXCount(long id, int items);
	static native int xnvSelectableSlider2D_GetItemYCount(long id, OutArg<Integer> pItems);
	static native int xnvSelectableSlider2D_SetItemYCount(long id, int items);
//	static native int xnvSelectableSlider2D_SetItemCount(long id, int itemsX, int itemsY);
	static native int xnvSelectableSlider2D_GetSliderXSize(long id, OutArg<Float> pSize);
	static native int xnvSelectableSlider2D_GetSliderYSize(long id, OutArg<Float> pSize);
	static native int xnvSelectableSlider2D_SetSliderSize(long id, float sizeX, float sizeY);
	static native int xnvSelectableSlider2D_GetValueChangeOnOffAxis(long id, OutArg<Boolean> pReport);
	static native int xnvSelectableSlider2D_SetValueChangeOnOffAxis(long id, boolean report);

	// SessionManager
	static native long xnvSessionManager_Create(String name);//OK
	static native int xnvSessionManager_Destroy(long id);//OK
	static native int xnvSessionManager_Initialize(long id, long context, String focus); // OZOZ: NodeHandle tracker, focus, quickrefocus //OK
	static native int xnvSessionManager_Update(long id, long context);//OK
	static native int xnvSessionManager_AddGestureByContext(long id, long context, String name, OutArg<Integer> gestureID);
	static native int xnvSessionManager_RemoveGesture(long id, int gestureID);

	// SessionGenerator
	static native int xnvSessionGenerator_RegisterToSessionStart(long id, Object obj, String callbackName, OutArg<Long> hCallback);//OK
	static native int xnvSessionGenerator_RegisterToSessionEnd(long id, Object obj, String callbackName, OutArg<Long> hCallback);//OK
	static native int xnvSessionGenerator_RegisterToSessionFocusProgress(long id, Object obj, String callbackName, OutArg<Long> hCallback);//OK
	static native int xnvSessionGenerator_UnregisterFromSessionStart(long id, long hCallback);
	static native int xnvSessionGenerator_UnregisterFromSessionEnd(long id, long hCallback);
	static native int xnvSessionGenerator_UnregisterFromSessionFocusProgress(long id, long hCallback);
	static native int xnvSessionGenerator_LosePoint(long id, int pointId);
	static native int xnvSessionGenerator_LosePoints(long id);
	static native int xnvSessionGenerator_EndSession(long id);
	static native int xnvSessionGenerator_ForceSession(long id, float x, float y, float z);
	static native int xnvSessionGenerator_TrackPoint(long id, float x, float y, float z);
	static native int xnvSessionGenerator_IsInSession(long id, OutArg<Boolean> pInSession);
	static native int xnvSessionGenerator_GetFocusPoint(long id, OutArg<Point3D> pFocus);

	// SteadyDetector
	static native long xnvSteadyDetector_Create(int cooldown, int duration, float stddev, String name);//OK
	static native int xnvSteadyDetector_Destroy(long id);
	static native int xnvSteadyDetector_RegisterSteady(long id, Object obj, String callbackName, OutArg<Long> hCallback);//OK
	static native int xnvSteadyDetector_RegisterNotSteady(long id, Object obj, String callbackName, OutArg<Long> hCallback);
	static native int xnvSteadyDetector_UnregisterSteady(long id, long hCallback);
	static native int xnvSteadyDetector_UnregisterNotSteady(long id, long hCallback);
	static native int xnvSteadyDetector_Reset(long id);
	static native int xnvSteadyDetector_GetDetectionDuration(long id, OutArg<Integer> pDuration);
	static native int xnvSteadyDetector_GetMaximumStdDevForSteady(long id, OutArg<Float> pDeviation);
	static native int xnvSteadyDetector_GetMinimumStdDevForNotSteady(long id, OutArg<Float> pDeviation);
	static native int xnvSteadyDetector_SetMaximumStdDevForSteady(long id, float stddev);
	static native int xnvSteadyDetector_SetMinimumStdDevForNotSteady(long id, float stddev);
	static native int xnvSteadyDetector_SetDetectionDuration(long id, int duration);

	// SwipeDetector
	static native long xnvSwipeDetector_Create(boolean useSteady, String name);//OK
	static native int xnvSwipeDetector_Destroy(long id);
	static native int xnvSwipeDetector_Reset(long id);
	static native int xnvSwipeDetector_RegisterSwipeUp(long id, Object obj, String callbackName, OutArg<Long> hCallback);
	static native int xnvSwipeDetector_RegisterSwipeDown(long id, Object obj, String callbackName, OutArg<Long> hCallback);
	static native int xnvSwipeDetector_RegisterSwipeLeft(long id, Object obj, String callbackName, OutArg<Long> hCallback);
	static native int xnvSwipeDetector_RegisterSwipeRight(long id, Object obj, String callbackName, OutArg<Long> hCallback);
	static native int xnvSwipeDetector_RegisterGeneralSwipe(long id, Object obj, String callbackName, OutArg<Long> hCallback);//OK
	static native int xnvSwipeDetector_UnregisterSwipeUp(long id, long hCallback);
	static native int xnvSwipeDetector_UnregisterSwipeDown(long id, long hCallback);
	static native int xnvSwipeDetector_UnregisterSwipeLeft(long id, long hCallback);
	static native int xnvSwipeDetector_UnregisterSwipeRight(long id, long hCallback);
	static native int xnvSwipeDetector_UnregisterGeneralSwipe(long id, long hCallback);
	static native int xnvSwipeDetector_SetMotionSpeedThreshold(long id, float velocity);
	static native int xnvSwipeDetector_SetMotionTime(long id, int duration);
	static native int xnvSwipeDetector_SetXAngleThreshold(long id, float angle);
	static native int xnvSwipeDetector_SetYAngleThreshold(long id, float angel);
	static native int xnvSwipeDetector_SetSteadyMaxStdDev(long id, float stddev);
	static native int xnvSwipeDetector_SetSteadyDuration(long id, int duration);
	static native int xnvSwipeDetector_SetUseSteady(long id, boolean use);
	static native int xnvSwipeDetector_GetMotionSpeedThreshold(long id, OutArg<Float> pVelocity);
	static native int xnvSwipeDetector_GetMotionTime(long id, OutArg<Integer> pDuration);
	static native int xnvSwipeDetector_GetXAngleThreshold(long id, OutArg<Float> pAngle);
	static native int xnvSwipeDetector_GetYAngleThreshold(long id, OutArg<Float> pAngle);
	static native int xnvSwipeDetector_GetSteadyMaxStdDev(long id, OutArg<Float> pDeviation);
	static native int xnvSwipeDetector_GetSteadyDuration(long id, OutArg<Integer> pDuration);
	static native int xnvSwipeDetector_GetUseSteady(long id, OutArg<Boolean> pUse);

	// PointFilter
	static native long xnvPointFilter_Create(String name);
	static native int xnvPointFilter_Destroy(long id);
	static native int xnvPointFilter_OverridePrimary(long id, int newPrimary);

	// PointArea
	static native long xnvPointArea_Create(float minx, float miny, float minz, float maxx, float maxy, float maxz, boolean relative, String name);
	static native int xnvPointArea_Destroy(long id);
	static native int xnvPointArea_ChangeArea(long id, float minx, float miny, float minz, float maxx, float maxy, float maxz, boolean relative);
	static native int xnvPointArea_RemoveSilent(long id, long delay, long sessionManager);
	static native int xnvPointArea_RegisterPointSilenced(long id, Object obj, String callbackName, OutArg<Long> hCallback);
	static native int xnvPointArea_RegisterPointRevived(long id, Object obj, String callbackName, OutArg<Long> hCallback);
	static native int xnvPointArea_RegisterSilentPointRemoved(long id, Object obj, String callbackName, OutArg<Long> hCallback);
	static native int xnvPointArea_UnregisterPointSilenced(long id, long hCallback);
	static native int xnvPointArea_UnregisterPointRevived(long id, long hCallback);
	static native int xnvPointArea_UnregisterSilentPointRemoved(long id, long hCallback);

	// PointDenoiser
	static native long xnvPointDenoiser_Create(float distanceThreshold, String name);
	static native int xnvPointDenoiser_Destroy(long id);
	static native int xnvPointDenoiser_SetDistanceThreshold(long id, float distanceThreshold);
	static native int xnvPointDenoiser_GetDistanceThreshold(long id, OutArg<Float> pThreshold);
	static native int xnvPointDenoiser_SetCloseRatio(long id, float ratio);
	static native int xnvPointDenoiser_GetCloseRatio(long id, OutArg<Float> pRatio);
	static native int xnvPointDenoiser_SetFarRatio(long id, float ratio);
	static native int xnvPointDenoiser_GetFarRatio(long id, OutArg<Float> pRatio);

}
