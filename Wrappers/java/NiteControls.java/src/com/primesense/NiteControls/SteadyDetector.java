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

public class SteadyDetector extends PointControl
{
	public SteadyDetector(int duration, float stdDev) throws org.openni.GeneralException
	{
		super(NativeMethods.xnvSteadyDetector_Create(0, duration, stdDev, "Steady"));

		steadyEvent = new Observable<IdValueEventArgs>()
		{
			@Override
			protected int registerNative(OutArg<Long> hCallback) 
			{
				return NativeMethods.xnvSteadyDetector_RegisterSteady(toNative(), this, "callback", hCallback);
			}

			@Override
			protected void unregisterNative(long hCallback) 
			{
				NativeMethods.xnvSteadyDetector_UnregisterSteady(toNative(), hCallback);
			}
			
			@SuppressWarnings("unused")
			public void callback(int id, float deviation)
			{
				notify(new IdValueEventArgs(id, deviation));
			}
		};
		notSteadyEvent = new Observable<IdValueEventArgs>()
		{
			@Override
			protected int registerNative(OutArg<Long> hCallback) 
			{
				return NativeMethods.xnvSteadyDetector_RegisterNotSteady(toNative(), this, "callback", hCallback);
			}

			@Override
			protected void unregisterNative(long hCallback) 
			{
				NativeMethods.xnvSteadyDetector_UnregisterNotSteady(toNative(), hCallback);
			}
			
			@SuppressWarnings("unused")
			public void callback(int id, float deviation)
			{
				notify(new IdValueEventArgs(id, deviation));
			}
		};
	}

	public SteadyDetector() throws org.openni.GeneralException
	{
		this(250, 0.01f);
	}

	protected void freeObject()
	{
		super.freeObject();

		NativeMethods.xnvSteadyDetector_Destroy(toNative());
	}

	public void reset() throws StatusException
	{
		int status = NativeMethods.xnvSteadyDetector_Reset(toNative());
		org.openni.WrapperUtils.throwOnError(status);
	}
	public int getDetectionDuration() throws StatusException
	{
		OutArg<Integer> duration = new OutArg<Integer>();
		int status = NativeMethods.xnvSteadyDetector_GetDetectionDuration(toNative(), duration);
		org.openni.WrapperUtils.throwOnError(status);
		return duration.value;
	}
	public float getMaxDeviationForSteady() throws StatusException
	{
		OutArg<Float> deviation = new OutArg<Float>();
		int status = NativeMethods.xnvSteadyDetector_GetMaximumStdDevForSteady(toNative(), deviation);
		org.openni.WrapperUtils.throwOnError(status);
		return deviation.value;
	}
	public float getMinDeviationForNotSteady() throws StatusException
	{
		OutArg<Float> deviation = new OutArg<Float>();
		int status = NativeMethods.xnvSteadyDetector_GetMinimumStdDevForNotSteady(toNative(), deviation);
		org.openni.WrapperUtils.throwOnError(status);
		return deviation.value;
	}
	public void setDetectionDuration(int duration) throws StatusException
	{
		int status = NativeMethods.xnvSteadyDetector_SetDetectionDuration(toNative(), duration);
		org.openni.WrapperUtils.throwOnError(status);
	}
	public void setMaxDeviationForSteady(float deviation) throws StatusException
	{
		int status = NativeMethods.xnvSteadyDetector_SetMaximumStdDevForSteady(toNative(), deviation);
		org.openni.WrapperUtils.throwOnError(status);
	}
	public void setMinDeviationForNotSteady(float deviation) throws StatusException
	{
		int status = NativeMethods.xnvSteadyDetector_SetMinimumStdDevForNotSteady(toNative(), deviation);
		org.openni.WrapperUtils.throwOnError(status);
	}
	
	// Events
	public IObservable<IdValueEventArgs> getSteadyEvent()
	{
		return steadyEvent;
	}
	public IObservable<IdValueEventArgs> getNotSteadyEvent()
	{
		return notSteadyEvent;
	}
		
	private Observable<IdValueEventArgs> steadyEvent;
	private Observable<IdValueEventArgs> notSteadyEvent;
}
