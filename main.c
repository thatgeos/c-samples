#include <SDL2/SDL.h>
#include "SDL2_inprint.h"
#include <stdlib.h>
#include <math.h>

void grid(SDL_Renderer* r, int w, int h, int m)
{
	int cx = w/2;				//coordinate in pixel del centro 0,0
	int cy = h/2;
	char txt[4];
	int k = -m + 1;

	float part = (float)w / (float)(2*m); //lunghezza in pixel di ogni unitá del grafico
	
	SDL_SetRenderDrawColor(r, 255, 255, 0, 255);

	for (float i = part; i < w; i += part)
	{
		sprintf(txt, "%d", k);
		k += 1;
		inprint(r, txt, i, cy);
		SDL_RenderDrawLine(r, i, 0, i, h);
		
	}
	k = 0;
	for (float i = cy,j = cy; i < h; i += part, j -= part)
	{
		sprintf(txt, "%d", -k);			//queste 4 righe generano i numeri al bordo di ogni unitá.
		inprint(r, txt, cx, i);			//sprintf trasferisce una variabile in una stringa, txt, 
		sprintf(txt, "%d", k);			//perché la libreria inprint si prende solo stringhe.
		inprint(r, txt, cx, j);
		k += 1;
		SDL_RenderDrawLine(r, 0, i, w, i);
		SDL_RenderDrawLine(r, 0, j, w, j);

	}

	SDL_SetRenderDrawColor(r, 255, 0, 0, 255);
	SDL_RenderDrawLine(r, cx, 0, cx, h);
	SDL_RenderDrawLine(r, 0, cy, w, cy);
}

void plot(SDL_Renderer* r, int w, int h, int m, float dx)
{
	float part = (float)w / (float)(2*m);
	float y[w];								// creo due array per immagazzinare tutti i valori x e y
	float x[w];

	float i = -m;
	SDL_SetRenderDrawColor(r, 255, 255, 255, 255);
	for (int n = 0; i < m; n += 1)
	{
		x[n] = i;

		y[n] = sqrt(1 - x[n]*x[n]);		//questa é la funzione vera e propria.

		i += dx;
		if (n > 0 && y[n]*y[n-1] > 0)			// n > o perché altrimenti n-1 risulta negativo e quindi x[n-1] dá segmentation fault
												// il prodotto é la legge della permanenza del segno.
												// mi serve per disegnare le funzioni con gli asintoti:
												// se i miei due punti sono uno positivo e uno negativo
												// allora o ho uno zero o ho un asintoto. se ho uno zero
												// non lo disegno lo stesso perché tanto non si vede
												// se ho un asintoto non lo devo proprio disegnare
		{
			SDL_RenderDrawLine(r, x[n-1]*part + w/2, -y[n-1]*part + h/2, x[n]*part + w/2, -y[n]*part + h/2);
			//allora, ascoltami bene piccolo batterio.
			//le coordinate di base per ogni linea sarebbero x[n-1], y[n-1] e x[n], y[n].
			//aggiungo w/2 e h/2 per spostare il centro dallo 0,0 di SDL al centro della finestra
			//moltiplico per part perché ogni unitá del mio grafico é esattamente part volte
			//quella del piano di SDL
		}
	}
}

int main(int argc, char* argv[])
{
	SDL_Window* window;
	SDL_Renderer* renderer;

	Uint32 width = 640;
	Uint32 height = 480;
	Uint32 flags = 0;
	Uint32 max = 10;						//valore massimo di x
	float dx = 0.1;							//intervallo tra una x e l'altra
	SDL_Event e;
	int i;
	

	for (i = 1; i < argc; i++)				//parametri da terminale
	{
		if (!strcasecmp("-?", argv[i]))			//intervallo tra una x e l'altra
		{
			printf("\nTRACCIA v0.2\n\n  -w 640 (lunghezza della finestra)\n  -h 480 (altezza della finestra)\n  -f (schermo intero) !!NON FUNZIONA CORRETTAMENTE!!\n  -m 10 (valore massimo di x)\n  -d 0.1 (intervallo tra due valori di x)\n\n");
			return 0;
		}

		if (!strcasecmp("-f", argv[i]))			//fullscreen
		{
			flags |= SDL_WINDOW_FULLSCREEN;
		}
		else					//else perché non puoi usare contemporaneamente -f e -w -h
		{
			if (!strcasecmp("-w", argv[i]))			//larghezza
			{
				if (atoi(argv[i+1]) > 0)		//atoi converte una stringa in un int
				{
					width = atoi(argv[i+1]);
				}
			}
			
			if (!strcasecmp("-h", argv[i]))			//altezza
			{
				if (atoi(argv[i+1]) > 0)		
				{
					height = atoi(argv[i+1]);
				}
			}
		}
		if (!strcasecmp("-m", argv[i]))			//x massima
		{
			if (atoi(argv[i+1]) > 0)		
			{
				max = atoi(argv[i+1]);
			}
		}

		if (!strcasecmp("-d", argv[i]))			//intervallo tra una x e l'altra
		{
			if (atof(argv[i+1]) > 0)		
			{
				dx = atof(argv[i+1]);
			}
		}

	}

	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		fprintf(stderr, "Video initialization failed: %s\n", SDL_GetError());
		exit(-1);
	}

	if (SDL_CreateWindowAndRenderer(width, height, flags, &window, &renderer) < 0) {
		fprintf(stderr, "Window/Renderer initialization failed: %s\n", SDL_GetError());
		exit(-1);
	}

	/* For SDL2, "inrenderer" is the first function that must be called */
	inrenderer(renderer);
	prepare_inline_font();

	/* Clear screen */
	SDL_SetRenderDrawColor(renderer, 51, 51, 51, 255);
	SDL_RenderClear(renderer);

	grid(renderer, width, height, max);
	plot(renderer, width, height, max, dx);

	while (!(SDL_PollEvent(&e) && e.type == SDL_QUIT))	//continua il display finché non premo x
	{
		if(e.type == SDL_WINDOWEVENT_FOCUS_LOST || e.type == SDL_WINDOWEVENT_FOCUS_GAINED)
		{
			grid(renderer, width, height, max);				//questa if sistema il bug che rende nera la finestra se perde il focus
			plot(renderer, width, height, max, dx);
		}


		SDL_RenderPresent(renderer);
		SDL_Delay(10);
	}

	/* Cleanup */
	kill_inline_font();
	SDL_Quit();
	return 0;
}