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
package com.primesense.NiteControls.Samples.Boxes;

import java.awt.Color;
import java.awt.Component;

import org.openni.GeneralException;
import org.openni.IObservable;
import org.openni.IObserver;
import org.openni.StatusException;

import com.primesense.NiteControls.*;

class LeaveEvent extends java.util.Observable
{
	void raise()
	{
		setChanged();
		notifyObservers();
	}
}


public class MyBox extends PointControl
{
	public MyBox(String name, Component panel) throws GeneralException
	{
		super();
		this._name = name;
		this._panel = panel;
		
		pushDetector = new PushDetector();
		swipeDetector = new SwipeDetector();
		steadyDetector = new SteadyDetector();
		flowRouter = new FlowRouter();
		broadcaster = new Broadcaster();
		
		broadcaster.addListener(pushDetector);
		broadcaster.addListener(flowRouter);
		
		
		pushDetector.getPushEvent().addObserver(new IObserver<VelocityAngleEventArgs>()
		{
			@Override
			public void update(IObservable<VelocityAngleEventArgs> observable,
					VelocityAngleEventArgs args)
			{
				leaveEvent.raise();
			}
		});
		steadyDetector.getSteadyEvent().addObserver(new IObserver<IdValueEventArgs>()
		{
			@Override
			public void update(IObservable<IdValueEventArgs> observable,
					IdValueEventArgs args)
			{
				System.out.println("Box " + _name + ": Steady");
				try {
					if (args.getId() == getPrimaryID())
					{
						try {
							flowRouter.setActive(swipeDetector);
						} catch (StatusException e) {
							// TODO Auto-generated catch block
							e.printStackTrace();
						}
						ChangeColor(Color.WHITE);
					}
				} catch (StatusException e) {
					e.printStackTrace();
				}
			}
		});
		swipeDetector.getGeneralSwipeEvent().addObserver(new IObserver<DirectionVelocityAngleEventArgs>()
		{
			@Override
			public void update(IObservable<DirectionVelocityAngleEventArgs> observable,
					DirectionVelocityAngleEventArgs args)
			{
				System.out.println("Box " + _name + ": Swipe " + args.getDirection());
				try {
					flowRouter.setActive(steadyDetector);
				} catch (StatusException e) {
					e.printStackTrace();
				}
				ChangeColor(Color.RED);
			}
		});
		
		getPrimaryPointCreateEvent().addObserver(new IObserver<HandPointEventArgs>()
		{
			@Override
			public void update(IObservable<HandPointEventArgs> observable,
					HandPointEventArgs args)
			{
				try {
					flowRouter.setActive(steadyDetector);
				} catch (StatusException e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				}
				// TODO Auto-generated method stub
				
			}
		});
		getPrimaryPointDestroyEvent().addObserver(new IObserver<IdEventArgs>()
		{
			@Override
			public void update(IObservable<IdEventArgs> observable, IdEventArgs args)
			{
				ChangeColor(Color.CYAN);
			}
		});
		getMessageUpdateEvent().addObserver(new IObserver<PointerEventArgs>()
		{
			@Override
			public void update(IObservable<PointerEventArgs> observable,
					PointerEventArgs args)
			{
				try {
					broadcaster.updateMessage(args.getPointer());
				} catch (StatusException e) {
					e.printStackTrace();
				}
			}
		});
		leaveEvent = new LeaveEvent();
	}

	public void ChangeColor(Color color)
	{
		_panel.setBackground(color);
	}
	
	private PushDetector pushDetector;
	private SwipeDetector swipeDetector;
	private SteadyDetector steadyDetector;
	private FlowRouter flowRouter;
	private Broadcaster broadcaster;

	private final String _name;
	private Component _panel;
	
	// Events
	public java.util.Observable getLeaveEvent()
	{
		return leaveEvent;
	}
	
	private LeaveEvent leaveEvent;
}
