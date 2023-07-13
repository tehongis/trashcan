#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>

#include <stdbool.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_mixer.h>

#ifndef M_PI
#define M_PI 3.1415926535897932384626433832795
#endif

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 1024

#define MAX_NODES 4096

struct NodeObj {
    int id;
    double x;
    double y;
    struct NodeObj *prev;
    struct NodeObj *next;
};

int id_count = 0;

struct NodeObj *new_node(float x, float y) {
    struct NodeObj *newnode = (struct NodeObj*) malloc( sizeof(struct NodeObj) );
    newnode->id = id_count++;
    newnode->x = x;
    newnode->y = y;
    newnode->prev = 0;
    newnode->next = 0;

    return newnode;
}

void nodeprint(struct NodeObj *node){
    printf("Node ID: %d\n",node->id);
    printf("x: %f\n",node->x);
    printf("y: %f\n",node->y);
    printf("Next: %p\n",node->next);
    printf("Prev: %p\n",node->prev);
    printf("\n");
}

int main(void) {

    srand(time(NULL));

    struct NodeObj *prevnode = 0;
    struct NodeObj *node[MAX_NODES];

    for (int i = 0; i < MAX_NODES; i++) {
        node[i] = new_node((float) rand()/RAND_MAX - 0.5,(float) rand()/RAND_MAX - 0.5 );
        if (prevnode != 0) {
            prevnode->next = node[i];
            node[i]->prev = prevnode;
        }
        prevnode = node[i];
        
    }

    // returns zero on success else non-zero
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        printf("error initializing SDL: %s\n", SDL_GetError());
    }
    SDL_Window* win = SDL_CreateWindow("GAME",
                                       SDL_WINDOWPOS_CENTERED,
                                       SDL_WINDOWPOS_CENTERED,
                                       SCREEN_WIDTH, SCREEN_HEIGHT, 0);

    //Initialize SDL_mixer
    if( Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 2048 ) < 0 ) {
        printf( "SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError() );
        exit(0);
    }

    Mix_Music* gMusic = NULL;

    //Load music
    gMusic = Mix_LoadMUS( "impact-prelude-by-kevin-macleod-from-filmmusic-io.mp3" );
    if( gMusic == NULL ) {
        printf( "Failed to load beat music! SDL_mixer Error: %s\n", Mix_GetError() );
        exit(0);
    }

    //Play the music
    Mix_PlayMusic( gMusic, -1 );

   // triggers the program that controls
    // your graphics hardware and sets flags
    Uint32 render_flags = SDL_RENDERER_ACCELERATED;
 
    // creates a renderer to render our images
    SDL_Renderer* rend = SDL_CreateRenderer(win, -1, render_flags);
 
    // creates a surface to load an image into the main memory
    SDL_Surface* surface;
 
    // please provide a path for your image
    surface = IMG_Load("oga-tofuman-tiles.png");
    if (!surface) {
        printf("Unable to load image.\n");
        exit(0);
    }
 
    // loads image to our graphics hardware memory.
    SDL_Texture* tex = SDL_CreateTextureFromSurface(rend, surface);
 
    // clears main-memory
    SDL_FreeSurface(surface);
 
    // let us control our image position
    // so that we can move it with our keyboard.
    SDL_Rect dest;

    // connects our texture with dest to control position
    // SDL_QueryTexture(tex, NULL, NULL, &dest.w, &dest.h);

    // adjust height and width of our image box.
    dest.w = 16;
    dest.h = 16;

    SDL_Rect imgPartRect;
    imgPartRect.x = 0;
    imgPartRect.y = 0;
    imgPartRect.w = 16;
    imgPartRect.h = 16;

    // controls animation loop
    int RUNNING = true;
 
    // speed of box
    int speed = 300;

    float angle = 0.0;

 // animation loop
    while (RUNNING) {
        SDL_Event event;
 
        // Events management
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
 
            case SDL_QUIT:
                // handling of close button
                RUNNING = false;
                break;
 
            case SDL_KEYDOWN:
                // keyboard API for key pressed
                switch (event.key.keysym.scancode) {
                case SDL_SCANCODE_W:
                case SDL_SCANCODE_UP:
                    dest.y -= speed / 30;
                    break;
                case SDL_SCANCODE_A:
                case SDL_SCANCODE_LEFT:
                    dest.x -= speed / 30;
                    break;
                case SDL_SCANCODE_S:
                case SDL_SCANCODE_DOWN:
                    dest.y += speed / 30;
                    break;
                case SDL_SCANCODE_D:
                case SDL_SCANCODE_RIGHT:
                    dest.x += speed / 30;
                    break;
                default:
                    break;
                }
            }
        }
/*

        // right boundary
        if (dest.x + dest.w > 1000)
            dest.x = 1000 - dest.w;
 
        // left boundary
        if (dest.x < 0)
            dest.x = 0;
 
        // bottom boundary
        if (dest.y + dest.h > 1000)
            dest.y = 1000 - dest.h;
 
        // upper boundary
        if (dest.y < 0)
            dest.y = 0;

 */

        // clears the screen
        SDL_RenderClear(rend);
        //SDL_RenderCopy( rend, tex, NULL, &dest);

        dest.w = 16;
        dest.h = 16;

        for (int i = 0; i < MAX_NODES; i++) {

            float mult = 2048 * sin(angle);

            int xloc = (int) ( node[i]->x * mult * sin(angle) ) - ( node[i]->y * mult * cos(angle) );
            int yloc = (int) ( node[i]->x * mult * cos(angle) ) + ( node[i]->y * mult * sin(angle) );
            // sets initial x-position of object
            dest.x = ((SCREEN_WIDTH- dest.w) / 2 + xloc );
            // sets initial y-position of object
            dest.y = ((SCREEN_HEIGHT - dest.h) / 2 + yloc );

            SDL_RenderCopy( rend, tex, &imgPartRect, &dest );
        }

        angle = angle + 0.01;
        if ( angle > M_PI*2 ) {
            angle = angle - M_PI*2;
        }

        // triggers the double buffers
        // for multiple rendering
        SDL_RenderPresent(rend);
 
        // calculates to 60 fps
        SDL_Delay(1000 / 60);
    }
 
    //Free the music
    Mix_FreeMusic( gMusic );
    gMusic = NULL;

    // destroy texture
    SDL_DestroyTexture(tex);
 
    // destroy renderer
    SDL_DestroyRenderer(rend);
 
    // destroy window
    SDL_DestroyWindow(win);
     
    // close SDL
    SDL_Quit();

/*
    for (int i = 0; i < MAX_NODES; i++) {
        nodeprint(node[i]);
    }
*/
    for (int i = 0; i < MAX_NODES; i++) {
        free(node[i]);
    }

    return 0;
}
