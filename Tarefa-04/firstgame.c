#include <SDL2/SDL.h>
#include <stdlib.h>
#include <time.h>
#define HERO_W 30
#define HERO_L 30
#define WALL_W 80
#define WALL_L 480
#define SCREEN_X 640
#define SCREEN_Y 480
#define GRAVITY_DT 30
#define GRAVITY_ACC 4
#define MAX_VEL 20
#define SIDEWAYS_DT 4
#define SPIKE_BASE 30
#define SPACE 80
#define MAX_SPIKES 14

int cont_colors=1, side=1, new_spike=0, change_spike=0, n_spikes=0;
unsigned long now, old_s, old_g;

typedef struct color{
	int R;
	int G;
	int B;
}	Color;

typedef struct spike{
	int x[3];
	int y[3];
}	Spike;

typedef struct wall{
	int x[2];
	int y[2];
	Color* c;
}	Wall;


typedef struct square{
	float x;
	float y;
	float vel;
	int w;
	int h;
	Color* c;
}	Square;

//Gravity
void gravity (Square* hero){
	unsigned long dt = now - old_g;

	if(dt >= GRAVITY_DT){
		
		hero->vel += (GRAVITY_ACC*dt)/100;
		if(hero->vel > MAX_VEL)
			hero->vel = MAX_VEL;

		hero->y += hero->vel*dt/100;

		old_g = SDL_GetTicks();
	}
}


int check_ground (Square* hero){
	if(hero->y + HERO_L >= SCREEN_Y)
		return 1;
	return 0;

}

int check_roof (Square* hero){
	if(hero->y <= 0)
		return 1;
	return 0;

}

void change_wall_color ( Wall *w, Color * colors)
{
	int color_index = rand()%3;
	w->c = &colors[color_index];
}

