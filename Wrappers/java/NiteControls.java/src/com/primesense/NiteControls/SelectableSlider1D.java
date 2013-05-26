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
import org.openni.Point3D;
import org.openni.StatusException;

public class SelectableSlider1D extends PointControl
{
	public SelectableSlider1D(int items, Axis axis, boolean recenter, float sliderSize, float firstValue, String name) throws org.openni.GeneralException
	{
		super(NativeMethods.xnvSelectableSlider1D_Create(items, axis.getValue(), recenter, sliderSize, firstValue, name));
		
		itemHoverEvent = new Observable<IndexEventArgs>()
		{
			@Override
			protected int registerNative(OutArg<Long> hCallback) 
			{
				return NativeMethods.xnvSelectableSlider1D_RegisterItemHover(toNative(), this, "callback", hCallback);
			}

			@Override
			protected void unregisterNative(long hCallback) 
			{
				NativeMethods.xnvSelectableSlider1D_UnregisterItemHover(toNative(), hCallback);
			}
			
			@SuppressWarnings("unused")
			public void callback(int id)
			{
				notify(new IndexEventArgs(id));
			}
		};

		itemSelectEvent = new Observable<IndexDirectionEventArgs>()
		{
			@Override
			protected int registerNative(OutArg<Long> hCallback) 
			{
				return NativeMethods.xnvSelectableSlider1D_RegisterItemSelect(toNative(), this, "callback", hCallback);
			}

			@Override
			protected void unregisterNative(long hCallback) 
			{
				NativeMethods.xnvSelectableSlider1D_UnregisterItemSelect(toNative(), hCallback);
			}
			
			@SuppressWarnings("unused")
			public void callback(int id, int dir)
			{
				notify(new IndexDirectionEventArgs(id, Direction.fromNative(dir)));
			}
		};
		
		valueChangeEvent = new Observable<ValueEventArgs>()
		{
			@Override
			protected int registerNative(OutArg<Long> hCallback) 
			{
				return NativeMethods.xnvSelectableSlider1D_RegisterValueChange(toNative(), this, "callback", hCallback);
			}

			@Override
			protected void unregisterNative(long hCallback) 
			{
				NativeMethods.xnvSelectableSlider1D_UnregisterValueChange(toNative(), hCallback);
			}
			
			@SuppressWarnings("unused")
			public void callback(float value)
			{
				notify(new ValueEventArgs(value));
			}
		};

		offAxisEvent = new Observable<DirectionEventArgs>()
		{
			@Override
			protected int registerNative(OutArg<Long> hCallback) 
			{
				return NativeMethods.xnvSelectableSlider1D_RegisterOffAxisMovement(toNative(), this, "callback", hCallback);
			}

			@Override
			protected void unregisterNative(long hCallback) 
			{
				NativeMethods.xnvSelectableSlider1D_UnregisterOffAxisMovement(toNative(), hCallback);
			}
			
			@SuppressWarnings("unused")
			public void callback(int dir)
			{
				notify(new DirectionEventArgs(Direction.fromNative(dir)));
			}
		};
	}
	public SelectableSlider1D(int items, Axis axis) throws org.openni.GeneralException
	{
		this(items, axis, true, 200.0f, 0.5f, "SelectableSlider1D");
	}

	protected void freeObject()
	{
		super.freeObject();
		NativeMethods.xnvSelectableSlider1D_Destroy(toNative());
	}

	// Center
	public void reposition(Point3D center) throws org.openni.StatusException
	{
		int status = NativeMethods.xnvSelectableSlider1D_Reposition(toNative(), center.getX(), center.getY(), center.getZ());
		org.openni.WrapperUtils.throwOnError(status);
	}
	public Point3D getCenter() throws org.openni.StatusException
	{
		OutArg<Point3D> point = new OutArg<Point3D>();
		int status = NativeMethods.xnvSelectableSlider1D_GetCenter(toNative(), point);
		org.openni.WrapperUtils.throwOnError(status);
		return point.value;
	}
	
	// ItemCount
	public void setItemCount(int items) throws StatusException
	{
		int status = NativeMethods.xnvSelectableSlider1D_SetItemCount(toNative(), items);
		org.openni.WrapperUtils.throwOnError(status);
	}
	public int getItemCount() throws StatusException
	{
		OutArg<Integer> items = new OutArg<Integer>();
		int status = NativeMethods.xnvSelectableSlider1D_GetItemCount(toNative(), items);
		org.openni.WrapperUtils.throwOnError(status);
		return items.value;
	}
	// SliderSize
	public void setSliderSize(float slider) throws StatusException
	{
		int status = NativeMethods.xnvSelectableSlider1D_SetSliderSize(toNative(), slider);
		org.openni.WrapperUtils.throwOnError(status);
	}
	public float getSliderSize() throws StatusException
	{
		OutArg<Float> slider = new OutArg<Float>();
		int status = NativeMethods.xnvSelectableSlider1D_GetSliderSize(toNative(), slider);
		org.openni.WrapperUtils.throwOnError(status);
		return slider.value;
	}
	// ValueChangeOnOffAxis
	public void setValueChangeOnOffAxis(boolean report) throws StatusException
	{
		int status = NativeMethods.xnvSelectableSlider1D_SetValueChangeOnOffAxis(toNative(), report);
		org.openni.WrapperUtils.throwOnError(status);
	}
	public boolean getValueChangeOnOffAxis() throws StatusException
	{
		OutArg<Boolean> report = new OutArg<Boolean>();
		int status = NativeMethods.xnvSelectableSlider1D_GetValueChangeOnOffAxis(toNative(), report);
		org.openni.WrapperUtils.throwOnError(status);
		return report.value;
	}
	
	// Events
	public IObservable<IndexEventArgs> getItemHoverEvent()
	{
		return itemHoverEvent;
	}
	public IObservable<IndexDirectionEventArgs> getItemSelectEvent()
	{
		return itemSelectEvent;
	}
	public IObservable<ValueEventArgs> getValueChangeEvent()
	{
		return valueChangeEvent;
	}
	public IObservable<DirectionEventArgs> getOffAxisEvent()
	{
		return offAxisEvent;
	}
	
	private Observable<IndexEventArgs> itemHoverEvent;
	private Observable<IndexDirectionEventArgs> itemSelectEvent;
	private Observable<ValueEventArgs> valueChangeEvent;
	private Observable<DirectionEventArgs> offAxisEvent;
	
}
