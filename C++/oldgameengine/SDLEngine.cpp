
#include <iostream>
#include <cmath>

#include <SDL2/SDL_events.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL.h>
#undef main

/*
From Box2D documentation:
What are the biggest mistakes made by new users?
Using pixels for length instead of meters.
Expecting Box2D to give pixel perfect results.
Using b2Polygon to create concave polygons.
Testing their code in release mode.
Not learning C++ before using Box2D.
Not reading this FAQ. :)
*/
#include <Box2D/Box2D.h>
#undef main


int main() {

  int running = 1;      // If this keep as 1, we have a go. Any init fails will set zero and let the program exit with error.

  const int WIDTH = 1280;
  const int HEIGHT = 1024;
  const int CX = WIDTH / 2;
  const int CY = HEIGHT / 2;
  const int SCREEN_FPS = 60;
  const int SCREEN_TICKS_PER_FRAME = 1000 / SCREEN_FPS;

  SDL_Event event;
  SDL_Window *window = NULL;
  SDL_Renderer *renderer = NULL;
  SDL_Texture *sprite = NULL;
  SDL_Rect texr;

  int w, h;

  float countA = 0.0;
  float countB = 0.4;
  float countC = 1.1;

  //Current time start time
  Uint32 startTime;

  int before_ticks;
  int current_ticks;

  //world
  b2Vec2 gravity(0.0f, -10.0f);
  b2World world = b2World(gravity);


  //ground
  b2BodyDef groundBodyDef;
  groundBodyDef.position.Set(0.0f, -10.0f);

  b2Body* groundBody = world.CreateBody(&groundBodyDef);
  
  b2PolygonShape groundBox;
  groundBox.SetAsBox(50.0f, 10.0f);
  groundBody->CreateFixture(&groundBox, 0.0f);

  //ship
  b2BodyDef bodyDef;
  bodyDef.type = b2_dynamicBody;
  bodyDef.position.Set(0.0f, 4.0f);
  b2Body* body = world.CreateBody(&bodyDef);

  b2PolygonShape dynamicBox;
  dynamicBox.SetAsBox(1.0f, 1.0f);

  b2FixtureDef fixtureDef;
  fixtureDef.shape = &dynamicBox;
  fixtureDef.density = 1.0f;
  fixtureDef.friction = 0.3f;

  body->CreateFixture(&fixtureDef);

  // simulation settings
  float timeStep = 1.0f / 60.0f;

  int32 velocityIterations = 6;
  int32 positionIterations = 2;

  if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
    printf("ERR: Failed to initialize SDL: %s\n", SDL_GetError());
    running = 0;
  }
  window = SDL_CreateWindow("Title goes here", SDL_WINDOWPOS_CENTERED,
                            SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, 0);

  renderer = SDL_CreateRenderer(window, -1, 0);
  if (renderer == NULL) {
      printf("ERR: Renderer failed.\n", SDL_GetError());
      running = 0;
  }
    
  sprite = IMG_LoadTexture(renderer, "images/sprite.png");
  if (sprite == NULL) {
      printf("ERR: Imageload failed: %s\n", SDL_GetError());
      running = 0;
  }
  
  SDL_QueryTexture(sprite, NULL, NULL, &w, &h);
  
  current_ticks = SDL_GetTicks();
  before_ticks = current_ticks;

  while (running) {

    while (SDL_PollEvent(&event)) {
      switch (event.type) {
      case SDL_QUIT: {
        running = 0;
        break;
        }
      }
    }

    const Uint8 *state = SDL_GetKeyboardState(NULL);
    if (state[SDL_SCANCODE_ESCAPE]) {
      printf("Escape is pressed.\n");
      running = 0;
    }


    // simulate world
    world.Step(timeStep, velocityIterations, positionIterations);
    b2Vec2 position = body->GetPosition();
    float angle = body->GetAngle();
    printf("%4.2f %4.2f %4.2f\n", position.x, position.y, angle);


    SDL_SetRenderDrawColor(renderer, 30, 40, 50, 255);
    SDL_RenderClear(renderer);

    countA = countA + 0.03;
    if (countA > M_PI * 2) {
        countA = countA - M_PI * 2;
    }

    countB = countB + 0.027;
    if (countB > M_PI * 2) {
        countB = countB - M_PI * 2;
    }

    countC = countC + 0.043;
    if (countC > M_PI * 2) {
        countC = countC - M_PI * 2;
    }

    for (int i = 0; i < 512; i++) {
        float f = (i / 100.0);
        texr.x = int( CX + 200 * ( sin( countA + f ) + sin( countB + f ) + sin(countC + f) ) );
        texr.y = int( CY + 200 * ( cos( countA + f ) + sin( countB + f ) + sin(countC + f) ) );
        texr.w = w * 2;
        texr.h = h * 2;

        // SDL_RenderCopy(renderer, sprite, NULL, &texr);
        double angle = f + countA * 360.0;
        SDL_RenderCopyEx(renderer, sprite, NULL, &texr, angle, NULL, SDL_FLIP_NONE);
    }

    SDL_RenderPresent(renderer);
    current_ticks = SDL_GetTicks();
    printf("Ticks: %d",current_ticks-before_ticks);
    before_ticks = current_ticks;
    SDL_Delay(50);
  }

  SDL_DestroyTexture(sprite);
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);

  SDL_Quit();
  return 0;
}
