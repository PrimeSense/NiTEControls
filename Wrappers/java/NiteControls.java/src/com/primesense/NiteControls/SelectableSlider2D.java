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
import org.openni.Point3D;
import org.openni.Observable;
import org.openni.OutArg;
import org.openni.StatusException;

public class SelectableSlider2D extends PointControl
{
	public SelectableSlider2D(int itemsX, int itemsY, float sliderSizeX, float sliderSizeY, String name) throws org.openni.GeneralException
	{
		super(NativeMethods.xnvSelectableSlider2D_Create(itemsX, itemsY, sliderSizeX, sliderSizeY, name));

		itemHoverEvent = new Observable<Index2EventArgs>()
		{
			@Override
			protected int registerNative(OutArg<Long> hCallback) 
			{
				return NativeMethods.xnvSelectableSlider2D_RegisterItemHover(toNative(), this, "callback", hCallback);
			}

			@Override
			protected void unregisterNative(long hCallback) 
			{
				NativeMethods.xnvSelectableSlider2D_UnregisterItemHover(toNative(), hCallback);
			}
			
			@SuppressWarnings("unused")
			public void callback(int idX, int idY)
			{
				notify(new Index2EventArgs(idX, idY));
			}
		};

		itemSelectEvent = new Observable<Index2DirectionEventArgs>()
		{
			@Override
			protected int registerNative(OutArg<Long> hCallback) 
			{
				return NativeMethods.xnvSelectableSlider2D_RegisterItemSelect(toNative(), this, "callback", hCallback);
			}

			@Override
			protected void unregisterNative(long hCallback) 
			{
				NativeMethods.xnvSelectableSlider2D_UnregisterItemSelect(toNative(), hCallback);
			}
			
			@SuppressWarnings("unused")
			public void callback(int idX, int idY, int dir)
			{
				notify(new Index2DirectionEventArgs(idX, idY, Direction.fromNative(dir)));
			}
		};
		
		valueChangeEvent = new Observable<Value2EventArgs>()
		{
			@Override
			protected int registerNative(OutArg<Long> hCallback) 
			{
				return NativeMethods.xnvSelectableSlider2D_RegisterValueChange(toNative(), this, "callback", hCallback);
			}

			@Override
			protected void unregisterNative(long hCallback) 
			{
				NativeMethods.xnvSelectableSlider2D_UnregisterValueChange(toNative(), hCallback);
			}
			
			@SuppressWarnings("unused")
			public void callback(float valueX, float valueY)
			{
				notify(new Value2EventArgs(valueX, valueY));
			}
		};
		offAxisEvent = new Observable<DirectionEventArgs>()
		{
			@Override
			protected int registerNative(OutArg<Long> hCallback) 
			{
				return NativeMethods.xnvSelectableSlider2D_RegisterOffAxisMovement(toNative(), this, "callback", hCallback);
			}

			@Override
			protected void unregisterNative(long hCallback) 
			{
				NativeMethods.xnvSelectableSlider2D_UnregisterOffAxisMovement(toNative(), hCallback);
			}
			
			@SuppressWarnings("unused")
			public void callback(int dir)
			{
				notify(new DirectionEventArgs(Direction.fromNative(dir)));
			}
		};

	}
	public SelectableSlider2D(int itemsX, int itemsY, String name) throws org.openni.GeneralException
	{
		this(itemsX, itemsY, 450, 450, name);
	}
	public SelectableSlider2D(int itemsX, int itemsY) throws org.openni.GeneralException
	{
		this(itemsX, itemsY, "SelectableSlider2D");
	}

	protected void freeObject()
	{
		super.freeObject();
		NativeMethods.xnvSelectableSlider2D_Destroy(toNative());
	}

	public void reposition(Point3D center) throws org.openni.StatusException
	{
		int status = NativeMethods.xnvSelectableSlider2D_Reposition(toNative(), center.getX(), center.getY(), center.getZ());
		org.openni.WrapperUtils.throwOnError(status);
	}
	public Point3D getCenter() throws org.openni.StatusException
	{
		OutArg<Point3D> point = new OutArg<Point3D>();
		int status = NativeMethods.xnvSelectableSlider2D_GetCenter(toNative(), point);
		org.openni.WrapperUtils.throwOnError(status);
		return point.value;
	}

	// ItemXCount
	public void setItemXCount(int items) throws StatusException
	{
		int status = NativeMethods.xnvSelectableSlider2D_SetItemXCount(toNative(), items);
		org.openni.WrapperUtils.throwOnError(status);
	}
	public int getItemXCount() throws StatusException
	{
		OutArg<Integer> items = new OutArg<Integer>();
		int status = NativeMethods.xnvSelectableSlider2D_GetItemXCount(toNative(), items);
		org.openni.WrapperUtils.throwOnError(status);
		return items.value;
	}
	// ItemYCount
	public void setItemYCount(int items) throws StatusException
	{
		int status = NativeMethods.xnvSelectableSlider2D_SetItemYCount(toNative(), items);
		org.openni.WrapperUtils.throwOnError(status);
	}
	public int getItemYCount() throws StatusException
	{
		OutArg<Integer> items = new OutArg<Integer>();
		int status = NativeMethods.xnvSelectableSlider2D_GetItemYCount(toNative(), items);
		org.openni.WrapperUtils.throwOnError(status);
		return items.value;
	}
	// SliderXSize
	public float getSliderXSize() throws StatusException
	{
		OutArg<Float> slider = new OutArg<Float>();
		int status = NativeMethods.xnvSelectableSlider2D_GetSliderXSize(toNative(), slider);
		org.openni.WrapperUtils.throwOnError(status);
		return slider.value;
	}
	// SliderYSize
	public float getSliderYSize() throws StatusException
	{
		OutArg<Float> slider = new OutArg<Float>();
		int status = NativeMethods.xnvSelectableSlider2D_GetSliderYSize(toNative(), slider);
		org.openni.WrapperUtils.throwOnError(status);
		return slider.value;
	}
	// SliderSize
	public void setSliderSize(float sliderXSize, float sliderYSize) throws StatusException
	{
		int status = NativeMethods.xnvSelectableSlider2D_SetSliderSize(toNative(), sliderXSize, sliderYSize);
		org.openni.WrapperUtils.throwOnError(status);
	}
	// ValueChangeOnOffAxis
	public void setValueChangeOnOffAxis(boolean report) throws StatusException
	{
		int status = NativeMethods.xnvSelectableSlider2D_SetValueChangeOnOffAxis(toNative(), report);
		org.openni.WrapperUtils.throwOnError(status);
	}
	public boolean getValueChangeOnOffAxis() throws StatusException
	{
		OutArg<Boolean> report = new OutArg<Boolean>();
		int status = NativeMethods.xnvSelectableSlider2D_GetValueChangeOnOffAxis(toNative(), report);
		org.openni.WrapperUtils.throwOnError(status);
		return report.value;
	}
	
	// Events
	public IObservable<Index2EventArgs> getItemHoverEvent()
	{
		return itemHoverEvent;
	}
	public IObservable<Index2DirectionEventArgs> getItemSelectEvent()
	{
		return itemSelectEvent;
	}
	public IObservable<Value2EventArgs> getValueChangeEvent()
	{
		return valueChangeEvent;
	}
	public IObservable<DirectionEventArgs> getOffAxisEvent()
	{
		return offAxisEvent;
	}
	
	private Observable<Index2EventArgs> itemHoverEvent;
	private Observable<Index2DirectionEventArgs> itemSelectEvent;
	private Observable<Value2EventArgs> valueChangeEvent;
	private Observable<DirectionEventArgs> offAxisEvent;
}
