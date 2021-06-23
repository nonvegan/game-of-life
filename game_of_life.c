#include <stdio.h>
#include <SDL2/SDL.h>
#include <time.h> 

#define WIDTH 800
#define HEIGHT 800
#define COLS 50
#define ROWS 50
#define FPS 10
#define DELAY_MS (1000/FPS)
#define START_PERCENT 20

void random_binary_fill_2d_array(int *arr,int m,int n, int p_fill)
{
    for (int i = 0; i < m; i++)
      for (int j = 0; j < n; j++)
	if(p_fill > rand() % 100) {
		*(arr + m * j + i) = 1;
	} else {
		*(arr + m * j + i) = 0;
	}
} 

void print_2d_array(int *arr,int m,int n)
{
	for (int i = 0; i < m; i++) {
		for (int j = 0; j < n; j++)
       	 		printf("%d ",*(arr + i*n + j));
		printf("\n");
	}
}

void draw_grid(SDL_Renderer *renderer, int *arr)
{	
	SDL_SetRenderDrawColor(renderer,249, 38, 114,255);

	/* for (int i = 1; i < COLS; i++) 
		SDL_RenderDrawLine(renderer,i * WIDTH/COLS,0,i * WIDTH/COLS,HEIGHT);

	for(int j = 1; j < ROWS; j++)
		SDL_RenderDrawLine(renderer,0, j * HEIGHT/ROWS, WIDTH, j * HEIGHT/ROWS); */

	for (int i = 0; i < COLS; i++)
		for (int j = 0;j < ROWS; j++) 
			if(*(arr + COLS * j + i)) {
				SDL_Rect rect = {i*WIDTH/COLS,j*HEIGHT/ROWS,WIDTH/COLS,HEIGHT/ROWS};
				SDL_RenderFillRect(renderer,&rect);
			}
}

int count_neighbour_cells(int * grid, int x, int y)
{	
	int neighbour_count =- *(grid + COLS * y + x);
	for(int a = -1; a <= 1; a++)
		for(int b = -1; b <= 1; b++) 
			neighbour_count += *(grid + COLS * ((y+b+ROWS)%ROWS) + ((x+a+COLS)%COLS)); 

	return neighbour_count;				
}

void update_grid(int *grid, int *buffer)
{
	memcpy(buffer,grid,sizeof (int) * COLS * ROWS);
	for (int i = 0; i < COLS; i++)
		for (int j = 0; j < ROWS; j++) {
			int neighbour_cells_count = count_neighbour_cells(buffer,i,j);
			if(!(*(buffer + COLS * j + i))) {
				if(neighbour_cells_count == 3)
					*(grid + COLS * j + i) = 1;	
			} else {
				if(neighbour_cells_count == 2 || neighbour_cells_count == 3) {
					*(grid + COLS * j + i) = 1;
				} else {
					*(grid + COLS * j + i) = 0;
				}
			}
		}
}	

int main (int argc, char* argv[])
{
	if(SDL_Init(SDL_INIT_VIDEO) != 0) {
		fprintf(stderr, "Unable to initialize SDL: %s\n", SDL_GetError());
		return 1;
	}

	SDL_Window *window = SDL_CreateWindow("Conway's Game of Life",
					      SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,
					      WIDTH,HEIGHT,SDL_WINDOW_SHOWN);

	if(window == NULL) {
	        fprintf(stderr, "SDL_CreateWindow Error: %s\n", SDL_GetError());
		return EXIT_FAILURE;
	}

	SDL_Renderer *renderer = SDL_CreateRenderer(window,-1,SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

	if (renderer == NULL) {
		SDL_DestroyWindow(window);
		fprintf(stderr, "SDL_CreateRenderer Error: %s\n", SDL_GetError());
		return EXIT_FAILURE;
    	}
	
	int grid[WIDTH][HEIGHT];
	int grid_buffer[WIDTH][HEIGHT];
	srand(time(NULL));
	random_binary_fill_2d_array((int *)grid,COLS,ROWS,START_PERCENT);	

	int t0 = (int)SDL_GetTicks() - DELAY_MS;
	int quit = 0;
	while(!quit) {
		SDL_Event e;
		while(SDL_PollEvent(&e)) {
			if (e.type == SDL_QUIT)
				quit = 1;
			if (e.type == SDL_MOUSEBUTTONDOWN)
				random_binary_fill_2d_array((int *)grid,COLS,ROWS,START_PERCENT);
		}	
		if((SDL_GetTicks() - t0) >= DELAY_MS) {
			SDL_SetRenderDrawColor(renderer,40, 44, 52,255);
			SDL_RenderClear(renderer);
			draw_grid(renderer,(int *)grid);	
			update_grid((int *)grid, (int *)grid_buffer);
			SDL_RenderPresent(renderer);
			t0 = SDL_GetTicks();
		}
	}

	SDL_DestroyWindow(window);	
	SDL_DestroyRenderer(renderer);
	SDL_Quit();

	return EXIT_SUCCESS;
}
