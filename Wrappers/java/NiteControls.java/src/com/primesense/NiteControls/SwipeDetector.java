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
import org.openni.*;

public class SwipeDetector extends PointControl
{
	public SwipeDetector(String name) throws org.openni.GeneralException
	{
		super(NativeMethods.xnvSwipeDetector_Create(true, name));

		swipeEvent = new Observable<DirectionVelocityAngleEventArgs>()
		{
			@Override
			protected int registerNative(OutArg<Long> hCallback) 
			{
				return NativeMethods.xnvSwipeDetector_RegisterGeneralSwipe(toNative(), this, "callback", hCallback);
			}

			@Override
			protected void unregisterNative(long hCallback) 
			{
				NativeMethods.xnvSwipeDetector_UnregisterGeneralSwipe(toNative(), hCallback);
			}
			
			@SuppressWarnings("unused")
			public void callback(int dir, float velocity, float angle)
			{
				notify(new DirectionVelocityAngleEventArgs(Direction.fromNative(dir), velocity, angle));
			}
		};
		swipeUpEvent = new Observable<VelocityAngleEventArgs>()
		{
			@Override
			protected int registerNative(OutArg<Long> hCallback) 
			{
				return NativeMethods.xnvSwipeDetector_RegisterSwipeUp(toNative(), this, "callback", hCallback);
			}

			@Override
			protected void unregisterNative(long hCallback) 
			{
				NativeMethods.xnvSwipeDetector_UnregisterSwipeUp(toNative(), hCallback);
			}
			
			@SuppressWarnings("unused")
			public void callback(float velocity, float angle)
			{
				notify(new VelocityAngleEventArgs(velocity, angle));
			}
		};
		swipeDownEvent = new Observable<VelocityAngleEventArgs>()
		{
			@Override
			protected int registerNative(OutArg<Long> hCallback) 
			{
				return NativeMethods.xnvSwipeDetector_RegisterSwipeDown(toNative(), this, "callback", hCallback);
			}

			@Override
			protected void unregisterNative(long hCallback) 
			{
				NativeMethods.xnvSwipeDetector_UnregisterSwipeDown(toNative(), hCallback);
			}
			
			@SuppressWarnings("unused")
			public void callback(float velocity, float angle)
			{
				notify(new VelocityAngleEventArgs(velocity, angle));
			}
		};
		swipeLeftEvent = new Observable<VelocityAngleEventArgs>()
		{
			@Override
			protected int registerNative(OutArg<Long> hCallback) 
			{
				return NativeMethods.xnvSwipeDetector_RegisterSwipeLeft(toNative(), this, "callback", hCallback);
			}

			@Override
			protected void unregisterNative(long hCallback) 
			{
				NativeMethods.xnvSwipeDetector_UnregisterSwipeLeft(toNative(), hCallback);
			}
			
			@SuppressWarnings("unused")
			public void callback(float velocity, float angle)
			{
				notify(new VelocityAngleEventArgs(velocity, angle));
			}
		};
		swipeRightEvent = new Observable<VelocityAngleEventArgs>()
		{
			@Override
			protected int registerNative(OutArg<Long> hCallback) 
			{
				return NativeMethods.xnvSwipeDetector_RegisterSwipeRight(toNative(), this, "callback", hCallback);
			}

			@Override
			protected void unregisterNative(long hCallback) 
			{
				NativeMethods.xnvSwipeDetector_UnregisterSwipeRight(toNative(), hCallback);
			}
			
			@SuppressWarnings("unused")
			public void callback(float velocity, float angle)
			{
				notify(new VelocityAngleEventArgs(velocity, angle));
			}
		};
	}
	public SwipeDetector() throws org.openni.GeneralException
	{
		this("SwipeDetector");
	}

	protected void freeObject()
	{
		super.freeObject();

		NativeMethods.xnvSwipeDetector_Destroy(toNative());
	}
	
