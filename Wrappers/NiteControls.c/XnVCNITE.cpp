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
#if 0
#include "XnVCNITE.h"
#include <XnHash.h>
#include <XnVNite.h>

#include "ObjectAllocation.h"
#include "MessageGenerator.h"
#include "MessageListener.h"
#include "PointControl.h"

// MessageGenerator hash
// XN_DECLARE_DEFAULT_HASH(XnVObjectID, XnVMessageGenerator*, MessageGeneratorsHash);
// MessageListener hash
// XN_DECLARE_DEFAULT_HASH(XnVObjectID, XnVMessageListener*, MessageListenersHash);
// SessionManager hash - also MessageGenerator
// XN_DECLARE_DEFAULT_HASH(XnVObjectID, XnVSessionManager*, SessionManagersHash);
// Broadcaster hash - also MessageGenerator and MessageListener
// XN_DECLARE_DEFAULT_HASH(XnVObjectID, XnVBroadcaster*, BroadcastersHash);
// FlowRouter hash - also MessageListener
XN_DECLARE_DEFAULT_HASH(XnVObjectID, XnVFlowRouter*, FlowRoutersHash);
// PointArea hash - also PointControl and MessageGenerator
XN_DECLARE_DEFAULT_HASH(XnVObjectID, XnVPointArea*, PointAreasHash);
// PointDenoiser hash - also PointControl and MessageGenerator
XN_DECLARE_DEFAULT_HASH(XnVObjectID, XnVPointDenoiser*, PointDenoisersHash);
// CircleDetector hash - also PointControl
XN_DECLARE_DEFAULT_HASH(XnVObjectID, XnVCircleDetector*, CircleDetectorsHash);
// PushDetector hash - also PointControl
XN_DECLARE_DEFAULT_HASH(XnVObjectID, XnVPushDetector*, PushDetectorsHash);
// SelectableSlider1D hash - also PointControl
XN_DECLARE_DEFAULT_HASH(XnVObjectID, XnVSelectableSlider1D*, SelectableSlider1DsHash);
// SelectableSlider2D hash - also PointControl
XN_DECLARE_DEFAULT_HASH(XnVObjectID, XnVSelectableSlider2D*, SelectableSlider2DsHash);
// SteadyDetector hash - also PointControl
XN_DECLARE_DEFAULT_HASH(XnVObjectID, XnVSteadyDetector*, SteadyDetectorsHash);
// SwipeDetector hash - also PointControl
XN_DECLARE_DEFAULT_HASH(XnVObjectID, XnVSwipeDetector*, SwipeDetectorsHash);
// WaveDetector hash - also PointControl
XN_DECLARE_DEFAULT_HASH(XnVObjectID, XnVWaveDetector*, WaveDetectorsHash);
// PointControl hash - also MessageListener
// XN_DECLARE_DEFAULT_HASH(XnVObjectID, XnVPointControl*, PointControlsHash);
// PointFilter hash - also PointControl and MessageGenereator
// XN_DECLARE_DEFAULT_HASH(XnVObjectID, XnVPointFilter*, PointFiltersHash);

//MessageGeneratorsHash g_MessageGenerators;
// MessageListenersHash g_MessageListeners;
//SessionManagersHash g_SessionManagers;
// BroadcastersHash g_Broadcasters;
FlowRoutersHash g_FlowRotuers;
PointAreasHash g_PointAreas;
PointDenoisersHash g_PointDenoisers;
CircleDetectorsHash g_CircleDetectors;
PushDetectorsHash g_PushDetectors;
SelectableSlider1DsHash g_SelectableSlider1Ds;
SelectableSlider2DsHash g_SelectableSlider2Ds;
SteadyDetectorsHash g_SteadyDetectors;
SwipeDetectorsHash g_SwipeDetectors;
WaveDetectorsHash g_WaveDetectors;
// PointControlsHash g_PointControls;
// PointFiltersHash g_PointFilters;

#if 0
XnVObjectID g_nNextAvailable = 1;

XnVObjectID AllocateID()
{
	XnVObjectID id = g_nNextAvailable;
	g_nNextAvailable++;
	return id;
}
#endif

// void xnvMessageGenerator_AddToHash(XnVObjectID id, XnVMessageGenerator* pGenerator)
// {
// 	g_MessageGenerators.Set(id, pGenerator);
// }
// XnVMessageGenerator* xnvMessageGenerator_RemoveFromHash(XnVObjectID id)
// {
// 	XnVMessageGenerator* pGenerator = NULL;
// 	XnStatus rc = g_MessageGenerators.Get(id, pGenerator);
// 	if (rc != XN_STATUS_OK)
// 	{
// 		return NULL;
// 	}
// 	g_MessageGenerators.Remove(id);
// 	return pGenerator;
// }
// void xnvMessageListener_AddToHash(XnVObjectID id, XnVMessageListener* pListener)
// {
// 	g_MessageListeners.Set(id, pListener);
// }
// XnVMessageListener* xnvMessageListener_RemoveFromHash(XnVObjectID id)
// {
// 	XnVMessageListener* pListener = NULL;
// 	XnStatus rc = g_MessageListeners.Get(id, pListener);
// 	if (rc != XN_STATUS_OK)
// 	{
// 		return NULL;
// 	}
// 	g_MessageListeners.Remove(id);
// 	return pListener;
// }
// void xnvPointControl_AddToHash(XnVObjectID id, XnVPointControl* pControl)
// {
// 	g_PointControls.Set(id, pControl);
// 	xnvMessageListener_AddToHash(id, pControl);
// }
// XnVPointControl* xnvPointControl_RemoveFromHash(XnVObjectID id)
// {
// 	xnvMessageListener_RemoveFromHash(id);
// 
// 	XnVPointControl* pControl = NULL;
// 	XnStatus rc = g_PointControls.Get(id, pControl);
// 	if (rc != XN_STATUS_OK)
// 	{
// 		return NULL;
// 	}
// 	g_PointControls.Remove(id);
// 	return pControl;
// }
// void xnvPointFilter_AddToHash(XnVObjectID id, XnVPointFilter* pFilter)
// {
// 	g_PointFilters.Set(id, pFilter);
// 	xnvPointControl_AddToHash(id, pFilter);
// 	xnvMessageGenerator_AddToHash(id, pFilter);
// }
// XnVPointFilter* xnvPointFilter_RemoveFromHash(XnVObjectID id)
// {
// 	xnvMessageGenerator_RemoveFromHash(id);
// 	xnvPointControl_RemoveFromHash(id);
// 
// 	XnVPointFilter* pFilter = NULL;
// 	XnStatus rc = g_PointFilters.Get(id, pFilter);
// 	if (rc != XN_STATUS_OK)
// 	{
// 		return NULL;
// 	}
// 	g_PointFilters.Remove(id);
// 	return pFilter;
// }

//////////////
#endif