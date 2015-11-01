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

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <SDL_pixels.h>


const char *err = NULL;
#include "db.h"

static const char *DefaultButtonName(SDL_GameControllerButton button);
static SDL_Color DefaultButtonColor(SDL_GameControllerButton button);
static bool TryReadColor(
	const char **cur, const char **nextColon, const char *nextComma,
	Uint8 *component);

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
	const char *joystickName = SDL_JoystickName(joystick);
	const char *buttonName = SDL_GameControllerGetStringForButton(button);
	bool found = false;
	// Search for a matching GUID + joystickName in the db
	for (const char *cur = db; cur && *cur; cur = strchr(cur, '\n'))
	{
#define STR_NOT_EQ(expected, actualP, actualEnd)\
	strlen(expected) != (actualEnd) - (actualP) || \
	strncmp(expected, actualP, (actualEnd) - (actualP)) != 0
		if (*cur == '\n') cur++;
		if (*cur == NULL) break;
		// Ignore hash comments
		if (*cur == '#') continue;
		// Compare GUID
		const char *nextComma = strchr(cur, ',');
		if (nextComma == NULL) continue;
		if (STR_NOT_EQ(guidBuf, cur, nextComma)) continue;
		// Same GUID; compare joystick name
		cur = nextComma + 1;
		nextComma = strchr(cur, ',');
		if (nextComma == NULL) continue;
		if (STR_NOT_EQ(joystickName, cur, nextComma)) continue;
		// GUID and joystick name match; read name and colors
		while (!found)
		{
			cur = nextComma + 1;
			nextComma = strchr(cur, ',');
			if (nextComma == NULL) break;
			// Compare button name
			const char *nextColon = strchr(cur, ':');
			if (nextColon == NULL) continue;
			if (STR_NOT_EQ(buttonName, cur, nextColon)) continue;
			// Found the button name; read the real button name
			cur = nextColon + 1;
			nextColon = strchr(cur, ':');
			if (nextColon == NULL) continue;
			if (cur == nextColon)
			{
				// No real button name, just return the default
				if (s) strcpy(s, DefaultButtonName(button));
			}
			else
			{
				if (s)
				{
					strncpy(s, cur, nextColon - cur);
					s[nextColon - cur] = '\0';
				}
			}
			char buf[256];
			// R
			if (!TryReadColor(&cur, &nextColon, nextComma, r)) continue;
			// G
			if (!TryReadColor(&cur, &nextColon, nextComma, g)) continue;
			// B
			if (!TryReadColor(&cur, &nextColon, nextComma, b)) continue;
			found = true;
		}
		if (found) break;
	}
	printf("GUID: %s Name: \'%s\'\n", guidBuf, SDL_JoystickName(joystick));

	if (!found)
	{
		if (s)
		{
			// Use our own defaults for the button names
			strcpy(s, DefaultButtonName(button));
		}
		const SDL_Color defaultColor = DefaultButtonColor(button);
		if (r)
		{
			*r = defaultColor.r;
		}
		if (g)
		{
			*g = defaultColor.g;
		}
		if (b)
		{
			*b = defaultColor.b;
		}
	}
	return 0;
}

static const char *DefaultButtonName(SDL_GameControllerButton button)
{
	switch (button)
	{
	case SDL_CONTROLLER_BUTTON_A: return "A";
	case SDL_CONTROLLER_BUTTON_B: return "B";
	case SDL_CONTROLLER_BUTTON_X: return "X";
	case SDL_CONTROLLER_BUTTON_Y: return "Y";
	case SDL_CONTROLLER_BUTTON_BACK: return "Back";
	case SDL_CONTROLLER_BUTTON_GUIDE: return "Guide";
	case SDL_CONTROLLER_BUTTON_START: return "Start";
	case SDL_CONTROLLER_BUTTON_LEFTSTICK: return "Left Stick";
	case SDL_CONTROLLER_BUTTON_RIGHTSTICK: return "Right Stick";
	case SDL_CONTROLLER_BUTTON_LEFTSHOULDER: return "LB";
	case SDL_CONTROLLER_BUTTON_RIGHTSHOULDER: return "RB";
	case SDL_CONTROLLER_BUTTON_DPAD_UP: return "D-pad Up";
	case SDL_CONTROLLER_BUTTON_DPAD_DOWN: return "D-pad Down";
	case SDL_CONTROLLER_BUTTON_DPAD_LEFT: return "D-pad Left";
	case SDL_CONTROLLER_BUTTON_DPAD_RIGHT: return "D-pad Right";
	default: return "";
	}
}

static SDL_Color NewColor(Uint8 r, Uint8 g, Uint8 b);

