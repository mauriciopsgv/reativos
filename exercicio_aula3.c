#include <SDL2/SDL.h>
#include <stdio.h>
#define HERO_W 30
#define HERO_L 30
#define WALL_W 20
#define WALL_L 480
#define SCREEN_X 640
#define SCREEN_Y 480
#define GRAVITY_DT 60
#define GRAVITY_ACC 8
#define MIN_VEL 10
#define MAX_VEL 57

int cont_cores=1;
unsigned long now, dt, old;

typedef struct color{
	int R;
	int G;
	int B;
}	Color;

typedef struct square{
	int x;
	int y;
	float vel;
	Color* c;
}	Square;

//Gravity
void gravity (Square* hero){

	if(now - old >= GRAVITY_DT - hero->vel){
		hero->y += 1;
		old = SDL_GetTicks();
		hero->vel += GRAVITY_ACC;
		if(hero->vel > MAX_VEL)
			hero->vel = MAX_VEL;
	}
}

int move_square (Square* hero, int flying){
	if(now - old >= GRAVITY_DT - hero->vel){
		hero->y -= 1;
		old = SDL_GetTicks();
		hero->vel -= flying/40;
		if(hero->vel < MIN_VEL)
			hero->vel = MIN_VEL;

		flying++;
	}

	return flying;
}

int check_ground (Square* hero){
	if(hero->y + HERO_L >= SCREEN_Y)
		return 1;
	return 0;

}

void change_color (Square * hero, Color * colors){
	cont_cores++;
	hero->x = hero->x + HERO_W/2;
	hero->y = hero->y + HERO_L/2;
	
	hero->c = colors[cont_cores%3];

	hero->x = hero->x - HERO_W/2;
	hero->y = hero->y - HERO_L/2;
}


int main (int argc, char* args[]){

	//INICIALIZATION
	SDL_Init(SDL_INIT_EVERYTHING);
	SDL_Window* window = SDL_CreateWindow("Squares", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_X, SCREEN_Y, SDL_WINDOW_SHOWN);
	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);
	SDL_Event e;
	SDL_Rect draw_h;
	int get_out = 0, flying=50;

	//Declaration of Colors
	Color blue;
	blue.R = 0x00;
	blue.G = 0x00;
	blue.B = 0xFF;

	Color red;
	red.R = 0xFF;
	red.G = 0x00;
	red.B = 0x00;

	Color green;
	green.R = 0x00;
	green.G = 0xFF;
	green.B = 0x00;

	Color colors[3] ={red, green, blue};

	//A hero is born
	Square hero = {300, 220, 0, &green};
	draw_h.w = HERO_W;
	draw_h.h = HERO_L;

	SDL_SetRenderDrawColor(renderer, 0x49,0x49,0x49,0x00);
	SDL_RenderFillRect(renderer, NULL);
	SDL_SetRenderDrawColor(renderer, hero.c->R, hero.c->G, hero.c->B, 0x00);
	draw_h.x =hero.x;
	draw_h.y =hero.y;
	SDL_RenderFillRect(renderer, &draw_h);
	SDL_RenderPresent(renderer);

	//EXECUTION

	while(1){

		now = SDL_GetTicks();
		dt = now - old;

			if(SDL_PollEvent(&e) == 1){
				if(e.type == SDL_QUIT){
					get_out = 1;
					break;
				}
				else if( e.type == SDL_KEYDOWN){
					switch (e.key.keysym.sym){
					case SDLK_UP:
						flying = 0;
						break;
					case SDLK_SPACE:
						change_color(&hero, colors);
						break;
					}
				}	
			}


		if(flying == 50){
			gravity(&hero);
			//get_out = check_ground(&hero);
		}
		else
			flying = move_square(&hero, flying);

		if(hero.y + HERO_L >= SCREEN_Y)
			hero.y = SCREEN_Y - HERO_L;

		//RENDERIZATION
		SDL_SetRenderDrawColor(renderer, 0x49,0x49,0x49,0x00);
		SDL_RenderFillRect(renderer, NULL);

		//Drawing the hero
		SDL_SetRenderDrawColor(renderer, hero.c->R, hero.c->G, hero.c->B, 0x00);
		draw_h.x =hero.x;
		draw_h.y =hero.y;
		SDL_RenderFillRect(renderer, &draw_h);

		SDL_RenderPresent(renderer);

		if(get_out){
			SDL_DestroyRenderer(renderer);
			SDL_DestroyWindow(window);
			SDL_Quit();
			exit(0);
		}
	}
	//FINALIZATION
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();

	return 0;
}
