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
//-----------------------------------------------------------------------------
// Headers
//-----------------------------------------------------------------------------
// General headers
#include <stdio.h>
// OpenNI headers
#include <XnOpenNI.h>
// NITE headers
#include <XnVSessionManager.h>
#include <XnVSteadyDetector.h>

// xml to initialize OpenNI
#define SAMPLE_XML_FILE "../../Data/Sample-Tracking.xml"
#define SAMPLE_XML_FILE_LOCAL "Sample-Tracking.xml"

//-----------------------------------------------------------------------------
// Callbacks
//-----------------------------------------------------------------------------

// Callback for when the focus is in progress
void XN_CALLBACK_TYPE SessionProgress(const XnChar* strFocus, const XnPoint3D& ptFocusPoint, XnFloat fProgress, void* UserCxt)
{
	printf("Session progress (%6.2f,%6.2f,%6.2f) - %6.2f [%s]\n", ptFocusPoint.X, ptFocusPoint.Y, ptFocusPoint.Z, fProgress,  strFocus);
}
// callback for session start
void XN_CALLBACK_TYPE SessionStart(const XnPoint3D& ptFocusPoint, void* UserCxt)
{
	printf("Session started. (%6.2f,%6.2f,%6.2f)...\n", ptFocusPoint.X, ptFocusPoint.Y, ptFocusPoint.Z);
}
// Callback for session end
void XN_CALLBACK_TYPE SessionEnd(void* UserCxt)
{
	printf("Session ended. Please perform focus gesture to start session\n");
}
// Callback for steady detection
void XN_CALLBACK_TYPE OnSteadyCB(XnUInt32 nId, XnFloat fStdDev, void* pUserCxt)
{
	printf("Steady!\n");
}
void XN_CALLBACK_TYPE OnUnsteadyCB(XnUInt32 nId, XnFloat fStdDev, void* pUserCxt)
{
	printf("Unsteady\n");
}
// callback for a new position of any hand
void XN_CALLBACK_TYPE OnPointUpdate(const XnVHandPointContext* pContext, void* cxt)
{
	printf("%d: (%f,%f,%f) [%f]\n", pContext->nID, pContext->ptPosition.X, pContext->ptPosition.Y, pContext->ptPosition.Z, pContext->fTime);
}

//-----------------------------------------------------------------------------
// Main
//-----------------------------------------------------------------------------

XnBool fileExists(const char *fn)
{
	XnBool exists;
	xnOSDoesFileExist(fn, &exists);
	return exists;
}


// this sample can run either as a regular sample, or as a client for multi-process (remote mode)
int main(int argc, char** argv)
{
	xn::Context context;
	xn::ScriptNode scriptNode;
	XnVSessionManager* pSessionManager;
	XnBool bRemoting = FALSE;

	// Create context
	const char *fn = NULL;
	if      (fileExists(SAMPLE_XML_FILE)) fn = SAMPLE_XML_FILE;
	else if (fileExists(SAMPLE_XML_FILE_LOCAL)) fn = SAMPLE_XML_FILE_LOCAL;
	else {
		printf("Could not find '%s' nor '%s'. Aborting.\n" , SAMPLE_XML_FILE, SAMPLE_XML_FILE_LOCAL);
		return XN_STATUS_ERROR;
	}
	XnStatus rc = context.InitFromXmlFile(fn, scriptNode);
	if (rc != XN_STATUS_OK)
	{
		printf("Couldn't initialize: %s\n", xnGetStatusString(rc));
		return 1;
	}

	// Create the Session Manager
	pSessionManager = new XnVSessionManager();
	rc = pSessionManager->Initialize(&context, "Click");
	if (rc != XN_STATUS_OK)
	{
		printf("Session Manager couldn't initialize: %s\n", xnGetStatusString(rc));
		delete pSessionManager;
		return 1;
	}

	// Initialization done. Start generating
	context.StartGeneratingAll();

	// Register session callbacks
	pSessionManager->RegisterSession(NULL, &SessionStart, &SessionEnd, &SessionProgress);

	// init & register wave control
	XnVSteadyDetector sd;
	sd.RegisterSteady(NULL, OnSteadyCB);
	sd.RegisterNotSteady(NULL, OnUnsteadyCB);
//	sd.RegisterPointUpdate(NULL, OnPointUpdate);
	pSessionManager->AddListener(&sd);

	printf("Please perform focus gesture to start session\n");
	printf("Hit any key to exit\n");

	// Main loop
	while (!xnOSWasKeyboardHit())
	{
		context.WaitAnyUpdateAll();
		pSessionManager->Update(&context);
	}

	delete pSessionManager;

	return 0;
}
