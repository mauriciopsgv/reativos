#include <SDL2/SDL.h>
#include <stdlib.h>
#include <time.h>
#define HERO_W 30
#define HERO_L 30
#define WALL_W 20
#define WALL_L 480
#define SCREEN_X 640
#define SCREEN_Y 480
#define GRAVITY_DT 62
#define GRAVITY_ACC 8
#define MIN_VEL 10
#define MAX_VEL 57
#define SIDEWAYS_DT 3

int cont_cores=1, side=1;
unsigned long now, old_s=0, old_g=0;

typedef struct color{
	int R;
	int G;
	int B;
}	Color;

typedef struct wall{
	int x[2];
	int y[2];
	Color* c;
}	Wall;


typedef struct square{
	int x;
	int y;
	float vel;
	Color* c;
}	Square;

//Gravity
void gravity (Square* hero){

	if(now - old_g >= GRAVITY_DT - hero->vel){
		hero->y += 1;
		old_g = SDL_GetTicks();
		hero->vel += GRAVITY_ACC;
		if(hero->vel > MAX_VEL)
			hero->vel = MAX_VEL;
	}
}

int move_square (Square* hero, int flying){
	if(now - old_g >= GRAVITY_DT - hero->vel){
		hero->y -= 1;
		old_g = SDL_GetTicks();
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

void change_wall_color ( Wall *w, Color * colors)
{
	int color_index = rand()%3;
	w->c = &colors[color_index];
}

void change_color (Square * hero, Color * colors){
	cont_cores++;
	hero->x = hero->x + HERO_W/2;
	hero->y = hero->y + HERO_L/2;
	
	hero->c = &colors[cont_cores%3];

	hero->x = hero->x - HERO_W/2;
	hero->y = hero->y - HERO_L/2;
}

void move_sideways (Square * hero){
	if(now - old_s >= SIDEWAYS_DT){
		hero->x += side;
		old_s = SDL_GetTicks();
	}
}

int bouncing_walls( Square* hero, Wall * w, Color * colors){
	if(side == 1){
		if(hero->x + HERO_W >= w->x[1]){
			if(hero->c->R == w->c->R){
				if(hero->c->G == w->c->G){
					side = side*(-1);
					change_wall_color(w, colors);
				}
				else
					return 1;
			}
			else
				return 1;
		}
	}
	else{
		if(hero->x <= w->x[0] + WALL_W){
			if(hero->c->R == w->c->R){
				if(hero->c->G == w->c->G){
					side = side*(-1);
					change_wall_color(w, colors);
				}
				else
					return 1;	
			}
			else
				return 1;
		}
	}

	return 0;
}

void finalization_error (SDL_Renderer* renderer, SDL_Window* window){
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
	exit(0);
}


int main (int argc, char* args[]){

	//INICIALIZATION
	srand(time(NULL));
	SDL_Init(SDL_INIT_EVERYTHING);
	SDL_Window* window = SDL_CreateWindow("Squares", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_X, SCREEN_Y, SDL_WINDOW_SHOWN);
	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);
	SDL_Event e;
	SDL_Rect draw_hero, draw_wall;
	int i,get_out_g = 0, get_out_w=0, flying=50;

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
	draw_hero.w = HERO_W;
	draw_hero.h = HERO_L;

	//The walls are built
	Wall w = {{0, SCREEN_X - WALL_W}, {0,0}, &green};
	draw_wall.w = WALL_W;
	draw_wall.h = WALL_L;


	//EXECUTION

	while(1){

		now = SDL_GetTicks();

			if(SDL_PollEvent(&e) == 1){
				if(e.type == SDL_QUIT){
					finalization_error(renderer,window);
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

		move_sideways(&hero);
		if(flying == 50){
			gravity(&hero);
			get_out_g = check_ground(&hero);
		}
		else
			flying = move_square(&hero, flying);

		get_out_w = bouncing_walls(&hero, &w, colors);


		//RENDERIZATION
		SDL_SetRenderDrawColor(renderer, 0x49,0x49,0x49,0x00);
		SDL_RenderFillRect(renderer, NULL);

		//Drawing the hero
		SDL_SetRenderDrawColor(renderer, hero.c->R, hero.c->G, hero.c->B, 0x00);
		draw_hero.x =hero.x;
		draw_hero.y =hero.y;
		SDL_RenderFillRect(renderer, &draw_hero);

		//Drawing the Walls
		SDL_SetRenderDrawColor(renderer, w.c->R, w.c->G, w.c->B, 0x00);
		for (i=0; i<2; i++)
		{
			draw_wall.x = w.x[i];
			draw_wall.y = w.y[i];
			SDL_RenderFillRect(renderer, &draw_wall);
		}

		SDL_RenderPresent(renderer);
	

		if(get_out_g || get_out_w){

			finalization_error(renderer,window);
		}
	}
	//FINALIZATION
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();

	return 0;
}
