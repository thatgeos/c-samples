#include <SDL2/SDL.h>
#include "SDL2_inprint.h"
#include "tinyexpr.h"
#include <stdlib.h>
#include <math.h>

void grid(SDL_Renderer* r, int w, int h, int m)
{
	int cx = w/2;				//coordinate in pixel del centro 0,0
	int cy = h/2;
	char txt[4];
	int k = -m + 1;

	double part = (double)w / (double)(2*m); //lunghezza in pixel di ogni unitá del grafico
	
	SDL_SetRenderDrawColor(r, 255, 255, 0, 255);

	for (double i = part; i < w; i += part)
	{
		sprintf(txt, "%d", k);
		k += 1;
		inprint(r, txt, i, cy);
		SDL_RenderDrawLine(r, i, 0, i, h);
		
	}
	k = 0;
	for (double i = cy,j = cy; i < h; i += part, j -= part)
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

void plot(SDL_Renderer* r, int w, int h, int m, double dx, char f[])
{
	
	double part = (double)w / (double)(2*m);
	double y[w];								// creo due array per immagazzinare tutti i valori x e y
	double x[w];
	double xvar;								// mi serve per la libreria di valutazione
	te_variable var[] = {{"x", &xvar}};				// si aspetta piú di una variabile
	int err;														// per immagazzinare eventuali errori
	te_expr *expr = te_compile(f, var, 1, &err);		// compilazione della funzione
	

	double i = -m;

	SDL_SetRenderDrawColor(r, 255, 255, 255, 255);
	
	for (int n = 0; i < m && n < w; n += 1)
	{
		x[n] = i;
		xvar = x[n];
		y[n] = te_eval(expr);		//questa é la funzione vera e propria.


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