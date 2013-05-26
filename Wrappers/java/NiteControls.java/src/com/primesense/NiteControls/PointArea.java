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
import org.openni.BoundingBox3D;
import org.openni.GeneralException;
import org.openni.IObservable;
import org.openni.Observable;
import org.openni.OutArg;
import org.openni.Point3D;
import org.openni.StatusException;

public class PointArea extends PointFilter
{
	public PointArea(BoundingBox3D area, boolean relative) throws GeneralException
	{
		this(area, relative, "");
	}
	public PointArea(BoundingBox3D area, boolean relative, String name) throws GeneralException
	{
		this(area.getMins(), area.getMaxs(), relative, name);
	}
	public PointArea(Point3D mins, Point3D maxs, boolean relative) throws GeneralException
	{
		this(mins, maxs, relative, "");
	}
	public PointArea(Point3D mins, Point3D maxs, boolean relative, String name) throws GeneralException
	{
		super(NativeMethods.xnvPointArea_Create(mins.getX(), mins.getY(), mins.getZ(), maxs.getX(), maxs.getY(), maxs.getZ(), relative, name));
		
		pointSilencedEvent = new Observable<IdEventArgs>()
		{
			@Override
			protected int registerNative(OutArg<Long> hCallback) 
			{
				return NativeMethods.xnvPointArea_RegisterPointSilenced(toNative(), this, "callback", hCallback);
			}

			@Override
			protected void unregisterNative(long hCallback) 
			{
				NativeMethods.xnvPointArea_UnregisterPointSilenced(toNative(), hCallback);
			}
			
			@SuppressWarnings("unused")
			public void callback(int id)
			{
				notify(new IdEventArgs(id));
			}
		};
		pointRevivedEvent = new Observable<IdEventArgs>()
		{
			@Override
			protected int registerNative(OutArg<Long> hCallback) 
			{
				return NativeMethods.xnvPointArea_RegisterPointRevived(toNative(), this, "callback", hCallback);
			}

			@Override
			protected void unregisterNative(long hCallback) 
			{
				NativeMethods.xnvPointArea_UnregisterPointRevived(toNative(), hCallback);
			}
			
			@SuppressWarnings("unused")
			public void callback(int id)
			{
				notify(new IdEventArgs(id));
			}
		};
		pointSilencedEvent = new Observable<IdEventArgs>()
		{
			@Override
			protected int registerNative(OutArg<Long> hCallback) 
			{
				return NativeMethods.xnvPointArea_RegisterSilentPointRemoved(toNative(), this, "callback", hCallback);
			}

			@Override
			protected void unregisterNative(long hCallback) 
			{
				NativeMethods.xnvPointArea_UnregisterSilentPointRemoved(toNative(), hCallback);
			}
			
			@SuppressWarnings("unused")
			public void callback(int id)
			{
				notify(new IdEventArgs(id));
			}
		};

	}
	
	protected void freeObject()
	{
		super.freeObject();

		NativeMethods.xnvPointArea_Destroy(toNative());
	}

	public void changeArea(BoundingBox3D area, boolean relative) throws StatusException
	{
		changeArea(area.getMins(), area.getMaxs(), relative);
	}
	public void changeArea(Point3D mins, Point3D maxs, boolean relative) throws StatusException
	{
		int status = NativeMethods.xnvPointArea_ChangeArea(toNative(), mins.getX(), mins.getY(), mins.getZ(), maxs.getX(), maxs.getY(), maxs.getZ(), relative);
		org.openni.WrapperUtils.throwOnError(status);
	}
	
	public void RemoveSilentPoints(long delay, SessionManager sessionManager) throws StatusException
	{
		int status = NativeMethods.xnvPointArea_RemoveSilent(toNative(), delay, sessionManager.toNative());
		org.openni.WrapperUtils.throwOnError(status);
	}
	
	// Events
	public IObservable<IdEventArgs> getPointSilencedEvent()
	{
		return pointSilencedEvent;
	}
	public IObservable<IdEventArgs> getPointRevivedEvent()
	{
		return pointRevivedEvent;
	}
	public IObservable<IdEventArgs> getSilentPointRemovedEvent()
	{
		return silentPointRemovedEvent;
	}
	
	private Observable<IdEventArgs> pointSilencedEvent;
	private Observable<IdEventArgs> pointRevivedEvent;
	private Observable<IdEventArgs> silentPointRemovedEvent;
}
