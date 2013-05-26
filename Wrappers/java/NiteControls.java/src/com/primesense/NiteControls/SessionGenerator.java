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

public class SessionGenerator extends MessageGenerator
{
	public SessionGenerator(long id) throws org.openni.GeneralException, org.openni.StatusException
	{
		super(id);

		sessionStartEvent = new Observable<PointEventArgs>()
		{
			@Override
			protected int registerNative(OutArg<Long> hCallback) 
			{
				return NativeMethods.xnvSessionGenerator_RegisterToSessionStart(toNative(), this, "callback", hCallback);
			}

			@Override
			protected void unregisterNative(long hCallback) 
			{
				NativeMethods.xnvSessionGenerator_UnregisterFromSessionStart(toNative(), hCallback);
			}
			
			@SuppressWarnings("unused")
			public void callback(Point3D point)
			{
				notify(new PointEventArgs(point));
			}
		};
		sessionEndEvent = new Observable<NullEventArgs>()
		{
			@Override
			protected int registerNative(OutArg<Long> hCallback) 
			{
				return NativeMethods.xnvSessionGenerator_RegisterToSessionEnd(toNative(), this, "callback", hCallback);
			}

			@Override
			protected void unregisterNative(long hCallback) 
			{
				NativeMethods.xnvSessionGenerator_UnregisterFromSessionEnd(toNative(), hCallback);
			}
			
			@SuppressWarnings("unused")
			public void callback()
			{
				notify(new NullEventArgs());
			}
		};
		sessionFocusProgressEvent = new Observable<StringPointValueEventArgs>()
		{
			@Override
			protected int registerNative(OutArg<Long> hCallback) 
			{
				return NativeMethods.xnvSessionGenerator_RegisterToSessionFocusProgress(toNative(), this, "callback", hCallback);
			}

			@Override
			protected void unregisterNative(long hCallback) 
			{
				NativeMethods.xnvSessionGenerator_UnregisterFromSessionFocusProgress(toNative(), hCallback);
			}
			
			@SuppressWarnings("unused")
			public void callback(String focus, Point3D position, float progress)
			{
				notify(new StringPointValueEventArgs(focus, position, progress));
			}
		};
	}

	protected void freeObject()
	{
		super.freeObject();
	}

	public void LosePoint(int pointId) throws StatusException
	{
		int status = NativeMethods.xnvSessionGenerator_LosePoint(toNative(), pointId);
		org.openni.WrapperUtils.throwOnError(status);
	}
	public void LosePoints() throws StatusException
	{
		int status = NativeMethods.xnvSessionGenerator_LosePoints(toNative());
		org.openni.WrapperUtils.throwOnError(status);
	}
	public void EndSession() throws StatusException
	{
		int status = NativeMethods.xnvSessionGenerator_EndSession(toNative());
		org.openni.WrapperUtils.throwOnError(status);
	}
	public void ForceSession(Point3D point) throws StatusException
	{
		int status = NativeMethods.xnvSessionGenerator_ForceSession(toNative(), point.getX(), point.getY(), point.getZ());
		org.openni.WrapperUtils.throwOnError(status);
	}
	public void TrackPoint(Point3D point) throws StatusException
	{
		int status = NativeMethods.xnvSessionGenerator_TrackPoint(toNative(), point.getX(), point.getY(), point.getZ());
		org.openni.WrapperUtils.throwOnError(status);
	}
	public boolean IsInSession() throws StatusException
	{
		OutArg<Boolean> inSession = new OutArg<Boolean>();
		int status = NativeMethods.xnvSessionGenerator_IsInSession(toNative(), inSession);
		org.openni.WrapperUtils.throwOnError(status);
		return inSession.value;
	}
	public Point3D GetFocusPoint() throws StatusException
	{
		OutArg<Point3D> focusPoint = new OutArg<Point3D>();
		int status = NativeMethods.xnvSessionGenerator_GetFocusPoint(toNative(), focusPoint);
		org.openni.WrapperUtils.throwOnError(status);
		return focusPoint.value;
	}
	
	// Events
	public IObservable<PointEventArgs> getSessionStartEvent()
	{
		return sessionStartEvent;
	}
	public IObservable<NullEventArgs> getSessionEndEvent()
	{
		return sessionEndEvent;
	}
	public IObservable<StringPointValueEventArgs> getSessionFocusProgressEvent()
	{
		return sessionFocusProgressEvent;
	}
	
	private Observable<PointEventArgs> sessionStartEvent;
	private Observable<NullEventArgs> sessionEndEvent;
	private Observable<StringPointValueEventArgs> sessionFocusProgressEvent;
	
}

