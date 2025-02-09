#include "app.h"

void app_init() {
	SDL_Init(SDL_INIT_VIDEO);
	TTF_Init();
}

void app_create(app_t* app) {
	app_init();
	app->window = SDL_CreateWindow("Scrolling System", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 900, 600, SDL_WINDOW_RESIZABLE);
	app->renderer = SDL_CreateRenderer(app->window, -1, SDL_RENDERER_ACCELERATED);
	app->font = TTF_OpenFont("Roboto-Black.ttf", 20);
	app->quit = false;
}

void app_setrender_color(app_t* app, SDL_Color color) {
	SDL_SetRenderDrawColor(app->renderer, COLOR_TOPARAM(color));
}

void app_setwindow_color(app_t* app, SDL_Color window_color) {
	app_setrender_color(app, window_color);
	SDL_RenderClear(app->renderer);
}

void app_readtext_list(app_t* app) {
	content_t* content_list = NULL;
	size_t content_count = 0;
	FILE* f = fopen("textlist.txt", "r");
	char* buff = NULL;
	printf("Contents:\n");
	while(fscanf(f, "%ms", &buff) != EOF) {
		if(!content_list) {
			content_list = malloc(sizeof(content_t));
		} else {
			content_list = realloc(content_list, (content_count+1) * sizeof(content_t));
		}
		memset(content_list+content_count, 0, sizeof(content_t));
		printf("%zu. %s\n", content_count, buff);
		SDL_Rect* canvas = &content_list[content_count].canvas;
		TTF_SizeUTF8(app->font, buff, &canvas->w, &canvas->h);
		content_list[content_count++].text = buff;
	}
	fclose(f);
	app->content_list = content_list;
	app->content_count = content_count;
}

void app_setcontent_positions(app_t* app) {
	SDL_Rect scroll_area = {0, 0, app->win_width, app->win_height};
	scrollcontainer_t* scroll_container = &app->scroll_container;
	size_t render_pos = scroll_container->item_renderpos;
	if(app->content_list) {
		if(scroll_container->init) {
			app->content_list[render_pos].canvas.y = scroll_container->scroll_y;
		} else {
			app->content_list[render_pos].canvas.y = scroll_area.y;
		}
	}
	for(size_t i=render_pos;i<app->content_count;i++) {
		if(i > render_pos) {
			app->content_list[i].canvas.y = app->content_list[i-1].canvas.y + app->content_list[i-1].canvas.h + 10;
		}
		if(app->content_list[i].canvas.y < scroll_area.y + scroll_area.h) {
			scrollcontainer_addscroll_item(scroll_container, app->content_list[i].canvas);
		} else {
			break;
		}
	}
}

void app_rendercontents(app_t* app) {
	scrollcontainer_t* scroll_container = &app->scroll_container;
	SDL_Color black = {0};
	size_t render_pos = scroll_container->item_renderpos,
	       item_count = scroll_container->item_count;
	for(size_t i=0;i<item_count;i++) {
		SDL_Surface* surface = TTF_RenderUTF8_Blended(app->font, app->content_list[render_pos].text, black);
		SDL_Texture* texture = SDL_CreateTextureFromSurface(app->renderer, surface);
		SDL_FreeSurface(surface);
		app->content_list[render_pos].canvas.x = 10;
		SDL_RenderCopy(app->renderer, texture, NULL, &app->content_list[render_pos].canvas);
		SDL_DestroyTexture(texture);
		render_pos++;
	}
}

void app_run(app_t* app) {
	SDL_Color white = {0xff, 0xff, 0xff, 0xff},
		  black = {0};
	app_readtext_list(app);
	int start_y = 0;
	scrollcontainer_t* scroll_container = &app->scroll_container;
	while(!app->quit) {
		while(SDL_PollEvent(&app->event)) {
			if(app->event.type == SDL_QUIT) {
				app->quit = true;
			} else if(app->event.type == SDL_MOUSEWHEEL) {
				if(app->event.wheel.y < 0) {
					scrollcontainer_performscroll(scroll_container, SCROLL_DOWN, 15);
					puts("Scrolling Down");
				} else if(app->event.wheel.y > 0) {
					scrollcontainer_performscroll(scroll_container, SCROLL_UP, 10);
					puts("Scrolling Up");
				}
			}
		}
		SDL_GetWindowSize(app->window, &app->win_width, &app->win_height);
		app_setwindow_color(app, white);
		SDL_Rect scroll_area = {0, 0, app->win_width, app->win_height};
		scrollcontainer_setprops(scroll_container, scroll_area);
		app_setcontent_positions(app);
		app_rendercontents(app);
		scrollcontainer_init(scroll_container);
		SDL_RenderPresent(app->renderer);
	}
}

void app_destroy(app_t* app) {
	for(size_t i=0;i<app->content_count;i++) {
		free(app->content_list[i].text);
	}
	scrollcontainer_destroy(&app->scroll_container);
	free(app->content_list);
	app->content_count = 0;
	TTF_CloseFont(app->font);
	SDL_DestroyRenderer(app->renderer);
	SDL_DestroyWindow(app->window);
	TTF_Quit();
	SDL_Quit();
}
