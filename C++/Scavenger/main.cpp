// Code by Tero Hongisto.
// Release under MIT license.

#include <iostream>

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <Box2D/Box2D.h>

#define TICK_INTERVAL 50

using namespace std;

static SDL_Renderer *gRenderer = NULL;

static Uint32 next_time;

static Uint32 time_left(void)
{
	Uint32 now;

	now = SDL_GetTicks();
	if (next_time <= now)
		return 0;
	else
		return next_time - now;
}

class gObj
{

	double objX;
	double objY;
	int objW;
	int objH;
	double objAngle;
	double objMomentum;

	SDL_Texture *objTexture;

public:
	//Default Constructor
	gObj(int x, int y, double angle, SDL_Texture *texture)
	{
		objX = x;
		objY = y;
		objAngle = angle;
		objTexture = texture;
		SDL_QueryTexture(texture, NULL, NULL, &objW, &objH);
		objMomentum = 0.0;
	}

public:
	void move()
	{
		objX = objX + objMomentum * sin(objAngle);
		objY = objY + objMomentum * cos(objAngle);
		objMomentum = objMomentum * 0.01;
	}

	void push(double pushAngle, double pushForce)
	{
		objAngle = pushAngle;
		objMomentum = pushForce;
	}

	void draw()
	{
		//Set rendering space and render to screen
		SDL_Rect renderQuad = {int(objX), int(objY), int(objW / 2), int(objH / 2)};
		//Render ship to screen
		SDL_RenderCopyEx(gRenderer, objTexture, NULL, &renderQuad, objAngle, NULL, SDL_FLIP_NONE);
	}
};

static SDL_Texture *loadTexture(std::string path)
{
	//The final texture
	SDL_Texture *newTexture = NULL;

	//Load image at specified path
	SDL_Surface *loadedSurface = IMG_Load(path.c_str());
	if (loadedSurface == NULL)
	{
		printf("Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError());
	}
	else
	{
		//Create texture from surface pixels
		newTexture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
		if (newTexture == NULL)
		{
			printf("Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
		}

		//Get rid of old loaded surface
		SDL_FreeSurface(loadedSurface);
	}

	return newTexture;
}

int main()
{

	SDL_Window *gWindow = NULL;
	SDL_Event event;
	SDL_Joystick *joy = NULL;

	static const char *nameMusic = "music/Stardrive.mp3";
	static const char *nameBackground = "graphics/Aurora.jpg";
	static const char *nameBox = "graphics/RTS_Crate.png";
	static const char *nameShip = "graphics/Ship_A.png";

	static SDL_Texture *textureBackground = NULL;
	static SDL_Texture *textureShip = NULL;
	static SDL_Texture *textureBox = NULL;

	bool RUNNING = false;

	// start SDL with video and audio support
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) == -1)
	{
		printf("SDL_Init: %s\n", SDL_GetError());
		exit(1);
	}


	printf("Joysticks: %d\n", SDL_NumJoysticks());
	
	// Check for joystick
	if (SDL_NumJoysticks() > 0)
	{
		// Open joystick
		joy = SDL_JoystickOpen(0);
	}
	else
	{
		printf("SDL_Init: Couldn't open Joystick.\n");
		exit(1);
	}

	if (joy)
	{
		printf("Opened Joystick 0\n");
		printf("Name: %s\n", SDL_JoystickName(0));
		printf("Number of Axes: %d\n", SDL_JoystickNumAxes(joy));
		printf("Number of Buttons: %d\n", SDL_JoystickNumButtons(joy));
		printf("Number of Balls: %d\n", SDL_JoystickNumBalls(joy));
	}

	Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 2, 2048);

	gWindow = SDL_CreateWindow("Cave", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1024, 768, SDL_WINDOW_OPENGL);
	gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);

	textureBackground = loadTexture(nameBackground);
	textureShip = loadTexture(nameShip);
	textureBox = loadTexture(nameBox);

	gObj *ship = new gObj(120, 150, 30, textureShip);
	gObj *box = new gObj(140, 200, 20, textureBox);

	// Check that the gWindow was successfully created
	if (gWindow == NULL)
	{
		// In the case that the gWindow could not be made...
		printf("Could not create gWindow: %s\n", SDL_GetError());
		return 1;
	}
	Mix_Music *music = Mix_LoadMUS(nameMusic);

	next_time = SDL_GetTicks() + TICK_INTERVAL;

	Mix_PlayMusic(music, 1);

	RUNNING = true;
	while (RUNNING)
	{

		// Get the next event

		if (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT)
			{
				RUNNING = false;
			}
		}

		static const Uint8 *keys;
		keys = SDL_GetKeyboardState(NULL);
		if (keys[SDL_SCANCODE_ESCAPE])
		{
			RUNNING = false;
			printf("Exiting.\n");
		}

		//Render background
		SDL_RenderCopy(gRenderer, textureBackground, NULL, NULL);

		ship->move();
		box->move();

		ship->draw();
		box->draw();

		SDL_RenderPresent(gRenderer);
		SDL_Delay(time_left());
		next_time += TICK_INTERVAL;
	}

	// Destroy texture
	SDL_DestroyTexture(textureBackground);
	textureBackground = NULL;
	SDL_DestroyTexture(textureShip);
	textureShip = NULL;
	SDL_DestroyTexture(textureBox);
	textureBox = NULL;

	SDL_JoystickClose(joy);

	// Close and destroy the gWindow
	SDL_DestroyWindow(gWindow);

	// Clean up
	SDL_Quit();

	return EXIT_SUCCESS;
}
