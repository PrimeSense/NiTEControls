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

#include "kbhit.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <termios.h>
#include <fcntl.h>

int _kbhit()
{
	int ch;
	int oldf;
	struct termios old_termios;
	struct termios new_termios;

	tcgetattr( STDIN_FILENO, &old_termios );
	new_termios = old_termios;
	new_termios.c_lflag &= ~( ICANON | ECHO );
	tcsetattr( STDIN_FILENO, TCSANOW, &new_termios );
	oldf = fcntl( STDIN_FILENO, F_GETFL, 0 );
	fcntl( STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK );

	ch = getchar();

	tcsetattr( STDIN_FILENO, TCSANOW, &old_termios );
	fcntl( STDIN_FILENO, F_SETFL, oldf );

	if( ch != EOF ) {
		ungetc( ch, stdin );
		return 1;
	}

	return 0;
}

