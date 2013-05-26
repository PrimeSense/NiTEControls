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
import org.openni.Context;
import org.openni.OutArg;
import org.openni.Point3D;
import org.openni.StatusException;

public class SessionManager extends SessionGenerator
{
	public SessionManager(org.openni.Context context, String focus) throws org.openni.GeneralException
	{
		super(NativeMethods.xnvSessionManager_Create("SessionManager"));
		int status = NativeMethods.xnvSessionManager_Initialize(toNative(), context.toNative(), focus);
		org.openni.WrapperUtils.throwOnError(status);
	}

	protected void freeObject()
	{
		super.freeObject();
		NativeMethods.xnvSessionManager_Destroy(toNative());
	}

	public void update(org.openni.Context context) throws org.openni.StatusException
	{
		int status = NativeMethods.xnvSessionManager_Update(toNative(), context.toNative());
		org.openni.WrapperUtils.throwOnError(status);
	}
	

	// Manage gesture & quick refocus
	public int addGestureByContext(Context context, String name) throws StatusException
	{
		OutArg<Integer> gestureID = new OutArg<Integer>();
		int status = NativeMethods.xnvSessionManager_AddGestureByContext(toNative(), context.toNative(), name, gestureID);
		org.openni.WrapperUtils.throwOnError(status);
		return gestureID.value;
	}

	public void removeGesture(int gestureID) throws StatusException
	{
		int status = NativeMethods.xnvSessionManager_RemoveGesture(toNative(), gestureID);
		org.openni.WrapperUtils.throwOnError(status);
	}
}
