#include <SDL2/SDL.h>
#include <stdlib.h>
#define T_BLUE 50
#define T_RED 20
#define T_GREEN 35
#define wait 250

unsigned int now;
unsigned int old = 0;

typedef struct colors{
	int width;
	int length;
	int speed;
	short R;
	short G;
	short B;
} Color;

typedef struct square{
	int x;
	int y;
	Color* c;
} Square;

void creating_enemies (Square* enemie, Color* color){
	enemie->x = rand()% (641-color->width);
	enemie->y = rand()% (481-color->length);
	enemie->c = color;
}

void update_enemies_x (Square* enemie){
	if(enemie->x + enemie->c->width + enemie->c->speed >= 640){
		enemie-> x = 640 - enemie->c->width;
		enemie-> c->speed = enemie-> c->speed * (-1);
		return;
	}

	if(enemie->x + enemie->c->speed <= 0){
		enemie-> x = 0;
		enemie-> c->speed = enemie-> c->speed * (-1);
		return; 
	}

	enemie->x += enemie->c->speed;
	return;	
}

void update_enemies_y (Square* enemie){
	if(enemie-> y + enemie->c->length + enemie->c->speed >= 480){
		enemie-> y = 480 - enemie->c->length;
		enemie-> c->speed = enemie-> c->speed * (-1);
		return;
	}

	if(enemie->y + enemie->c->speed <= 0){
		enemie-> y = 0;
		enemie-> c->speed = enemie-> c->speed * (-1);
		return;
	}

	enemie->y += enemie->c->speed;
	return;	
}

int main(int argc, char* args[]){

	//INICIALIZATION
	SDL_Init(SDL_INIT_EVERYTHING);
	SDL_Window* window = SDL_CreateWindow("Squares", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640,480, SDL_WINDOW_SHOWN);
	SDL_Renderer* renderer = SDL_CreateRenderer(window, 1, 0);
	SDL_Event e;

	//Declaration of Colors
	Color blue;
	blue.width = T_BLUE;
	blue.length = T_BLUE;
	blue.speed = 10;
	blue.R = 0x00;
	blue.G = 0x00;
	blue.B = 0xFF;

	Color red;
	red.width = T_RED;
	red.length = T_RED;
	red.speed = 30;
	red.R = 0xFF;
	red.G = 0x00;
	red.B = 0x00;

	Color green;
	green.width = T_GREEN;
	green.length = T_GREEN;
	green.speed = 20;
	red.R = 0x00;
	red.G = 0xFF;
	red.B = 0x00;

	//Generic Enemy
	Square g_enemy

	//Declaration of the hero
	Square hero;
	hero.x = 320; 
	hero.y = 240; 
	hero.c = &green; // as green is the all-around color, it's set as the default color

	//Declaration of Enemies
	Square enemie1;
	creating_enemies(&enemie1, &green);


	//EXECUTION
	while(1){

		SDL_PoolEvent(&e);
		now = SDL_GetTicks();

		if(e != 0){

		}
		
		if(now > old + wait){
			update_enemies_x(&enemie1);
			update_enemies_y(&enemie1);
		}

		SDL_SetRenderDrawColor(renderer, 0x00,0x00,0x00,0x00);
		SDL_RenderFillRect(renderer, NULL);

		if(now > old + wait){
			g_enemy = enemie1;
			SDL_SetRenderDrawColor(renderer, g_enemy->c->R,g_enemy->c->G,g_enemy->c->B,0x00);
			SDL_RenderFillRect(renderer, {g_enemy->x, g_enemy->y, g_enemy->width, g_enemy->length});
		} 
	} 

	//FINALIZATION
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}