	public void setMotionSpeedThreshold(float velocity) throws StatusException
	{
		int status = NativeMethods.xnvSwipeDetector_SetMotionSpeedThreshold(toNative(), velocity);
		org.openni.WrapperUtils.throwOnError(status);
	}
	public void setMotionTime(int duration) throws StatusException
	{
		int status = NativeMethods.xnvSwipeDetector_SetMotionTime(toNative(), duration);
		org.openni.WrapperUtils.throwOnError(status);
	}
	public void setXAngleThreshold(float angle) throws StatusException
	{
		int status = NativeMethods.xnvSwipeDetector_SetXAngleThreshold(toNative(), angle);
		org.openni.WrapperUtils.throwOnError(status);
	}
	public void setYAngleThreshold(float angle) throws StatusException
	{
		int status = NativeMethods.xnvSwipeDetector_SetYAngleThreshold(toNative(), angle);
		org.openni.WrapperUtils.throwOnError(status);
	}
	public void setSteadyMaxDeviation(float deviation) throws StatusException
	{
		int status = NativeMethods.xnvSwipeDetector_SetSteadyMaxStdDev(toNative(), deviation);
		org.openni.WrapperUtils.throwOnError(status);
	}
	public void setSteadyDuration(int duration) throws StatusException
	{
		int status = NativeMethods.xnvSwipeDetector_SetSteadyDuration(toNative(), duration);
		org.openni.WrapperUtils.throwOnError(status);
	}
	public void setUseSteady(boolean use) throws StatusException
	{
		int status = NativeMethods.xnvSwipeDetector_SetUseSteady(toNative(), use);
		org.openni.WrapperUtils.throwOnError(status);
	}

	public float getMotionSpeedThreshold() throws StatusException
	{
		OutArg<Float> velocity = new OutArg<Float>();
		int status = NativeMethods.xnvSwipeDetector_GetMotionSpeedThreshold(toNative(), velocity);
		org.openni.WrapperUtils.throwOnError(status);
		return velocity.value;
	}
	public int getMotionTime() throws StatusException
	{
		OutArg<Integer> duration = new OutArg<Integer>();
		int status = NativeMethods.xnvSwipeDetector_GetMotionTime(toNative(), duration);
		org.openni.WrapperUtils.throwOnError(status);
		return duration.value;
	}
	public float getXAngleThreshold() throws StatusException
	{
		OutArg<Float> angle = new OutArg<Float>();
		int status = NativeMethods.xnvSwipeDetector_GetXAngleThreshold(toNative(), angle);
		org.openni.WrapperUtils.throwOnError(status);
		return angle.value;
	}
	public float getYAngleThreshold() throws StatusException
	{
		OutArg<Float> angle = new OutArg<Float>();
		int status = NativeMethods.xnvSwipeDetector_GetYAngleThreshold(toNative(), angle);
		org.openni.WrapperUtils.throwOnError(status);
		return angle.value;
	}
	public float getSteadyMaxDeviation() throws StatusException
	{
		OutArg<Float> deviation = new OutArg<Float>();
		int status = NativeMethods.xnvSwipeDetector_GetSteadyMaxStdDev(toNative(), deviation);
		org.openni.WrapperUtils.throwOnError(status);
		return deviation.value;
	}
	public int getSteadyDuration() throws StatusException
	{
		OutArg<Integer> duration = new OutArg<Integer>();
		int status = NativeMethods.xnvSwipeDetector_GetSteadyDuration(toNative(), duration);
		org.openni.WrapperUtils.throwOnError(status);
		return duration.value;
	}
	public boolean getUseSteady() throws StatusException
	{
		OutArg<Boolean> use = new OutArg<Boolean>();
		int status = NativeMethods.xnvSwipeDetector_GetUseSteady(toNative(), use);
		org.openni.WrapperUtils.throwOnError(status);
		return use.value;
	}
	
	// Events
	public IObservable<DirectionVelocityAngleEventArgs> getGeneralSwipeEvent()
	{
		return swipeEvent;
	}

	public IObservable<VelocityAngleEventArgs> getSwipeUpEvent()
	{
		return swipeUpEvent;
	}
	public IObservable<VelocityAngleEventArgs> getSwipeDownEvent()
	{
		return swipeDownEvent;
	}
	public IObservable<VelocityAngleEventArgs> getSwipeLeftEvent()
	{
		return swipeLeftEvent;
	}
	public IObservable<VelocityAngleEventArgs> getSwipeRightEvent()
	{
		return swipeRightEvent;
	}

	private Observable<DirectionVelocityAngleEventArgs> swipeEvent;
	private Observable<VelocityAngleEventArgs> swipeUpEvent;
	private Observable<VelocityAngleEventArgs> swipeDownEvent;
	private Observable<VelocityAngleEventArgs> swipeLeftEvent;
	private Observable<VelocityAngleEventArgs> swipeRightEvent;
	
}