void change_color (Square * hero, Color * colors){
	cont_colors++;
	hero->c = &colors[cont_colors%3];
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
					new_spike++;
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
					change_spike++;
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

void create_spike (Spike** v, int n){

	if(n == 14)
		return;

	v[n] = (Spike*) malloc(sizeof(Spike));
	n_spikes++;

	if(n == 0){

		v[n]->x[0] = WALL_W;
		v[n]->y[0] = rand()%(SCREEN_Y - SPIKE_BASE + 1);

		v[n]->x[1] = WALL_W;
		v[n]->y[1] = v[n]->y[0] + SPIKE_BASE;

		v[n]->x[2] = WALL_W + 22; // o numero 22 é baseado em uma aproximacao inteira para a altura de um triangulo equilatero de lado 30
		v[n]->y[2] = (v[n]->y[0] + v[n]->y[1])/2;
	}
	else
	{
		v[n]->x[0] = SCREEN_X - WALL_W;
		v[n]->y[0] = v[n-1]->y[1];
			
		v[n]->x[1] = SCREEN_X - WALL_W;
		v[n]->y[1] = v[n]->y[0] + SPIKE_BASE;
			
		v[n]->x[2] = SCREEN_X - WALL_W - 22;
		v[n]->y[2] = (v[n]->y[0] + v[n]->y[1])/2;
		
	}
}

void change_spike_to_right (Spike** v, int n){
	int i;	

	v[0]->x[0] = SCREEN_X - WALL_W;
	v[0]->y[0] = rand()%(SCREEN_Y - SPIKE_BASE - (n+1)*(SPIKE_BASE) - SPACE + 1);

	v[0]->x[1] = SCREEN_X - WALL_W;
	v[0]->y[1] = v[0]->y[0] + SPIKE_BASE;

	v[0]->x[2] = SCREEN_X -WALL_W - 22;
	v[0]->y[2] = (v[0]->y[0] + v[0]->y[1])/2;

	if(n > 0){
		for(i=1; i<n+1; i++)
		{
			v[i]->x[0] = SCREEN_X - WALL_W;
			v[i]->y[0] = v[i-1]->y[1];
			
			v[i]->x[1] = SCREEN_X - WALL_W;
			v[i]->y[1] = v[i]->y[0] + SPIKE_BASE;
			
			v[i]->x[2] = SCREEN_X - WALL_W - 22;
			v[i]->y[2] = (v[i]->y[0] + v[i]->y[1])/2;
		}
	}
}

void change_spike_to_left (Spike** v, int n){
	int i;	

	v[0]->x[0] = WALL_W;
	v[0]->y[0] = rand()%(SCREEN_Y - SPIKE_BASE - (n)*(SPIKE_BASE) - SPACE + 1);

	v[0]->x[1] = WALL_W;
	v[0]->y[1] = v[0]->y[0] + SPIKE_BASE;

	v[0]->x[2] = WALL_W + 22;
	v[0]->y[2] = (v[0]->y[0] + v[0]->y[1])/2;

	if(n > 0){
		for(i=1; i<n; i++)
		{
			v[i]->x[0] = WALL_W;
			v[i]->y[0] = v[i-1]->y[1];
			
			v[i]->x[1] = WALL_W;
			v[i]->y[1] = v[i]->y[0] + SPIKE_BASE;
			
			v[i]->x[2] = WALL_W + 22;
			v[i]->y[2] = (v[i]->y[0] + v[i]->y[1])/2;
		}
	}
}

void add_space (Spike** v, int n){
	int i;
	int space_index = rand()%n;

	for(i=space_index+1; i<n; i++){
		v[i]->y[0] += SPACE;
		v[i]->y[1] += SPACE;
		v[i]->y[2] += SPACE;
	}
}

int front_collision (Square * hero, Square * rec){

	return side*(rec->x - (hero->x + HERO_W)) <= 0;
}

int square_collision ( Square* hero , Square* rec, int up_down ){
	int xm_h, ym_h, xm_r, ym_r;
	xm_h = hero->x + HERO_W/2;
	ym_h = hero->y + HERO_L/2;
	xm_r = rec->x + ((rec->w)/2);
	ym_r = rec->y + ((rec->h)/2);

	return (side*(xm_r - xm_h) < ((HERO_W+rec->w)/2) && up_down*(ym_r - ym_h) < ((HERO_L+rec->h)));


}

int spike_collision(Square * hero, Spike ** v, int n, int i){
	int h;
	float dx;
	Square aux = { 0, 0, 0, 0, 0, NULL};

	if(side == -1){
			// TIPO 2
		if( hero->y > v[i]->y[2]){
			dx = hero->x - v[i]->x[1];
			if(dx > 22)
				return 0;

			h = (int) 15*(dx)/22;
			aux.x = WALL_W;
			aux.y = v[i]->y[2];
			aux.w = 22;
			aux.h = 15- h;

			if(h == 0)
				return front_collision(hero, &aux);
			return square_collision(hero, &aux,-1);
		}
			// TIPO 1
		else{
			dx = hero->x - v[i]->x[1];
			if(dx > 22)
				return 0;
				
			h = (int) 15*(dx)/22;
			aux.x = WALL_W;
			aux.y = v[i]->y[2] - 15 + h;
			aux.w = 22;
			aux.h = 15 - h;

			if(h == 0)
				return front_collision(hero, &aux);
			return square_collision(hero, &aux,1);
		}
	}
	else
	{
		
		//TIPO 4
		if( hero->y > v[i]->y[2]){
			dx = v[i]->x[1] - hero->x;
			if(dx > 22)
				return 0;
			h = (int) 15*(dx)/22;
			aux.x = v[i]->x[2];
			aux.y = v[i]->y[2];
			aux.w = 22;
			aux.h = 15 - h;
			if(h == 0)
				return front_collision(hero, &aux);
			return square_collision(hero, &aux,-1);
		}
		else
		{
			//TIPO 3
			dx = v[i]->x[1] - hero->x;
			if(dx > 22)
				return 0;
			h = (int) 15*(dx)/22;
			aux.x = v[i]->x[2];
			aux.y = v[i]->y[2] - 15 + h;
			aux.w = 22;
			aux.h = h - 15;

			if(h == 0)
				return front_collision(hero, &aux);
			return square_collision(hero, &aux,1);

		}
			
	}

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
	SDL_Window* window = SDL_CreateWindow("Evil Spikes", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_X, SCREEN_Y, SDL_WINDOW_SHOWN);
	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);
	SDL_Event e;
	SDL_Rect draw_hero, draw_wall;
	int i, j, k, l, get_out_g = 0, get_out_w=0, get_out_s=0, start=0, changed=0;
	Spike* v[14];

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
	Square hero = {300, 220, 0, HERO_W, HERO_L, &green};
	draw_hero.w = HERO_W;
	draw_hero.h = HERO_L;

	//The walls are built
	Wall w = {{0, SCREEN_X - WALL_W}, {0,0}, &green};
	draw_wall.w = WALL_W;
	draw_wall.h = WALL_L;

	//RENDERIZATION
		SDL_SetRenderDrawColor(renderer, 0x49,0x49,0x49,0x00);
		SDL_RenderFillRect(renderer, NULL);
	//Drawing the hero
		SDL_SetRenderDrawColor(renderer, hero.c->R, hero.c->G, hero.c->B, 0x00);
		draw_hero.x = (int) hero.x;
		draw_hero.y = (int) hero.y;
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

	while(start == 0){
		if(SDL_PollEvent(&e) == 1){
			if( e.type == SDL_KEYDOWN){
				switch (e.key.keysym.sym){
				case SDLK_UP:
					start = 1;
				}
			}
		}
	}
	old_g = SDL_GetTicks();
	old_s = old_g;

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
						hero.vel = -15;
						break;
					case SDLK_SPACE:
						change_color(&hero, colors);
						break;
					}
				}	
			}

		move_sideways(&hero);
		gravity(&hero);

		get_out_g = check_ground(&hero);
		get_out_g = check_roof(&hero);
		get_out_w = bouncing_walls(&hero, &w, colors);


		// Spikes Logic
		if(new_spike != 0){
			if(new_spike != n_spikes){
				create_spike( v, n_spikes);
				if(n_spikes > 1){
					change_spike_to_left(v, n_spikes);
					add_space(v, n_spikes);
				}
			}
			else if(change_spike != changed)
			{
				change_spike_to_right(v, n_spikes-1);
				add_space(v, n_spikes);
				changed++;
			}
			for(l=0; l<n_spikes; l++){
				get_out_s =spike_collision(&hero, v, n_spikes, l);
				if(get_out_s == 1)
					finalization_error(renderer, window);
			}
		}


		if(get_out_g || get_out_w){

			finalization_error(renderer,window);
		}


		//RENDERIZATION
		SDL_SetRenderDrawColor(renderer, 0x49,0x49,0x49,0x00);
		SDL_RenderFillRect(renderer, NULL);

		//Drawing the hero
		SDL_SetRenderDrawColor(renderer, hero.c->R, hero.c->G, hero.c->B, 0x00);
		draw_hero.x = (int) hero.x;
		draw_hero.y = (int) hero.y;
		SDL_RenderFillRect(renderer, &draw_hero);

		//Drawing the Walls
		SDL_SetRenderDrawColor(renderer, w.c->R, w.c->G, w.c->B, 0x00);
		for (i=0; i<2; i++)
		{
			draw_wall.x = w.x[i];
			draw_wall.y = w.y[i];
			SDL_RenderFillRect(renderer, &draw_wall);
		}

		//Drawing Spikes
		SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0x00, 0x00);
		for(j=0; j<n_spikes; j++){
			for(k=0; k<3; k++){
				SDL_RenderDrawLine(renderer, v[j]->x[k%3], v[j]->y[k%3], v[j]->x[(k+1)%3], v[j]->y[(k+1)%3]);
			}
		}


		SDL_RenderPresent(renderer);
	

		
	}
	//FINALIZATION
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();

	return 0;
}
