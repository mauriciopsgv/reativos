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
	struct color c;
} Square;

void creating_enemys (Square* enemy, Color* colors){
	float theta = rand()%360;
	int j = rand()%3;
	enemy->x = rand()% (641-colors[j].width);
	enemy->y = rand()% (481-colors[j].length);
	enemy->direction[0] = (int) sin(theta)*100; 
	enemy->direction[1] = (int) cos(theta)*100;
	enemy->c = colors[j];
}

void update_enemys_x (Square* enemy){
	enemy->x += ( enemy->c.speed * enemy->direction[0] * (now - old))/100 ;
	return;	
}

void update_enemys_y (Square* enemy){
	enemy->y += (enemy->c.speed * enemy->direction[1] * (now - old))/100;
	return;	
}

int main(int argc, char* args[]){

	//INICIALIZATION
	srand(time(NULL));
	SDL_Init(SDL_INIT_EVERYTHING);
	SDL_Window* window = SDL_CreateWindow("Squares", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640,480, SDL_WINDOW_SHOWN);
	SDL_Renderer* renderer = SDL_CreateRenderer(window, 1, 0);
	SDL_Event e;
	SDL_rect r;
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

	Color colors[3] ={red, green, blue};


	//Declaration of the hero
	Square hero;
	hero.x = 320; 
	hero.y = 240; 
	hero.c = &green; // as green is the all-around color, it's set as the default color

	//Declaration of enemys
	Square enemy1, enemy2, enemy3, enemy4, enemy5;
	Square enemys[5]  = {enemy1, enemy2, enemy3, enemy4, enemy5};
	for(i=0; i<5; i++){
		creating_enemys( &enemys[i], colors);
	}
	


	//EXECUTION
	while(1){

		now = SDL_GetTicks();

		if(SDL_PoolEvent(&e) == 1){
			if(e.type == SDL_Quit)
				break;
		}

	for(i=0; i<5; i++){
		update_enemys_x(&enemys[i]);
		update_enemys_y(&enemys[i]);
	}


	//RENDERIZATION
		SDL_SetRenderDrawColor(renderer, 0x00,0x00,0x00,0x00);
		SDL_RenderFillRect(renderer, NULL);

		for(i=0; i<5; i++){
			SDL_SetRenderDrawColor(renderer, enemys[i].c.R,enemys[i].c.G,enemys[i].c.B,0x00);
			r = {enemys[i].x, enemys[i].y, enemys[i].width, enemys[i].length};
			SDL_RenderFillRect(renderer, r);
		}

	}

	//FINALIZATION
		SDL_DestroyRenderer(renderer);
		SDL_DestroyWindow(window);
		SDL_Quit();

		return 0;
}

/*	if(enemy->x + enemy->c->width + enemy->c->speed >= 640){
		enemy-> x = 640 - enemy->c->width;
		enemy-> c->speed = enemy-> c->speed * (-1);
		return;
	}

	if(enemy->x + enemy->c->speed <= 0){
		enemy-> x = 0;
		enemy-> c->speed = enemy-> c->speed * (-1);
		return; 
	}

	if(enemy-> y + enemy->c->length + enemy->c->speed >= 480){
		enemy-> y = 480 - enemy->c->length;
		enemy-> c->speed = enemy-> c->speed * (-1);
		return;
	}

	if(enemy->y + enemy->c->speed <= 0){
		enemy-> y = 0;
		enemy-> c->speed = enemy-> c->speed * (-1);
		return;
	}

*/