static SDL_Color DefaultButtonColor(SDL_GameControllerButton button)
{
	// Default colors for Xbox 360 controller
	switch (button)
	{
	case SDL_CONTROLLER_BUTTON_A: return NewColor(96, 160, 0);
	case SDL_CONTROLLER_BUTTON_B: return NewColor(240, 0, 0);
	case SDL_CONTROLLER_BUTTON_X: return NewColor(0, 96, 208);
	case SDL_CONTROLLER_BUTTON_Y: return NewColor(255, 160, 0);
	case SDL_CONTROLLER_BUTTON_BACK: return NewColor(224, 224, 224);
	case SDL_CONTROLLER_BUTTON_GUIDE: return NewColor(128, 176, 0);
	case SDL_CONTROLLER_BUTTON_START: return NewColor(224, 224, 224);
	case SDL_CONTROLLER_BUTTON_LEFTSTICK: return NewColor(96, 128, 128);
	case SDL_CONTROLLER_BUTTON_RIGHTSTICK: return NewColor(96, 128, 128);
	case SDL_CONTROLLER_BUTTON_LEFTSHOULDER: return NewColor(224, 224, 224);
	case SDL_CONTROLLER_BUTTON_RIGHTSHOULDER: return NewColor(224, 224, 224);
	case SDL_CONTROLLER_BUTTON_DPAD_UP: return NewColor(96, 128, 128);
	case SDL_CONTROLLER_BUTTON_DPAD_DOWN: return NewColor(96, 128, 128);
	case SDL_CONTROLLER_BUTTON_DPAD_LEFT: return NewColor(96, 128, 128);
	case SDL_CONTROLLER_BUTTON_DPAD_RIGHT: return NewColor(96, 128, 128);
	default: return NewColor(0, 0, 0);
	}
}

static const char *DefaultAxisName(SDL_GameControllerAxis axis);
static SDL_Color DefaultAxisColor(SDL_GameControllerAxis axis);

int SDLJBN_GetAxisNameAndColor(SDL_Joystick *joystick,
                               SDL_GameControllerAxis axis,
                               char *s, Uint8 *r, Uint8 *g, Uint8 *b)
{
	if (joystick == NULL)
	{
		err = "joystick is NULL";
		return -1;
	}
	if (axis < SDL_CONTROLLER_AXIS_LEFTX || axis >= SDL_CONTROLLER_AXIS_MAX)
	{
		err = "axis is invalid";
		return -1;
	}
	char guidBuf[256];
	SDL_JoystickGetGUIDString(SDL_JoystickGetGUID(joystick), guidBuf, 256);
	printf("GUID: %s Name: \'%s\'\n", guidBuf, SDL_JoystickName(joystick));

	if (s)
	{
		// Use our own defaults for the axis names
		strcpy(s, DefaultAxisName(axis));
	}
	const SDL_Color defaultColor = DefaultAxisColor(axis);
	if (r)
	{
		*r = defaultColor.r;
	}
	if (g)
	{
		*g = defaultColor.g;
	}
	if (b)
	{
		*b = defaultColor.b;
	}
	return 0;
}

static const char *DefaultAxisName(SDL_GameControllerAxis axis)
{
	switch (axis)
	{
	case SDL_CONTROLLER_AXIS_LEFTX: return "Left Stick X";
	case SDL_CONTROLLER_AXIS_LEFTY: return "Left Stick Y";
	case SDL_CONTROLLER_AXIS_RIGHTX: return "Right Stick X";
	case SDL_CONTROLLER_AXIS_RIGHTY: return "Right Stick Y";
	case SDL_CONTROLLER_AXIS_TRIGGERLEFT: return "LT";
	case SDL_CONTROLLER_AXIS_TRIGGERRIGHT: return "RT";
	default: return "";
	}
}

static SDL_Color DefaultAxisColor(SDL_GameControllerAxis axis)
{
	// Default colors for Xbox 360 controller
	switch (axis)
	{
	case SDL_CONTROLLER_AXIS_LEFTX: return NewColor(96, 128, 128);
	case SDL_CONTROLLER_AXIS_LEFTY: return NewColor(96, 128, 128);
	case SDL_CONTROLLER_AXIS_RIGHTX: return NewColor(96, 128, 128);
	case SDL_CONTROLLER_AXIS_RIGHTY: return NewColor(96, 128, 128);
	case SDL_CONTROLLER_AXIS_TRIGGERLEFT: return NewColor(224, 224, 224);
	case SDL_CONTROLLER_AXIS_TRIGGERRIGHT: return NewColor(224, 224, 224);
	default: return NewColor(0, 0, 0);
	}
}

static SDL_Color NewColor(Uint8 r, Uint8 g, Uint8 b)
{
	SDL_Color c;
	c.r = r;
	c.g = g;
	c.b = b;
	c.a = 255;
	return c;
}

static bool TryReadColor(
	const char **cur, const char **nextColon, const char *nextComma,
	Uint8 *component)
{
	*cur = *nextColon + 1;
	*nextColon = strchr(*cur, ':');
	if (*nextColon == NULL) *nextColon = nextComma;
	if (*cur == *nextColon) return false;
	if (component)
	{
		char buf[256];
		strncpy(buf, *cur, *nextColon - *cur);
		*component = atoi(buf);
	}
}

const char *SDLJBN_GetError(void)
{
	return err;
}
