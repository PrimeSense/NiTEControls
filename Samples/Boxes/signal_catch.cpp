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

#include "signal_catch.h"
#include <stdio.h>
#include <unistd.h>
#include <signal.h>


XnBool * g_pValueToSet = NULL;

void SignalHandler(int nSignal)
{
	printf("Caught signal: %d\n", nSignal);
	if (NULL != g_pValueToSet) {
		*g_pValueToSet = true;
	}
}

void CatchSignals(XnBool* bSignalWasCaught)
{
	g_pValueToSet = bSignalWasCaught;

	struct sigaction act;

	memset( &act, 0, sizeof( act ) );
	act.sa_handler = &SignalHandler;
	act.sa_flags = 0;
	sigaction( SIGINT, &act, NULL );
	sigaction( SIGTERM, &act, NULL );
	sigaction( SIGKILL, &act, NULL );
}


