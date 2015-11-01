#include <stdio.h>

#include <SDL.h>

#include <SDL_joystickbuttonnames.h>


// Detect all the currently connected joysticks and write a HTML page showing
// their names, buttons and button colors.
int main(int argc, char *argv[])
{
	FILE *f = NULL;

	// Init SDL
	if (SDL_Init(SDL_INIT_GAMECONTROLLER) != 0)
	{
		printf("SDL_Init error: %s\n", SDL_GetError());
		goto bail;
	}

	// Load game controller mappings
	// https://github.com/gabomdq/SDL_GameControllerDB
	if (SDL_GameControllerAddMappingsFromFile("gamecontrollerdb.txt") == -1)
	{
		printf("Cannot load controller mappings file: %s\n", SDL_GetError());
		goto bail;
	}

	// Open output file
	f = fopen("out.html", "w");
	if (f == NULL)
	{
		perror("Failed to open output file");
		goto bail;
	}

	// Detect all current controllers
	const int numJoysticks = SDL_NumJoysticks();
	fprintf(f, "<html><body><h1>%d joystick(s) detected</h1>", numJoysticks);
	for (int i = 0; i < numJoysticks; i++)
	{
		fprintf(f, "<h2>Joystick %d: ", i);
		if (!SDL_IsGameController(i))
		{
			fprintf(f, "Not a game controller</h2>");
			continue;
		}

		SDL_GameController *gc = SDL_GameControllerOpen(i);
		if (gc == NULL)
		{
			fprintf(f, "Error</h2>");
			printf("Cannot open game controller: %s\n", SDL_GetError());
			continue;
		}

		SDL_Joystick *j = SDL_GameControllerGetJoystick(gc);
		if (j == NULL)
		{
			fprintf(f, "Error</h2>");
			printf("Cannot get joystick: %s\n", SDL_GetError());
			SDL_GameControllerClose(gc);
			continue;
		}

		fprintf(f, "(%s): ", SDL_JoystickName(j));

		fprintf(f, "<br>");
		for (SDL_GameControllerButton button = SDL_CONTROLLER_BUTTON_A;
			button < SDL_CONTROLLER_BUTTON_MAX;
			button++)
		{
			char buf[256];
			Uint8 r, g, b;
			if (SDLJBN_GetButtonNameAndColor(j, button, buf, &r, &g, &b) != 0)
			{
				printf("Cannot get button: %s\n", SDLJBN_GetError());
				break;
			}
			fprintf(f, "<span style=\"color: rgb(%d,%d,%d)\">%s</span> ",
				r, g, b, buf);
		}

		fprintf(f, "<br>");
		for (SDL_GameControllerAxis axis = SDL_CONTROLLER_AXIS_LEFTX;
			axis < SDL_CONTROLLER_AXIS_MAX;
			axis++)
		{
			char buf[256];
			Uint8 r, g, b;
			if (SDLJBN_GetAxisNameAndColor(j, axis, buf, &r, &g, &b) != 0)
			{
				printf("Cannot get axis: %s\n", SDLJBN_GetError());
				break;
			}
			fprintf(f, "<span style=\"color: rgb(%d,%d,%d)\">%s</span> ",
				r, g, b, buf);
		}

		SDL_GameControllerClose(gc);
	}
	fprintf(f, "</body></html>");

bail:
	fclose(f);
	SDL_Quit();
	return 0;
}
