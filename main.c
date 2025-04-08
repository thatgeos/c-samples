#include <SDL2/SDL.h>		//libreria grafica
#include "SDL2_inprint.h"	//libreria per avere un font senza usare ttf
#include "tinyexpr.h"		//libreria per valutare una funzione
#include "f.h"				//libreria che contiene le funzioni che disegnano piano e grafico
#include <stdlib.h>			
#include <math.h>



int main(int argc, char* argv[])
{
	SDL_Window* window;
	SDL_Renderer* renderer;

	Uint32 width = 640;
	Uint32 height = 480;
	Uint32 flags = 0;
	Uint32 max = 10;								//valore massimo di x
	double dx = 0.1;						    	//intervallo tra una x e l'altra
	SDL_Event e;									//evento di sdl. capta il segnale di chiusura dalla x
	char f[200];									//stringa che conterrá la funzione
	FILE *file;
	int i;
	int color = 0;										//gestisce il colore delle funzioni

	if (argv[1] != NULL && !(strcasecmp("-f", argv[1])) && argv[2] != NULL)
	{
		file = fopen(argv[2], "rt"); // la ricezione vera e propria del file va sotto
		
		if (file == NULL)
		{
			printf("File non trovato.\n\n");
			return 0;
		}
		
		printf("Aperto: %s\n\n", argv[2]);
	}
	else
	{
		printf("\nTRACCIA v0.3\n\nSintassi: ./traccia -f nomefile.txt\n\n  -w 640 (lunghezza della finestra)\n  -h 480 (altezza della finestra)\n  -m 10 (valore massimo di x)\n  -d 0.1 (intervallo tra due valori di x)\n\n NOTA: sono supportate piú funzioni nel file di testo.\n       Sono supportati gli operandi aritmetici, le parentesi\n       e le funzioni incluse in math.h\n\n");
		return 0;
	}

	


	for (i = 1; i < argc; i++)				//parametri da terminale
	{

		if (!strcasecmp("-w", argv[i]))			//larghezza
		{
			if (argv[i+1] != NULL && atoi(argv[i+1]) > 0)		//atoi converte una stringa in un int
			{
				width = atoi(argv[i+1]);
			}
		}
		
		if (!strcasecmp("-h", argv[i]))			//altezza
		{
			if (argv[i+1] != NULL && atoi(argv[i+1]) > 0)		
			{
				height = atoi(argv[i+1]);
			}
		}
		
		if (!strcasecmp("-m", argv[i]))			//x massima
		{
			if (argv[i+1] != NULL && atoi(argv[i+1]) > 0)		
			{
				max = atoi(argv[i+1]);
			}
		}

		if (!strcasecmp("-d", argv[i]))			//intervallo tra una x e l'altra
		{
			if (argv[i+1] != NULL && atof(argv[i+1]) > 0)		
			{
				dx = atof(argv[i+1]);
			}
		}

	}

	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		fprintf(stderr, "Errore inizializzazione video: %s\n", SDL_GetError());
		exit(-1);
	}
	if (SDL_CreateWindowAndRenderer(width, height, flags, &window, &renderer) < 0) {
		fprintf(stderr, "Errore inizializzazione renderer: %s\n", SDL_GetError());
		exit(-1);
	}

	/* For SDL2, "inrenderer" is the first function that must be called */
	inrenderer(renderer);
	prepare_inline_font();

	/* Clear screen */
	SDL_SetRenderDrawColor(renderer, 51, 51, 51, 255);
	SDL_RenderClear(renderer);

	//grid(renderer, width, height, max);

	while (!(SDL_PollEvent(&e) && e.type == SDL_QUIT))	//continua il display finché non premo x
	{
		while (fscanf(file, "%s", f) != EOF)
		{
			grid(renderer, width, height, max);
			plot(renderer, width, height, max, dx, f, color);
			color += 1;
		}

		SDL_RenderPresent(renderer);
		SDL_Delay(10);
	}

	/* Cleanup */
	kill_inline_font();
	SDL_Quit();
	return 0;
}
