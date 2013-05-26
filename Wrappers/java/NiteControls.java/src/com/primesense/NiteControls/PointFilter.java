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
import org.openni.StatusException;

public class PointFilter extends PointControl implements IMessageGenerator
{
	public PointFilter() throws org.openni.GeneralException
	{
		this("PointFilter");
	}
	public PointFilter(String name) throws org.openni.GeneralException
	{
		super(NativeMethods.xnvPointFilter_Create(name));
		init(true);
	}
	public PointFilter(long id) throws org.openni.GeneralException
	{
		super(id);
		init(false);
	}
	private void init(boolean local) throws org.openni.GeneralException
	{
		this.localFilter = local;
		this.generator = new MessageGenerator(toNative());
	}

	protected void freeObject()
	{
		super.freeObject();
		this.generator.dispose();

		if (this.localFilter)
		{
			NativeMethods.xnvPointFilter_Destroy(toNative());
		}
		this.localFilter = false;
	}

	public void overridePrimary(int newPrimary) throws StatusException
	{
		int status = NativeMethods.xnvPointFilter_OverridePrimary(toNative(), newPrimary);
		org.openni.WrapperUtils.throwOnError(status);
	}
	
	public void addListener(MessageListener listener) throws org.openni.StatusException
	{
		this.generator.addListener(listener);
	}
	public void removeListener(MessageListener listener) throws org.openni.StatusException
	{
		this.generator.removeListener(listener);
	}
	public void generate(long message) throws org.openni.StatusException
	{
		this.generator.generate(message);
	}

	private MessageGenerator generator;
	private boolean localFilter;
}
