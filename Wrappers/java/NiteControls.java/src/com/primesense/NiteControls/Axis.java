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

import java.util.NoSuchElementException;

public enum Axis
{
	X(0),
	Y(1),
	Z(2),
	ILLEGAL(3);

	Axis(int value) {this.val = value;}
	public int getValue() {return this.val;}

	static Axis fromNative(int value)
	{
		for (Axis type : Axis.values()) 
		{
			if (type.val == value)
				return type;
		}
		
		throw new NoSuchElementException();
	}

	private final int val;
}
