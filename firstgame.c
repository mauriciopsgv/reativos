#include <SDL2/SDL.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#define B_SIZE 50
#define B_SPEED 1
#define SPEED_BLUE
#define R_SIZE 20
#define R_SPEED 3
#define G_SIZE 35
#define G_SPEED 2

unsigned int now;
unsigned int old = 0;

typedef struct color{
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
	int direction[2];
	Color* c;
} Square;

void creating_enemies (Square* enemie, Color* colors){
	float theta = rand()%360;
	int j = rand()%3;
	enemie->x = rand()% (641-color->width);
	enemie->y = rand()% (481-color->length);
	enemie->direction = {(int*) sin(theta)*100, (int*) cos(theta)*100}; 
	enemie->c = colors[j];
}

void update_enemies_x (Square* enemie){
	enemie->x += ( enemie->c->speed * enemie->direction[0] * (now - old))/100 ;
	return;	
}

void update_enemies_y (Square* enemie){
	enemie->y += (enemie->c->speed * enemie->direction[1] * (now - old))/100;
	return;	
}

int main(int argc, char* args[]){

	//INICIALIZATION
	setSeed(time(NULL));
	SDL_Init(SDL_INIT_EVERYTHING);
	SDL_Window* window = SDL_CreateWindow("Squares", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640,480, SDL_WINDOW_SHOWN);
	SDL_Renderer* renderer = SDL_CreateRenderer(window, 1, 0);
	SDL_Event e;
	int i;

	//Declaration of Colors
	Color blue;
	blue.width = B_SIZE;
	blue.length = B_SIZE;
	blue.speed = B_SPEED;
	blue.R = 0x00;
	blue.G = 0x00;
	blue.B = 0xFF;

	Color red;
	red.width = R_SIZE;
	red.length = R_SIZE;
	red.speed = R_SPEED;
	red.R = 0xFF;
	red.G = 0x00;
	red.B = 0x00;

	Color green;
	green.width = G_SIZE;
	green.length = G_SIZE;
	green.speed = G_SPEED;
	red.R = 0x00;
	red.G = 0xFF;
	red.B = 0x00;

	Color colors[3] ={ red, green, blue};


	//Declaration of the hero
	Square hero;
	hero.x = 320; 
	hero.y = 240; 
	hero.c = &green; // as green is the all-around color, it's set as the default color

	//Declaration of Enemies
	Square enemie1, enemie2, enemie3, enemie4, enemie5;
	Square enemies[5]  = {enemie1, enemie2, enemie3, enemie4, enemie5};
	for(i=0; i<5; i++){
		creating_enemies( &enemies[i], colors);
	}
	


	//EXECUTION
	while(1){

		now = SDL_GetTicks();

		if(SDL_PoolEvent(&e) == 1){
			if(e.type == SDL_Quit)
				break;
		}

		update_enemies_x(&enemie1);
		update_enemies_y(&enemie1);


	//RENDERIZATION
		SDL_SetRenderDrawColor(renderer, 0x00,0x00,0x00,0x00);
		SDL_RenderFillRect(renderer, NULL);

		for(i=0; i<5; i++){
			SDL_SetRenderDrawColor(renderer, enemies[i].c.R,enemies[i].c.G,enemies[i].c.B,0x00);
			SDL_RenderFillRect(renderer, {enemies[i].x, enemies[i].y, enemies[i].width, enemies[i].length});
		}

	}

	//FINALIZATION
		SDL_DestroyRenderer(renderer);
		SDL_DestroyWindow(window);
		SDL_Quit();

		return 0;
}

/*	if(enemie->x + enemie->c->width + enemie->c->speed >= 640){
		enemie-> x = 640 - enemie->c->width;
		enemie-> c->speed = enemie-> c->speed * (-1);
		return;
	}

	if(enemie->x + enemie->c->speed <= 0){
		enemie-> x = 0;
		enemie-> c->speed = enemie-> c->speed * (-1);
		return; 
	}

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

*/
