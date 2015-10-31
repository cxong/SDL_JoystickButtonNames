/*
    Copyright (c) 2015, Cong Xu

    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions are met:

    Redistributions of source code must retain the above copyright notice, this
    list of conditions and the following disclaimer.
    Redistributions in binary form must reproduce the above copyright notice,
    this list of conditions and the following disclaimer in the documentation
    and/or other materials provided with the distribution.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
    AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
    IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
    ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
    LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
    CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
    SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
    INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
    CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
    ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
    POSSIBILITY OF SUCH DAMAGE.
*/
#include "SDL_joystickbuttonnames.h"

#include <stdio.h>


const char *err = NULL;

int SDLJBN_GetButtonNameAndColor(SDL_Joystick *joystick,
                                 SDL_GameControllerButton button,
                                 char *s, Uint8 *r, Uint8 *g, Uint8 *b)
{
	if (joystick == NULL)
	{
		err = "joystick is NULL";
		return -1;
	}
	if (button < SDL_CONTROLLER_BUTTON_A ||
		button >= SDL_CONTROLLER_BUTTON_MAX)
	{
		err = "button is invalid";
		return -1;
	}
	char guidBuf[256];
	SDL_JoystickGetGUIDString(SDL_JoystickGetGUID(joystick), guidBuf, 256);
	printf("GUID: %s Name: \'%s\'\n", guidBuf, SDL_JoystickName(joystick));

	if (s)
	{
		strcpy(s, SDL_GameControllerGetStringForButton(button));
	}
	if (r)
	{
		*r = 128;
	}
	if (g)
	{
		*g = 128;
	}
	if (b)
	{
		*b = 128;
	}
	return 0;
}

const char *SDLJBN_GetError(void)
{
	return err;
}
