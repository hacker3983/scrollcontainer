#ifndef _APP_H
#define _APP_H
#include <stdlib.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "scrollcontainer.h"
#define COLOR_TOPARAM(color) color.r, color.g, color.b, color.a

typedef struct content {
	char* text;
	SDL_Rect canvas;
} content_t;

typedef struct app {
	int quit,
	    win_width, win_height;
	SDL_Window* window;
	SDL_Renderer* renderer;
	SDL_Event event;
	TTF_Font* font;
	content_t* content_list;
	scrollcontainer_t scroll_container;
	size_t content_count;
} app_t;

void app_init();
void app_create(app_t* app);
void app_readtext_list(app_t* app);
void app_run(app_t* app); 
void app_destroy(app_t* app);
#endif
