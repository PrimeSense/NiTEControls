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
import org.openni.IObservable;
import org.openni.Observable;
import org.openni.OutArg;
import org.openni.StatusException;

public class PushDetector extends PointControl
{
	public PushDetector(String name) throws org.openni.GeneralException
	{
		super(NativeMethods.xnvPushDetector_Create(name));
	
		pushEvent = new Observable<VelocityAngleEventArgs>()
		{
			@Override
			protected int registerNative(OutArg<Long> hCallback) 
			{
				return NativeMethods.xnvPushDetector_RegisterPush(toNative(), this, "callback", hCallback);
			}

			@Override
			protected void unregisterNative(long hCallback) 
			{
				NativeMethods.xnvPushDetector_UnregisterPush(toNative(), hCallback);
			}
			
			@SuppressWarnings("unused")
			public void callback(float velocity, float angle)
			{
				notify(new VelocityAngleEventArgs(velocity, angle));
			}
		};		
		stabilizedEvent = new Observable<ValueEventArgs>()
		{
			@Override
			protected int registerNative(OutArg<Long> hCallback) 
			{
				return NativeMethods.xnvPushDetector_RegisterStabilized(toNative(), this, "callback", hCallback);
			}

			@Override
			protected void unregisterNative(long hCallback) 
			{
				NativeMethods.xnvPushDetector_UnregisterStabilized(toNative(), hCallback);
			}
			
			@SuppressWarnings("unused")
			public void callback(float velocity)
			{
				notify(new ValueEventArgs(velocity));
			}
		};		
	}
	public PushDetector() throws org.openni.GeneralException
	{
		this("PushDetector");
	}

	protected void freeObject()
	{
		super.freeObject();
		NativeMethods.xnvPushDetector_Destroy(toNative());	
	}

	public void reset() throws StatusException
	{
		int status = NativeMethods.xnvPushDetector_Reset(toNative());
		org.openni.WrapperUtils.throwOnError(status);
	}

