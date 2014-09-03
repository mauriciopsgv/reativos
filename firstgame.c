#include <SDL2/SDL.h>
#include <stdlib.h>
#define T_BLUE 50
#define T_RED 20
#define T_GREEN 35

typedef struct colors{
	int width;
	int length;
	int color_speed;
} Color;

typedef struct square{
	int x;
	int y;
	int add;
	SDL_Rect r;
} Square;

void creating_enemies (Square* enemie, Color color){
	enemie->x = rand()% (641-color.width);
	enemie->y = rand()% (481-color.length);
	enemie->add = color_speed;
	enemie->r = { enemie->x, enemie->y, color.width, color.length};
}

void update_enemies (Square enemie){
	
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
	blue.color_speed = 10;

	Color red;
	red.width = T_RED;
	red.length = T_RED;
	red.color_speed = 30;

	Color green;
	green.width = T_GREEN;
	green.length = T_GREEN;
	green.color_speed = 20;

	//Declaration of the hero
	Square hero;
	hero.x = 320; 
	hero.y = 240; 
	hero.r = {hero.x, hero.y, TAM_GREEN, TAM_GREEN}; // as green is the all-around color, it's set as the default color

	//Declaration of Enemies
	Square enemie1;
	creating_enemies(enemie1, green);


	//EXECUTION
	while(1){
		SDL_PoolEvent(&e);

		if(e != 0){

		}
		


		SDL_SetRenderDrawColor(renderer, 0x00,0x00,0x00,0x00);
		SDL_RenderFillRect(renderer, NULL);
		SDL_Rect r = { 200 ,200, 25, 25};
		SDL_SetRenderDrawColor(renderer, 0x00,0x00,0xFF,0x00);
		SDL_RenderFillRect(renderer, &r);
	} 

	//FINALIZATION
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}
