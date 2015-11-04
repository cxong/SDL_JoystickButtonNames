#include <stdio.h>
#include <SDL.h>
#include <SDL_joystickbuttonnames.h>

int main(int argc, char *argv[])
{
	SDL_Init(SDL_INIT_GAMECONTROLLER);
	SDLJBN_Init();

	SDL_GameControllerAddMappingsFromFile("gamecontrollerdb.txt");
	SDLJBN_AddMappingsFromFile("../gamecontrollerbuttondb.txt");

	for (int i = 0; i < SDL_NumJoysticks(); i++)
	{
		SDL_GameController *gc = SDL_GameControllerOpen(i);
		SDL_Joystick *j = SDL_GameControllerGetJoystick(gc);

		char buf[256];
		Uint8 r, g, b;

		SDLJBN_GetButtonNameAndColor(j, SDL_CONTROLLER_BUTTON_A, buf, &r, &g, &b);
		printf("On joystick %d, button A is %s with color rgb(%d,%d,%d)\n", i, buf, r, g, b);

		SDLJBN_GetAxisNameAndColor(j, SDL_CONTROLLER_AXIS_TRIGGERLEFT, buf, &r, &g, &b);
		printf("On joystick %d, left trigger is %s with color rgb(%d,%d,%d)\n", i, buf, r, g, b);

		SDL_GameControllerClose(gc);
	}

	SDLJBN_Quit();
	SDL_Quit();
	return 0;
}
