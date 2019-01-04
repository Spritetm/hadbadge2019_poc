#include <SDL.h>
#include <stdio.h>
#include <stdint.h>
#include "sdl_glue.h"

#define FBW 320
#define FBH 240
#define SCREEN_WIDTH FBW*2
#define SCREEN_HEIGHT FBH*2

SDL_Window* window = NULL;
SDL_Surface* screen_surface = NULL; 
SDL_Surface* render_surf = NULL;


int gfx_init() {
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
		return 0;
	}
	window=SDL_CreateWindow("HaDRiscVBadgeEmuThing", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
	if (!window) {
		printf("SDL could not create window! SDL_Error: %s\n", SDL_GetError());
		return 0;
	}
	screen_surface=SDL_GetWindowSurface(window);
	render_surf=SDL_CreateRGBSurface(0, FBW, FBH, 32, 0xff0000, 0xff00, 0xff, 0);
}


static uint32_t convpix(uint8_t col) {
	uint32_t ret=0;
	if (col&(0x80)) ret|=0x800000;
	if (col&(0x40)) ret|=0x400000;
	if (col&(0x20)) ret|=0x200000;
	if (col&(0x10)) ret|=0x008000;
	if (col&(0x08)) ret|=0x004000;
	if (col&(0x04)) ret|=0x002000;
	if (col&(0x02)) ret|=0x000080;
	if (col&(0x01)) ret|=0x000040;
	return ret;
}

void gfx_clear() {
	SDL_FillRect(render_surf, NULL, SDL_MapRGB(render_surf->format, 0, 0, 0));
}

void gfx_render_fbmem_scanline(int ypos, const uint8_t *scanline) {
	SDL_LockSurface(render_surf);
	uint32_t *pixels=(uint32_t*)render_surf->pixels;
	for (int x=0; x<FBW; x++) {
		pixels[ypos*(render_surf->pitch/4)+x]=convpix(*scanline++);
	}
	SDL_UnlockSurface(render_surf);
}

int gfx_flip() {
	SDL_BlitScaled(render_surf, NULL, screen_surface, NULL);
	SDL_UpdateWindowSurface(window);
}

static int buttons=0;

int gfx_poll_event() {
	SDL_Event e;
	while(SDL_PollEvent(&e)!=0) {
		if (e.type==SDL_QUIT) {
			return 1;
		} else if (e.type==SDL_KEYDOWN || e.type==SDL_KEYUP) {
			if (e.key.keysym.sym==SDLK_ESCAPE && e.type==SDL_KEYDOWN) return 1;
			int mask;
			if (e.key.keysym.sym==SDLK_UP) mask=BUTTON_UP;
			if (e.key.keysym.sym==SDLK_DOWN) mask=BUTTON_DOWN;
			if (e.key.keysym.sym==SDLK_LEFT) mask=BUTTON_LEFT;
			if (e.key.keysym.sym==SDLK_RIGHT) mask=BUTTON_RIGHT;
			if (e.type==SDL_KEYDOWN) buttons|=mask; else buttons&=~mask;
		}
	}
	return 0;
}

int gfx_get_buttons() {
	return buttons;
}
