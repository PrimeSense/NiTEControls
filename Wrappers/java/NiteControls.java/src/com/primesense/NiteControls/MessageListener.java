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
import org.openni.GeneralException;
import org.openni.Observable;
import org.openni.IObservable;
import org.openni.OutArg;

public class MessageListener extends ObjectWrapper
{
	public MessageListener(long id) throws org.openni.GeneralException
	{
		super(id);

		activateEvent = new Observable<NullEventArgs>()
		{
			@Override
			protected int registerNative(OutArg<Long> hCallback) 
			{
				return NativeMethods.xnvMessageListener_RegisterActivate(toNative(), this, "callback", hCallback);
			}

			@Override
			protected void unregisterNative(long hCallback) 
			{
				NativeMethods.xnvMessageListener_UnregisterActivate(toNative(), hCallback);
			}
			
			@SuppressWarnings("unused")
			public void callback()
			{
				notify(new NullEventArgs());
			}
		};
		deactivateEvent = new Observable<NullEventArgs>()
		{
			@Override
			protected int registerNative(OutArg<Long> hCallback) 
			{
				return NativeMethods.xnvMessageListener_RegisterDeactivate(toNative(), this, "callback", hCallback);
			}

			@Override
			protected void unregisterNative(long hCallback) 
			{
				NativeMethods.xnvMessageListener_UnregisterDeactivate(toNative(), hCallback);
			}
			
			@SuppressWarnings("unused")
			public void callback()
			{
				notify(new NullEventArgs());
			}
		};
		messageUpdateEvent = new Observable<PointerEventArgs>()
		{
			@Override
			protected int registerNative(OutArg<Long> hCallback) 
			{
				return NativeMethods.xnvMessageListener_RegisterUpdate(toNative(), this, "callback", hCallback);
			}

			@Override
			protected void unregisterNative(long hCallback) 
			{
				NativeMethods.xnvMessageListener_UnregisterUpdate(toNative(), hCallback);
			}
			
			@SuppressWarnings("unused")
			public void callback(long message)
			{
				notify(new PointerEventArgs(message));
			}
		};
	}

	protected void freeObject()
	{
	}

	public void updateMessage(long message) throws org.openni.StatusException
	{
		int status = NativeMethods.xnvMessageListener_Update(toNative(), message);
		org.openni.WrapperUtils.throwOnError(status);
	}

	public IObservable<PointerEventArgs> getMessageUpdateEvent()
	{
		return messageUpdateEvent;
	}
	public IObservable<NullEventArgs> getActivateEvent()
	{
		return activateEvent;
	}
	public IObservable<NullEventArgs> getDeactivateEvent()
	{
		return deactivateEvent;
	}

	
	private Observable<PointerEventArgs> messageUpdateEvent;
	private Observable<NullEventArgs> activateEvent;
	private Observable<NullEventArgs> deactivateEvent;
}
