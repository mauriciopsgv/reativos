#include <SDL2/SDL.h>
#define DELAY 500

int vx = { 50, 0, -50, 0};
int vy = { 0, 50, 0, -50};

void MoveSquare(SDL_Rect* r, int command){

	r->x += vx[command];
	r->y += vy[command];

}

int main(int argc, char* args[])
{
	// Initialization
	int command, move;
	SDL_Init(SDL_INIT_EVERYTHING);
	SDL_Window* window =SDL_CreateWindow("Hello World!", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480, SDL_WINDOW_SHOW);
	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);
	SDL_Rect r1 ={100,50,50,50};
	SDL_Rect r2 ={500,300,50,50};
	SDL_Event e;

	

	old = SDL_GetTicks();

	SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0x00);
	SDL_RenderFillRect(renderer, NULL);
	SDL_RenderDrawColor(renderer, 0xFF, 0x00, 0x00, 0x00);
	SDL_RenderFillRect(renderer, &r1);
	SDL_RenderPresent(renderer);



	// Execution
	while(1){
		

		if(SDL_GetTicks() > old + DELAY){

			MoveSquare(r1,command);

			SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0x00);
			SDL_RenderFillRect(renderer, NULL);

			SDL_RenderDrawColor(renderer, 0xFF, 0x00, 0x00, 0x00);
			SDL_RenderFillRect(renderer, &r1);

			SDL_RenderPresent(renderer);
			old = SDL_GetTicks();

			move++;
			if(move%2 == 0){
				command++;
			}
		}
	}
}
