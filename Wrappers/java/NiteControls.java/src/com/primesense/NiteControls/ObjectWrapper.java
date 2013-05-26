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

abstract class ObjectWrapper
{
	ObjectWrapper(long id) throws org.openni.GeneralException
	{
		if (id == 0)
		{
			throw new org.openni.GeneralException("JavaWrapper: Trying to wrap a null object?");
		}
		this.id = id;
	}

	public long toNative()
	{
		return this.id;
	}

	protected void finalize()
	{
		dispose();
	}

	public void dispose()
	{
		if (this.id != 0)
		{
			freeObject();
			this.id = 0;
		}
	}
	protected abstract void freeObject();

	private long id;
}
