#include <iostream>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>

void error1(const char what[]) {
    printf("%s: %s\n",what ,SDL_GetError() );
    exit(1);
}

const int W = 480, H = 320, FRAME_RATE = 25;

int main() {
    // more info at: https://wiki.libsdl.org/

    puts( "Initializing SDL.." );
    if(SDL_Init(SDL_INIT_EVERYTHING)) error1("SDL_Init Error");
    SDL_Window *win = SDL_CreateWindow("Pong", 100, 100, W, H, SDL_WINDOW_SHOWN);
    if (!win) error1("SDL_CreateWindow Error");
    SDL_Renderer *ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED|SDL_RENDERER_PRESENTVSYNC);
    if (!ren) error1("SDL_CreateRenderer Error");

    SDL_Surface *sur = SDL_LoadBMP("bg.bmp");
    if(!sur) error1("SDL_CreateRGBSurface error");
    SDL_Texture *bg = SDL_CreateTextureFromSurface(ren,sur);
    if(!bg) error1("SDL_CreateTextureFromSurface error");
    SDL_FreeSurface(sur);

    sur = SDL_LoadBMP("fb.bmp");
    if(!sur) error1("SDL_CreateRGBSurface error");
    SDL_Texture *fb = SDL_CreateTextureFromSurface(ren,sur);
    if(!fb) error1("SDL_CreateTextureFromSurface error");
    SDL_FreeSurface(sur);

    Uint32 rate_ms;
    SDL_Event e;
    int speed = 5;
    SDL_Rect pos;
    pos.x = 100;
    pos.y = 100;
    pos.h = 36;
    pos.w = 36;
    int dx = speed, dy = speed;

    bool running = true;
    while(running) { // gamel loop
         rate_ms = SDL_GetTicks();
        // check for input
        while(SDL_PollEvent(&e)) {
            switch(e.type) {
            case SDL_QUIT:
                running = false;
                break;
            case SDL_KEYDOWN:
                switch(e.key.keysym.scancode) {
                    case SDL_SCANCODE_RETURN:
                    case SDL_SCANCODE_RETURN2:
                    case SDL_SCANCODE_KP_ENTER:
                    case SDL_SCANCODE_SPACE:
                        dx = 0;
                        dy = 0;
                    case SDL_SCANCODE_ESCAPE:
                        running = false;
                    break;
                }
                break;
            case SDL_MOUSEBUTTONUP:
                pos.x = e.motion.x;
                pos.y = e.motion.y;
                break;
            }
        }
        // game logic
        pos.x += dx;
        pos.y += dy;
        if(pos.x<=1) dx = speed;
        else if(pos.x>=W-pos.w) dx = -speed;
        if(pos.y<=1) dy = speed;
        else if(pos.y>=H-pos.h) dy = -speed;
        // render
        //SDL_RenderClear(ren);
        SDL_RenderCopy(ren, bg, NULL, NULL);
        SDL_RenderCopy(ren, fb, NULL, &pos);
        SDL_RenderPresent(ren);
        // sleep
        rate_ms = SDL_GetTicks()-rate_ms;
        if (rate_ms<FRAME_RATE) SDL_Delay(FRAME_RATE-rate_ms);
    }

    // free resources
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    SDL_Quit();
    return 0;
}

