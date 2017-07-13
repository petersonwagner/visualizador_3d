#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include "objread.h"
#include "commline.h"
#include "perspect.h"

void init(SDL_Window **Window, SDL_Renderer **Renderer);
void close(SDL_Window *Window, SDL_Renderer *Renderer);
void render_lines (SDL_Renderer *Renderer, coord3_t camera, vlist_t *vertices, coord2_t **draw_points, linelist_t *lines);

int main(int argc, char* argv[])
{
	FILE *stream = NULL;
	vlist_t vertices;
	linelist_t lines;
	coord3_t camera;
	coord2_t *draw_points = NULL;
	bool quit = false;
	bool mouseButtonHold = false;
	SDL_Event Event;
	SDL_Window* Window = NULL;
	SDL_Renderer* Renderer = NULL;

	//valores iniciais da camera
	camera.x = 10;
	camera.y = 10;
	camera.z = 50;

	//obtem stream do argv
	stream = get_stream (argv);

	//leitura do arquivo obj
	readobj (stream, &vertices, &lines);

	//alocação do vetor de pontos de desenho
	draw_points = (coord2_t*) malloc (sizeof(coord2_t) * vertices.size);

	//inicialização do SDL
	init (&Window, &Renderer);

	//desenho inicial dos pontos na tela
	render_lines (Renderer, camera, &vertices, &draw_points, &lines);

	//While application is running
	while (!quit)
	{
		//Handle events on queue
		while (SDL_PollEvent (&Event) != 0)
		{
			//se usuario sair
			if (Event.type == SDL_QUIT)
				quit = true;

			//se usuario pressionou uma tecla
			else if(Event.type == SDL_KEYDOWN)
			{
				switch (Event.key.keysym.sym)
				{
					case SDLK_q:
						quit = true;
						break;

					case SDLK_UP:
					    camera.y++;
						break;

					case SDLK_DOWN:
					    camera.y--;
						break;

					case SDLK_LEFT:
					    camera.x--;
						break;

					case SDLK_RIGHT:
					    camera.x++;
						break;
				}

				//desenho dos pontos na tela
				render_lines (Renderer, camera, &vertices, &draw_points, &lines);
			}

			//se usuario pressionou botao do mouse, atualizar o estado da variavel mouseButtonHold
			else if (Event.type == SDL_MOUSEBUTTONDOWN)
				mouseButtonHold = true;
			else if (Event.type == SDL_MOUSEBUTTONUP)
				mouseButtonHold = false;


			//se usuario esta segurando o botao e moveu o mouse
			else if (mouseButtonHold && Event.type == SDL_MOUSEMOTION)
			{
				//se mouse moveu no eixo x
				if (Event.motion.xrel > 0)
					camera.x++;
				else if (Event.motion.xrel < 0)
					camera.x--;

				//se mouse moveu no eixo y
				if (Event.motion.yrel > 0)
					camera.y--;
				else if (Event.motion.yrel < 0)
					camera.y++;

				//desenho dos pontos na tela
				render_lines (Renderer, camera, &vertices, &draw_points, &lines);
			}
		}
	}

	//Free resources and close SDL
	close(Window, Renderer);

	return 0;
}


void init(SDL_Window **Window, SDL_Renderer **Renderer)
{
	//Initialize SDL
	if (SDL_Init (SDL_INIT_VIDEO) < 0)
	{
		printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
		abort();
	}

	//Create window
	*Window = SDL_CreateWindow ("Wireframe", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	if (*Window == NULL)
	{
		printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
		abort();
	}

	//Create renderer for window
	*Renderer = SDL_CreateRenderer (*Window, -1, SDL_RENDERER_ACCELERATED);
	if (*Renderer == NULL)
	{
		printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
		abort();
	}

	//Initialize renderer color
	SDL_SetRenderDrawColor (*Renderer, 0, 0, 0, 0xFF);
}

//Frees media and shuts down SDL
void close(SDL_Window *Window, SDL_Renderer *Renderer)
{
	//Destroy window
	SDL_DestroyRenderer (Renderer);
	SDL_DestroyWindow (Window);

	//Quit SDL subsystems
	SDL_Quit();
}

void render_lines (SDL_Renderer *Renderer, coord3_t camera, vlist_t *vertices, coord2_t **draw_points, linelist_t *lines)
{
	int i;

	SDL_SetRenderDrawColor (Renderer, 0, 0, 0, 0xFF);

	//Clear screen
	SDL_RenderClear (Renderer);

	//calculo do pontos para desenhar na tela
	weak_perspective (camera, vertices, draw_points);

	//desenho dos pontos
	for (i = 0; i < lines->size; ++i)
	{
		#define index1 lines->array[i].v1 - 1
		#define index2 lines->array[i].v2 - 1

		lineRGBA (Renderer,
			(*draw_points) [ index1 ].x,
			(*draw_points) [ index1 ].y,
			(*draw_points) [ index2 ].x,
			(*draw_points) [ index2 ].y,
			0xFF, 0xFF, 0xFF, 0xFF);

		#undef index1
		#undef index2	
	}

	//Update screen
	SDL_RenderPresent (Renderer);
}