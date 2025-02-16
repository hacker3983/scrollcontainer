#include "scrollcontainer.h"

void scrollcontainer_init(scrollcontainer_t* scroll_container) {
	scroll_container->init = true;
}

void scrollcontainer_setprops(scrollcontainer_t* scroll_container, SDL_Rect scroll_area) {
	if(scroll_container->init) {
		return;
	}
	scroll_container->scroll_area = scroll_area;
	scroll_container->scroll_y = scroll_area.y;
	scroll_container->items = NULL;
	scroll_container->item_count = 0;
}

void scrollcontainer_addscroll_item(scrollcontainer_t* scroll_container, SDL_Rect scroll_item) {
	if(scroll_container->init) {
		return;
	}
	scroll_item_t* new_items = NULL;
	size_t new_itemcount = 0;
	if(!scroll_container->items) {
		new_items = malloc(sizeof(scroll_item_t));
		new_itemcount = 1;
	} else {
		new_itemcount = scroll_container->item_count + 1;
		new_items = realloc(scroll_container->items, new_itemcount * sizeof(scroll_item_t));
	}
	if(!new_items) {
		return;
	}
	new_items[new_itemcount-1].initial_canvas = scroll_item;
	new_items[new_itemcount-1].scroll_canvas = scroll_item;
	scroll_container->items = new_items;
	scroll_container->item_count = new_itemcount;
}

void scrollcontainer_performscroll(scrollcontainer_t* scroll_container, int scroll_type, int speed) {
	SDL_Rect scroll_area = scroll_container->scroll_area;
	size_t item_renderpos = scroll_container->item_renderpos;
	if(!scroll_container->init) {
		return;
	}
	if(scroll_type == SCROLL_DOWN) {
		int disappear_y = scroll_area.y - scroll_container->items[0].scroll_canvas.h;
		scroll_container->scroll_y -= speed;
		if(scroll_container->scroll_y <= disappear_y) {
			scroll_container->item_renderpos++;
			free(scroll_container->items);
			scroll_container->items = NULL;
			scroll_container->init = false;
		}
	} else if(scroll_type == SCROLL_UP) {
		if(scroll_container->scroll_y < scroll_area.y) {
			scroll_container->scroll_y += speed;
			if(scroll_container->scroll_y >= scroll_area.y) {
				scroll_container->scroll_y = scroll_area.y;
				if(scroll_container->item_renderpos) {
					scroll_container->item_renderpos--;
				}
			}
		} else if(scroll_container->item_renderpos) {
			scroll_container->item_renderpos--;
			scroll_container->scroll_y = scroll_area.y;
		}
	}
}

void scrollcontainer_destroy(scrollcontainer_t* scroll_container) {
	scroll_container->scroll_area = (SDL_Rect){0};
	free(scroll_container->items); scroll_container->items = NULL;
	scroll_container->item_renderpos = 0;
	scroll_container->item_count = 0;
	scroll_container->init = false;
}
