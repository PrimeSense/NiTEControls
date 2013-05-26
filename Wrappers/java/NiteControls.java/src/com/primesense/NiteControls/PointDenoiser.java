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
import org.openni.StatusException;

public class PointDenoiser extends PointFilter
{
	public PointDenoiser(float distanceThreshold, String name) throws org.openni.GeneralException
	{
		super(NativeMethods.xnvPointDenoiser_Create(distanceThreshold, name));
	}
	public PointDenoiser(String name) throws org.openni.GeneralException
	{
		this(10, name);
	}
	public PointDenoiser(float distanceThreshold) throws org.openni.GeneralException
	{
		this(distanceThreshold, "PointDenoiser");
	}
	public PointDenoiser() throws org.openni.GeneralException
	{
		this(10, "PointDenoiser");
	}

	protected void freeObject()
	{
		super.freeObject();

		NativeMethods.xnvPointDenoiser_Destroy(toNative());
	}
	
	public void setDistanceThreshold(float threshold) throws StatusException
	{
		int status = NativeMethods.xnvPointDenoiser_SetDistanceThreshold(toNative(), threshold);
		org.openni.WrapperUtils.throwOnError(status);
	}
	public void setCloseRatio(float ratio) throws StatusException
	{
		int status = NativeMethods.xnvPointDenoiser_SetCloseRatio(toNative(), ratio);
		org.openni.WrapperUtils.throwOnError(status);
	}
	public void setFarRatio(float ratio) throws StatusException
	{
		int status = NativeMethods.xnvPointDenoiser_SetFarRatio(toNative(), ratio);
		org.openni.WrapperUtils.throwOnError(status);
	}
	
	public float getDistanceThreshold() throws StatusException
	{
		OutArg<Float> threshold = new OutArg<Float>();
		int status = NativeMethods.xnvPointDenoiser_GetDistanceThreshold(toNative(), threshold);
		org.openni.WrapperUtils.throwOnError(status);
		return threshold.value;
	}
	public float getCloseRatio() throws StatusException
	{
		OutArg<Float> ratio = new OutArg<Float>();
		int status = NativeMethods.xnvPointDenoiser_GetCloseRatio(toNative(), ratio);
		org.openni.WrapperUtils.throwOnError(status);
		return ratio.value;
	}
	public float getFarRatio() throws StatusException
	{
		OutArg<Float> ratio = new OutArg<Float>();
		int status = NativeMethods.xnvPointDenoiser_GetFarRatio(toNative(), ratio);
		org.openni.WrapperUtils.throwOnError(status);
		return ratio.value;
	}
}
