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

interface IMessageGenerator
{
	void addListener(MessageListener listener) throws org.openni.StatusException;
	void removeListener(MessageListener listener) throws org.openni.StatusException;
	void generate(long message) throws org.openni.StatusException;
}

public class MessageGenerator extends ObjectWrapper implements IMessageGenerator 
{
	public MessageGenerator(long id) throws org.openni.GeneralException
	{
		super(id);
	}
	
	public void addListener(MessageListener listener) throws org.openni.StatusException
	{
		int status = NativeMethods.xnvMessageGenerator_AddListener(toNative(), listener.toNative());
		org.openni.WrapperUtils.throwOnError(status);
	}
	public void removeListener(MessageListener listener) throws org.openni.StatusException
	{
		int status = NativeMethods.xnvMessageGenerator_RemoveListener(toNative(), listener.toNative());
		org.openni.WrapperUtils.throwOnError(status);
	}

	public void generate(long message) throws org.openni.StatusException
	{
		int status = NativeMethods.xnvMessageGenerator_Generate(toNative(), message);
		org.openni.WrapperUtils.throwOnError(status);
	}

	protected void freeObject()
	{}
}
