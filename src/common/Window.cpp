//
// Created by colte on 31.01.2023.
//

#include "Window.h"
#include <SDL_opengl.h>

Window::Window(const char *title, int width, int height, Uint32 flags, float *bgColor)
: _bgColor(bgColor) {
    SDL_DisplayMode dm;
    if(SDL_GetCurrentDisplayMode(0, &dm) != 0)
        printf("%s\n", SDL_GetError());
    _window = SDL_CreateWindow(title, dm.w/4, dm.h/9, width, height, flags);
    _renderer = SDL_CreateRenderer(_window, -1, 0);
}

Window* Window::CreateWindowObject(const char *title, float *bgColor, int width,
                                  int height, Uint32 flags) {
    if(pWindow == nullptr)
        pWindow = new Window(title, width, height, flags, bgColor);
    return pWindow;
}

Window::~Window() {
    if(pWindow) {
        SDL_DestroyRenderer(_renderer);
        SDL_DestroyWindow(_window);
        pWindow = nullptr;
    }
}

bool Window::LibraryInit() {
    if(SDL_WasInit(SDL_INIT_EVERYTHING) == 0)
        return SDL_Init(SDL_INIT_EVERYTHING) != 0;
    else return false;
}

void Window::LibraryQuit() {
    if(SDL_WasInit(SDL_INIT_EVERYTHING) != 0)
        SDL_Quit();
}

void Window::Update(bool& active) {
    SDL_Event event;

    // Events management
    while (SDL_PollEvent(&event)) {
        switch (event.type) {

            case SDL_QUIT:
                // handling of close button
                active = false;
                break;
        }
    }
}

void Window::ClearBG() {
    SDL_SetRenderDrawColor(_renderer, _bgColor[0], _bgColor[1], _bgColor[2], _bgColor[3]);
    SDL_RenderClear(_renderer);
}

void Window::Render() {
    SDL_RenderPresent(_renderer);

    // calculates to 60 fps
    SDL_Delay(1000 / 60);
}

SDL_Renderer *Window::GetRenderer() {
    return _renderer;
}

float* Window::CreateColor(float r, float g, float b, float a) {
    float** color = new float*;
    *color = new float[4];
    (*color)[0] = r;
    (*color)[1] = g;
    (*color)[2] = b;
    (*color)[3] = a;
    return *color;
}

void Window::DestroyColor(float **color) {
    delete[] *color;
    *color = nullptr;
}
