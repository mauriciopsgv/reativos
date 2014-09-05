#include <SDL2/SDL.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#define B_SIZE 50
#define B_SPEED 1
#define R_SIZE 20
#define R_SPEED 3
#define G_SIZE 35
#define G_SPEED 2
#define SCREEN_X 800
#define SCREEN_Y 600


unsigned long now;
unsigned long old = 0;
unsigned long dt;
int cont_cores = 4;
int n_enemies=0;

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
	int xm;
	int ym;
	int side_x;
	int side_y;
	float direction[2];
	struct color c;
} Square;

void creating_enemys (Square* enemy, Color* colors){
	float theta = rand()%90;
	int j = rand()%3;
	enemy->x = rand()% (SCREEN_X+1-colors[j].width);
	enemy->y = rand()% (SCREEN_Y+1-colors[j].length);
	enemy->direction[0] = (int) (cos(theta)*100); 
	enemy->direction[1] = (int) (sin(theta)*100);
	enemy->side_x = rand()%2;
	enemy->side_y = rand()%2;

	enemy->c = colors[j];
	enemy->xm = enemy->x + enemy->c.width/2;
	enemy->ym = enemy->y + enemy->c.length/2;
	n_enemies++;
}

void update_enemys_x (Square* enemy){
	if(enemy->side_x == 1)
		enemy->x += enemy->c.speed *(( (enemy->direction[0]/100) * (dt))/10);
	else
		enemy->x -= enemy->c.speed *(( (enemy->direction[0]/100) * (dt))/10);
	return;	
}

void update_enemys_y (Square* enemy){
	if(enemy->side_y == 1)
		enemy->y += enemy->c.speed *((  (enemy->direction[1]/100) * (dt))/10);
	else
		enemy->y -= enemy->c.speed *((  (enemy->direction[1]/100) * (dt))/10);
	return;	
}

void collision_with_walls (Square * enemy){
	if(enemy->x + enemy->c.width >= SCREEN_X){
		enemy->x = SCREEN_X - enemy->c.width;
		enemy->side_x = 0;
		return;
	}

	if(enemy->x <= 0){
		enemy->x = 0;
		enemy->side_x = 1;
		return; 
	}

	if(enemy-> y + enemy->c.length >= SCREEN_Y){
		enemy-> y = SCREEN_Y - enemy->c.length;
		enemy->side_y= 0;
		return;
	}

	if(enemy->y <= 0){
		enemy-> y = 0;
		enemy->side_y = 1;
		return;
	}
}

void collision_with_hero (Square * enemys, Square * hero){
	int k;
	Square aux;

	for(k=0; k<n_enemies; k++){
		if(hero->xm - enemy->xm < (hero->c.width - enemy->c.width)/2  && hero->ym - enemy->ym < (hero->c.length - enemy->c.length)/2 )
		{
			if(hero->c.speed == enemy->c.speed)
			{
				n_enemies--;
				aux = enemys[k];
				enemys[k] = enemys[n_enemies];
				enemys[n_enemies] = aux;
			}
			else
			{
				while(1);
			}
		}		
	}
}

void change_color (Square * hero, Color * colors){
	cont_cores++;
	
	hero->c = colors[cont_cores%3];

	hero->x = hero->xm - hero->c.width/2;
	hero->y = hero->ym - hero->c.length/2;
}

int main(int argc, char* args[]){

	//INICIALIZATION
	srand(time(NULL));
	SDL_Init(SDL_INIT_EVERYTHING);
	SDL_Window* window = SDL_CreateWindow("Squares", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_X,SCREEN_Y, SDL_WINDOW_SHOWN);
	SDL_Renderer* renderer = SDL_CreateRenderer(window, 1, 0);
	SDL_Event e;
	SDL_Rect r;
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
	green.R = 0x00;
	green.G = 0xFF;
	green.B = 0x00;

	Color colors[3] ={red, green, blue};


	//Declaration of the hero
	Square hero;
	hero.x = 320; 
	hero.y = 240; 
	hero.c = green; // as green is the all-around color, it's set as the default color
	hero->xm = hero->x + hero->c.width/2;
	hero->ym = hero->y + hero->c.length/2;

	//Declaration of enemys
	Square enemy1, enemy2, enemy3, enemy4, enemy5;
	Square enemys[5]  = {enemy1, enemy2, enemy3, enemy4, enemy5};
	for(i=0; i<5; i++){
		creating_enemys( &enemys[i], colors);
	}


	


	//EXECUTION
	while(1){

		now = SDL_GetTicks();
		dt = now - old;

		if(SDL_PollEvent(&e) == 1){
			if(e.type == SDL_QUIT)
				break;
			else if( e.type == SDL_KEYDOWN){
				switch (e.key.keysym.sym){
					case SDLK_UP:
						hero.y -= ((hero.c.speed * dt)/10);
						break;

					case SDLK_DOWN:
						hero.y += ((hero.c.speed * dt)/10);
						break;

					case SDLK_LEFT:
						hero.x -= ((hero.c.speed * dt)/10);
						break;

					case SDLK_RIGHT:
						hero.x += ((hero.c.speed * dt)/10);
						break;

					case SDLK_SPACE:
						change_color(&hero, colors);
						break;


				}
			}
			
		}

		for(i=0; i<n_enemies; i++){
			update_enemys_x(&enemys[i]);
			update_enemys_y(&enemys[i]);
		}
		old = SDL_GetTicks();

		for(i=0; i<5; i++){
			collision_with_walls(&enemys[i]);
		}
			collision_with_hero(enemys, &hero);

	//RENDERIZATION
		SDL_SetRenderDrawColor(renderer, 0x00,0x00,0x00,0x00);
		SDL_RenderFillRect(renderer, NULL);

		for(i=0; i<n_enemies; i++){
			SDL_SetRenderDrawColor(renderer, enemys[i].c.R,enemys[i].c.G,enemys[i].c.B,0x00);
			r.x=enemys[i].x;
			r.y=enemys[i].y;
			r.w=enemys[i].c.width;
			r.h=enemys[i].c.length;
			SDL_RenderFillRect(renderer, &r);
		}

		SDL_SetRenderDrawColor(renderer, hero.c.R, hero.c.G, hero.c.B, 0x00);
		r.x=hero.x;
		r.y=hero.y;
		r.w=hero.c.width;
		r.h=hero.c.length;
		SDL_RenderFillRect(renderer, &r);

		SDL_SetRenderDrawColor(renderer, hero.c.R, hero.c.G, hero.c.B, 0x00);
		r.x=hero.x+5;
		r.y=hero.y+5;
		r.w=hero.c.width-5;
		r.h=hero.c.length-5;
		SDL_RenderFillRect(renderer, &r);

		SDL_RenderPresent(renderer);
	}

	//FINALIZATION
		SDL_DestroyRenderer(renderer);
		SDL_DestroyWindow(window);
		SDL_Quit();

		return 0;
}
