#include "SDL.h"
#include <complex.h>
#include <stdbool.h>

/* The maximum iteration count. */
const int max_it = 170;

/* Compute the out-coloring based on the iteration counter. */
Uint32 outcolor(int it) {
  // return 0x00010001 * ((it * 0xff) / max_it);
  return 0x00ffffff; /* white */
}

/* "Compute" the in-coloring. */
Uint32 incolor() {
  return 0x00ff00000; /* red */
}

void drawmandelbrot(SDL_Surface *surface) {
  if (SDL_MUSTLOCK(surface)) {
    SDL_LockSurface(surface);
  }
  Uint32 *pixels = (Uint32 *) surface->pixels;

  for (int i = 0; i < surface->w * surface->h; i++) {
    int y = i / surface->w;
    int x = i % surface->w;

    float complex c = ((3.0 * x / surface->w) - 2.0)
        + I * ((2.0 * y / surface->h) - 1.0);

    bool diverges = false;
    float complex z = 0;
    int it;
    for (it = 1; it <= max_it; it++) {
      /* z = z² + c */
      z = cpowf(z, 2) + c;

      /* If |z| ever gets greater than 2, it diverges. */
      if (cabs(z) > 2) {
        diverges = true;
        break;
      }
    }

    Uint32 color;
    if (diverges) {
      color = outcolor(it);
    } else {
      color = incolor();
    }

    pixels[i] = color;

    /* Update the screen every 10 lines. */
    if (y % 10 == 0 && x == 0) {
      SDL_Flip(surface);
    }
  }

  /* Update the screen a final time. */
  SDL_Flip(surface);
  if (SDL_MUSTLOCK(surface)) {
    SDL_UnlockSurface(surface);
  }
}

int main(int argc, char *argv[]) {
  /* Set up SDL. */
  if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
    fprintf(stderr, "Error: Could not initialize SDL: %s.\n", SDL_GetError());
    exit(1);
  }
  SDL_Surface *screen = SDL_SetVideoMode(1024, 768, 32, SDL_SWSURFACE);

  /* Do the mandelbrot. */
  drawmandelbrot(screen);

  /* Save BMP */
  char *file = "mandelbrot.bmp";
  if (SDL_SaveBMP(screen, file) != 0) {
    fprintf(stderr, "Could not write %s!\n", file);
  }

  /* Quit. */
  SDL_Delay(20000);
  SDL_Quit();
}
