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
import org.openni.Observable;
import org.openni.IObservable;
import org.openni.OutArg;
import org.openni.Point3D;
import org.openni.StatusException;

public class PointControl extends MessageListener
{
	public PointControl(long id) throws org.openni.GeneralException 
	{
		super(id);
		this.localCreation = false;
		init();
	}
	public PointControl() throws org.openni.GeneralException
	{
		this("PointControl");
	}
	public PointControl(String name) throws org.openni.GeneralException
	{
		super(NativeMethods.xnvPointControl_Create(name));
		this.localCreation = true;
		init();
	}

	private void init()
	{
		pointCreateEvent = new Observable<HandEventArgs>()
		{
			@Override
			protected int registerNative(OutArg<Long> hCallback) 
			{
				return NativeMethods.xnvPointControl_RegisterPointCreate(toNative(), this, "callback", hCallback);
			}

			@Override
			protected void unregisterNative(long hCallback) 
			{
				NativeMethods.xnvPointControl_UnregisterPointCreate(toNative(), hCallback);
			}
			
			@SuppressWarnings("unused")
			public void callback(HandPointContext hand)
			{
				notify(new HandEventArgs(hand));
			}
		};
		pointUpdateEvent = new Observable<HandEventArgs>() {
			@Override
			protected int registerNative(OutArg<Long> hCallback) 
			{
				return NativeMethods.xnvPointControl_RegisterPointUpdate(toNative(), this, "callback", hCallback);
			}

			@Override
			protected void unregisterNative(long hCallback) 
			{
				NativeMethods.xnvPointControl_UnregisterPointUpdate(toNative(), hCallback);
			}
			
			@SuppressWarnings("unused")
			public void callback(HandPointContext hand)
			{
				notify(new HandEventArgs(hand));
			}
			
		};
		pointDestroyEvent = new Observable<IdEventArgs>()
		{
			@Override
			protected int registerNative(OutArg<Long> hCallback) 
			{
				return NativeMethods.xnvPointControl_RegisterPointDestroy(toNative(), this, "callback", hCallback);
			}

			@Override
			protected void unregisterNative(long hCallback) 
			{
				NativeMethods.xnvPointControl_UnregisterPointDestroy(toNative(), hCallback);
			}
			
			@SuppressWarnings("unused")
			public void callback(int id)
			{
				notify(new IdEventArgs(id));
			}
		};
		primaryPointCreateEvent = new Observable<HandPointEventArgs>()
		{
			@Override
			protected int registerNative(OutArg<Long> hCallback) 
			{
				return NativeMethods.xnvPointControl_RegisterPrimaryPointCreate(toNative(), this, "callback", hCallback);
			}

			@Override
			protected void unregisterNative(long hCallback) 
			{
				NativeMethods.xnvPointControl_UnregisterPrimaryPointCreate(toNative(), hCallback);
			}
			
			@SuppressWarnings("unused")
			public void callback(HandPointContext hand, Point3D point)
			{
				notify(new HandPointEventArgs(hand, point));
			}
		};
		primaryPointUpdateEvent = new Observable<HandEventArgs>()
		{
			@Override
			protected int registerNative(OutArg<Long> hCallback) 
			{
				return NativeMethods.xnvPointControl_RegisterPrimaryPointUpdate(toNative(), this, "callback", hCallback);
			}

			@Override
			protected void unregisterNative(long hCallback) 
			{
				NativeMethods.xnvPointControl_UnregisterPrimaryPointUpdate(toNative(), hCallback);
			}
			
			@SuppressWarnings("unused")
			public void callback(HandPointContext hand)
			{
				notify(new HandEventArgs(hand));
			}
		};
		primaryPointReplaceEvent = new Observable<HandIdEventArgs>()
		{
			@Override
			protected int registerNative(OutArg<Long> hCallback) 
			{
				return NativeMethods.xnvPointControl_RegisterPrimaryPointReplace(toNative(), this, "callback", hCallback);
			}

			@Override
			protected void unregisterNative(long hCallback) 
			{
				NativeMethods.xnvPointControl_UnregisterPrimaryPointReplace(toNative(), hCallback);
			}
			
			@SuppressWarnings("unused")
			public void callback(HandPointContext hand, int id)
			{
				notify(new HandIdEventArgs(hand, id));
			}
		};
		primaryPointDestroyEvent = new Observable<IdEventArgs>()
		{
			@Override
			protected int registerNative(OutArg<Long> hCallback) 
			{
				return NativeMethods.xnvPointControl_RegisterPrimaryPointDestroy(toNative(), this, "callback", hCallback);
			}

			@Override
			protected void unregisterNative(long hCallback) 
			{
				NativeMethods.xnvPointControl_UnregisterPrimaryPointDestroy(toNative(), hCallback);
			}
			
			@SuppressWarnings("unused")
			public void callback(int id)
			{
				notify(new IdEventArgs(id));
			}
		};

		noPointsEvent = new Observable<NullEventArgs>()
		{
			@Override
			protected int registerNative(OutArg<Long> hCallback) 
			{
				return NativeMethods.xnvPointControl_RegisterNoPoints(toNative(), this, "callback", hCallback);
			}

			@Override
			protected void unregisterNative(long hCallback) 
			{
				NativeMethods.xnvPointControl_UnregisterNoPoints(toNative(), hCallback);
			}
			
			@SuppressWarnings("unused")
			public void callback()
			{
				notify(new NullEventArgs());
			}
		};
	}

	protected void freeObject()
	{
		super.freeObject();

		if (this.localCreation)
		{
			NativeMethods.xnvPointControl_Destroy(toNative());
		}
	}
	
	public int getPrimaryID() throws StatusException
	{
		OutArg<Integer> primary = new OutArg<Integer>();
		int status = NativeMethods.xnvPointControl_GetPrimaryID(toNative(), primary);
		org.openni.WrapperUtils.throwOnError(status);
		return primary.value;
	}
	
	// Events
	public IObservable<HandEventArgs> getPointCreateEvent()
	{
		return pointCreateEvent;
	}

	public IObservable<HandEventArgs> getPointUpdateEvent()
	{
		return pointUpdateEvent;
	}
	public IObservable<IdEventArgs> getPointDestroyEvent()
	{
		return pointDestroyEvent;
	}

	public IObservable<HandPointEventArgs> getPrimaryPointCreateEvent()
	{
		return primaryPointCreateEvent;
	}
	public IObservable<HandEventArgs> getPrimaryPointUpdateEvent()
	{
		return primaryPointUpdateEvent;
	}
	public IObservable<HandIdEventArgs> getPrimaryPointReplaceEvent()
	{
		return primaryPointReplaceEvent;
	}
	public IObservable<IdEventArgs> getPrimaryPointDestroyEvent()
	{
		return primaryPointDestroyEvent;
	}
	public IObservable<NullEventArgs> getNoPointsEvent()
	{
		return noPointsEvent;
	}

	private Observable<HandEventArgs> pointCreateEvent;
	private Observable<HandEventArgs> pointUpdateEvent;
	private Observable<IdEventArgs> pointDestroyEvent;

	private Observable<HandPointEventArgs> primaryPointCreateEvent;
	private Observable<HandEventArgs> primaryPointUpdateEvent;
	private Observable<HandIdEventArgs> primaryPointReplaceEvent;
	private Observable<IdEventArgs> primaryPointDestroyEvent;
	
	private Observable<NullEventArgs> noPointsEvent;

	private boolean localCreation;
}
