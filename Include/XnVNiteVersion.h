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
#ifndef _XNV_NITE_VERSION_H_
#define _XNV_NITE_VERSION_H_

#define XNV_NITE_CONTROLS_MAJOR_VERSION 1
#define XNV_NITE_CONTROLS_MINOR_VERSION 7
#define XNV_NITE_CONTROLS_MAINTENANCE_VERSION 0
#define XNV_NITE_CONTROLS_BUILD_VERSION 1

/** Nite Framework version (in brief string format): "Major.Minor.Maintenance (Build)" */ 
#define XNV_NITE_CONTROLS_BRIEF_VERSION_STRING \
	XN_STRINGIFY(XNV_NITE_CONTROLS_MAJOR_VERSION) "." \
	XN_STRINGIFY(XNV_NITE_CONTROLS_MINOR_VERSION) "." \
	XN_STRINGIFY(XNV_NITE_CONTROLS_MAINTENANCE_VERSION) \
	" (Build " XN_STRINGIFY(XNV_NITE_CONTROLS_BUILD_VERSION) ")"

/** Nite Framework version (in numeric format): (major version * 100000000 + minor version * 1000000 + maintenance version * 10000 + build version). */
#define XNV_NITE_CONTROLS_VERSION (XNV_NITE_CONTROLS_MAJOR_VERSION*100000000 + XNV_NITE_CONTROLS_MINOR_VERSION*1000000 + XNV_NITE_CONTROLS_MAINTENANCE_VERSION*10000 + XNV_NITE_CONTROLS_BUILD_VERSION)

/** Nite Framework version (in string format): "Major.Minor.Maintenance.Build-Platform (MMM DD YYYY HH:MM:SS)". */ 
#define XNV_NITE_CONTROLS_VERSION_STRING \
	XNV_NITE_CONTROLS_BRIEF_VERSION_STRING  "-" \
	XN_PLATFORM_STRING " (" XN_TIMESTAMP ")"

#endif // _XNV_NITE_FW_VERSION_H_
