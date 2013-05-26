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
#ifndef __SIGNAL_CATCH_H__
#define __SIGNAL_CATCH_H__

#include <XnPlatform.h>

#if XN_PLATFORM == XN_PLATFORM_WIN32

#define CatchSignals(x)

#else
// Stores pointer to bSignalWasCaught, and starts catching signals
// when a signal is caught, *bSignalWasCaught will be set to true
void CatchSignals(XnBool* bSignalWasCaught);
#endif

#endif
