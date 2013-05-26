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

public class FlowRouter extends MessageListener
{
	public FlowRouter(String name) throws org.openni.GeneralException
	{
		super(NativeMethods.xnvFlowRouter_Create(name));
	}
	public FlowRouter() throws org.openni.GeneralException
	{
		this("FlowRouter");
	}

	protected void freeObject()
	{
		super.freeObject();

		NativeMethods.xnvFlowRouter_Destroy(toNative());
	}

	public void setActive(MessageListener listener) throws org.openni.StatusException
	{
		int status;
		if (listener == null)
		{
			status = NativeMethods.xnvFlowRouter_SetActive(toNative(), 0);
		}
		else
		{
			status =  NativeMethods.xnvFlowRouter_SetActive(toNative(), listener.toNative());
		}
		org.openni.WrapperUtils.throwOnError(status);
	}

	public MessageListener getActive() throws org.openni.StatusException, org.openni.GeneralException
	{
		OutArg<Long> listener = new OutArg<Long>();

		int status = NativeMethods.xnvFlowRouter_GetActive(toNative(), listener);
		org.openni.WrapperUtils.throwOnError(status);

		if (listener.value == 0)
		{
			return null;
		}

		return new MessageListener(listener.value);
	}
}
