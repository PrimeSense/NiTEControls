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
import org.openni.EventArgs;
import org.openni.Point3D;

public class StringPointValueEventArgs extends EventArgs
{
	public StringPointValueEventArgs(String name, Point3D point, float value)
	{
		this.name = name;
		this.point = point;
		this.value = value;
	}
	
	public String getName()
	{
		return name;
	}
	public Point3D getPoint()
	{
		return point;
	}
	public float getValue()
	{
		return value;
	}
	
	private String name;
	private Point3D point;
	private float value;
}
