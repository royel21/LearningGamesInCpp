/*
 * sdl_stbtt - stb_truetype demo using SDL
 * Robert Norris, May 2011
 * Public Domain
 *
 * Compile:
 *   gcc --std=c99 -o sdl_stbtt sdl_stbtt.c `sdl-config --cflags --libs` -lm
 *
 * Run:
 *   ./sdl_stbtt <path-to-ttf-file> <text-to-render>
 */

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <SDL.h>

#define FONT_HEIGHT 32

#define STB_TRUETYPE_IMPLEMENTATION
#include "stb_truetype.h"

int main(int argc, char** argv) {
    if (argc != 3) {
        printf("usage: sdl_stbtt <path-to-ttf-file> <text-to-render>\n");
        exit(-1);
    }

    /* getting the font into memory. this uses mmap, but a fread variant would
     * work fine */
    int fontfd = open(argv[1], O_RDONLY);
    if (fontfd < 0) {
        perror("couldn't open font file");
        exit(1);
    }

    struct stat st;
    if (fstat(fontfd, &st) < 0) {
        perror("couldn't stat font file");
        close(fontfd);
        exit(1);
    }

    void* fontdata = mmap(NULL, st.st_size, PROT_READ, MAP_SHARED, fontfd, 0);
    if (!fontdata) {
        perror("couldn't map font file");
        close(fontfd);
        exit(1);
    }

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        fprintf(stderr, "sdl init failed: %s\n", SDL_GetError());
        munmap(fontdata, st.st_size);
        close(fontfd);
        exit(1);
    }

    /* creating an off-screen surface to render the glyphs into. stbtt outputs
     * the glyphs in 8-bit greyscale, so we want a 8-bit surface to match */
    SDL_Surface* glyphdata = SDL_CreateRGBSurface(SDL_SWSURFACE, 512, 512, 8, 0, 0, 0, 0);
    if (!glyphdata) {
        fprintf(stderr, "couldn't create sdl buffer: %s\n", SDL_GetError());
        munmap(fontdata, st.st_size);
        close(fontfd);
        SDL_Quit();
        exit(1);
    }

    /* 8-bit sdl surfaces are indexed (palletised), so setup a pallete with
     * 256 shades of grey. this is needed so the sdl blitter has something to
     * convert from when blitting to a direct colour surface */
    SDL_Color colors[256];
    for (int i = 0; i < 256; i++) {
        colors[i].r = i;
        colors[i].g = i;
        colors[i].b = i;
    }
    SDL_SetPalette(glyphdata, SDL_LOGPAL | SDL_PHYSPAL, colors, 0, 256);

    /* "bake" (render) lots of interesting glyphs into the bitmap. the cdata
     * array ends up with metrics for each glyph */
    stbtt_bakedchar cdata[96];
    stbtt_BakeFontBitmap(fontdata, stbtt_GetFontOffsetForIndex(fontdata, 0), FONT_HEIGHT, glyphdata->pixels, 512, 512, 32, 96, cdata);

    /* done with the raw font data now */
    munmap(fontdata, st.st_size);
    close(fontfd);

    /* create a direct colour on-screen surface */
    SDL_Surface* s = SDL_SetVideoMode(640, 480, 32, 0);
    if (!s) {
        fprintf(stderr, "sdl video mode init failed: %s\n", SDL_GetError());
        SDL_FreeSurface(glyphdata);
        SDL_Quit();
        exit(1);
    }

    /* the actual text draw. we loop over the characters, find the
     * corresponding glyph and blit it to the correct place in the on-screen
     * surface */

     /* x and y are the position in the dest surface to blit the next glyph to */
    float x = 0, y = 0;
    for (char* c = argv[2]; *c; c++) {
        /* stbtt_aligned_quad effectively holds a source and destination
         * rectangle for the glyph. we get one for the current char */
        stbtt_aligned_quad q;
        stbtt_GetBakedQuad(cdata, 512, 512, *c - 32, &x, &y, &q, 1);

        /* now convert from stbtt_aligned_quad to source/dest SDL_Rects */

        /* width and height are simple */
        int w = q.x1 - q.x0;
        int h = q.y1 - q.y0;

        /* t0,s0 and t1,s1 are texture-space coordinates, that is floats from
         * 0.0-1.0. we have to scale them back to the pixel space used in the
         * glyph data bitmap. its a simple as multiplying by the glyph bitmap
         * dimensions */
        SDL_Rect src = { .x = q.s0 * 512, .y = q.t0 * 512, .w = w, .h = h };

        /* in gl/d3d the y value is inverted compared to what sdl expects. y0
         * is negative here. we add (subtract) it to the baseline to get the
         * correct "top" position to blit to */
        SDL_Rect dest = { .x = q.x0, .y = FONT_HEIGHT + q.y0, .w = w, .h = h };

        /* draw it */
        SDL_BlitSurface(glyphdata, &src, s, &dest);
    }

    /* done with the glyphdata now */
    SDL_FreeSurface(glyphdata);

    /* wait for escape */
    SDL_Event e;
    while (SDL_WaitEvent(&e) && e.type != SDL_KEYDOWN && e.key.keysym.sym != SDLK_ESCAPE);

    SDL_FreeSurface(s);
    SDL_Quit();

    exit(0);
}