	// PushImmediateMinimumVelocity
	public void setPushImmediateMinimumVelocity(float velocity) throws StatusException
	{
		int status = NativeMethods.xnvPushDetector_SetPushImmediateMinimumVelocity(toNative(), velocity);
		org.openni.WrapperUtils.throwOnError(status);
	}
	public float getPushImmediateMinimumVelocity() throws StatusException
	{
		OutArg<Float> velocity = new OutArg<Float>();
		int status = NativeMethods.xnvPushDetector_GetPushImmediateMinimumVelocity(toNative(), velocity);
		org.openni.WrapperUtils.throwOnError(status);		
		return velocity.value;
	}
	// PushImmediateDuration
	public void setPushImmediateDuration(int duration) throws StatusException
	{
		int status = NativeMethods.xnvPushDetector_SetPushImmediateDuration(toNative(), duration);
		org.openni.WrapperUtils.throwOnError(status);
	}
	public int getPushImmediateDuration() throws StatusException
	{
		OutArg<Integer> duration = new OutArg<Integer>();
		int status = NativeMethods.xnvPushDetector_GetPushImmediateDuration(toNative(), duration);
		org.openni.WrapperUtils.throwOnError(status);
		return duration.value;
	}
	// PushImmediateOffset
	public void setPushImmediateOffset(int offset) throws StatusException
	{
		int status = NativeMethods.xnvPushDetector_SetPushImmediateOffset(toNative(), offset);
		org.openni.WrapperUtils.throwOnError(status);
	}
	public int getPushImmediateOffset() throws StatusException
	{
		OutArg<Integer> offset = new OutArg<Integer>();
		int status = NativeMethods.xnvPushDetector_GetPushImmediateOffset(toNative(), offset);
		org.openni.WrapperUtils.throwOnError(status);
		return offset.value;
	}
	// PushPreviousMinimumVelocity
	public void setPushPreviousMinimumVelocity(float velocity) throws StatusException
	{
		int status = NativeMethods.xnvPushDetector_SetPushPreviousMinimumVelocity(toNative(), velocity);
		org.openni.WrapperUtils.throwOnError(status);
	}
	public float getPushPreviousMinimumVelocity() throws StatusException
	{
		OutArg<Float> velocity = new OutArg<Float>();
		int status = NativeMethods.xnvPushDetector_GetPushPreviousMinimumVelocity(toNative(), velocity);
		org.openni.WrapperUtils.throwOnError(status);
		return velocity.value;
	}
	// PushPreviousDuration
	public void setPushPreviousDuration(int duration) throws StatusException
	{
		int status = NativeMethods.xnvPushDetector_SetPushPreviousDuration(toNative(),duration);
		org.openni.WrapperUtils.throwOnError(status);
	}
	public int getPushPreviousDuration() throws StatusException
	{
		OutArg<Integer> duration = new OutArg<Integer>();
		int status = NativeMethods.xnvPushDetector_GetPushPreviousDuration(toNative(), duration);
		org.openni.WrapperUtils.throwOnError(status);
		return duration.value;
	}
	// PushPreviousOffset
	public void setPushPreviousOffset(int offset) throws StatusException
	{
		int status = NativeMethods.xnvPushDetector_SetPushPreviousOffset(toNative(), offset);
		org.openni.WrapperUtils.throwOnError(status);
	}
	public int getPushPreviousOffset() throws StatusException
	{
		OutArg<Integer> offset = new OutArg<Integer>();
		int status = NativeMethods.xnvPushDetector_GetPushPreviousOffset(toNative(), offset);
		org.openni.WrapperUtils.throwOnError(status);
		return offset.value;
	}
	// PushMaximumAngleBetweenImmediateAndZ
	public void setPushMaximumAngleBetweenImmediateAndZ(float angle) throws StatusException
	{
		int status = NativeMethods.xnvPushDetector_SetPushMaximumAngleBetweenImmediateAndZ(toNative(), angle);
		org.openni.WrapperUtils.throwOnError(status);
	}
	public float getPushMaximumAngleBetweenImmediateAndZ() throws StatusException
	{
		OutArg<Float> angle = new OutArg<Float>();
		int status = NativeMethods.xnvPushDetector_GetPushMaximumAngleBetweenImmediateAndZ(toNative(), angle);
		org.openni.WrapperUtils.throwOnError(status);
		return angle.value;
	}
	// PushMinimumAngleBetweenImmediateAndPrevious
	public void setPushMinimumAngleBetweenImmediateAndPrevious(float angle) throws StatusException
	{
		int status = NativeMethods.xnvPushDetector_SetPushMinimumAngleBetweenImmediateAndPrevious(toNative(), angle);
		org.openni.WrapperUtils.throwOnError(status);
	}
	public float getPushMinimumAngleBetweenImmediateAndPrevious() throws StatusException
	{
		OutArg<Float> angle = new OutArg<Float>();
		int status = NativeMethods.xnvPushDetector_GetPushMinimumAngleBetweenImmediateAndPrevious(toNative(), angle);
		org.openni.WrapperUtils.throwOnError(status);
		return angle.value;
	}
	// StableMaximumVelocity
	public void setStableMaximumVelocity(float velocity) throws StatusException
	{
		int status = NativeMethods.xnvPushDetector_SetStableMaximumVelocity(toNative(), velocity);
		org.openni.WrapperUtils.throwOnError(status);
	}
	public float getStableMaximumVelocity() throws StatusException
	{
		OutArg<Float> velocity = new OutArg<Float>();
		int status = NativeMethods.xnvPushDetector_GetStableMaximumVelocity(toNative(), velocity);
		org.openni.WrapperUtils.throwOnError(status);
		return velocity.value;
	}
	// StableDuration
	public void setStableDuration(int duration) throws StatusException
	{
		int status = NativeMethods.xnvPushDetector_SetStableDuration(toNative(), duration);
		org.openni.WrapperUtils.throwOnError(status);
	}
	public int getStableDuration() throws StatusException
	{
		OutArg<Integer> duration = new OutArg<Integer>();
		int status = NativeMethods.xnvPushDetector_GetStableDuration(toNative(), duration);
		org.openni.WrapperUtils.throwOnError(status);
		return duration.value;
	}
	
	// Events
	public IObservable<VelocityAngleEventArgs> getPushEvent()
	{
		return pushEvent;
	}
	public IObservable<ValueEventArgs> getStabilizedEvent()
	{
		return stabilizedEvent;
	}
	
	private Observable<VelocityAngleEventArgs> pushEvent;
	private Observable<ValueEventArgs> stabilizedEvent;
}
