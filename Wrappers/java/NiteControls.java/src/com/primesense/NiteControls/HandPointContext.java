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
import org.openni.Point3D;

public class HandPointContext
{
	public HandPointContext(int id, Point3D pt, float time, float confidence)
	{
		this.ID = id;
		this.position = pt;
		this.time = time;
		this.confidence = confidence;
	}
	
	public int getID()
	{
		return this.ID;
	}
	public Point3D getPosition()
	{
		return this.position;
	}
	public float getTime()
	{
		return this.time;
	}
	public float getConfidence()
	{
		return this.confidence;
	}
		
	private final int ID;
	private final Point3D position;
	private final float time;
	private final float confidence;
}
