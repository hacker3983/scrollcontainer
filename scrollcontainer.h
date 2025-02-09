#ifndef _SCROLLCONTAINER_H
#define _SCROLLCONTAINER_H
#include <stdbool.h>
#include <SDL2/SDL.h>

enum scroll_type {
	SCROLL_UP,
	SCROLL_DOWN
};

typedef struct scroll_item {
	SDL_Rect initial_canvas;
	SDL_Rect scroll_canvas;
} scroll_item_t;

typedef struct scrollcontainer {
	int scroll_y;
	SDL_Rect scroll_area;
	scroll_item_t* items;
	size_t item_renderpos;
	size_t item_count;
	bool init;
} scrollcontainer_t;

void scrollcontainer_init(scrollcontainer_t* scroll_container);
void scrollcontainer_setprops(scrollcontainer_t* scroll_container, SDL_Rect scroll_area);
void scrollcontainer_addscroll_item(scrollcontainer_t* scroll_container, SDL_Rect scroll_item);
void scrollcontainer_performscroll(scrollcontainer_t* scroll_container, int scroll_type, int speed);
void scrollcontainer_destroy(scrollcontainer_t* scroll_container);
#endif
