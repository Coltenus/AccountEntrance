//
// Created by colte on 31.01.2023.
//

#ifndef SERVER_WINDOW_H
#define SERVER_WINDOW_H

#include <SDL.h>

class Window {
private:
    SDL_Window* _window;
    SDL_Renderer* _renderer;
    float* _bgColor;
    Window(const char *title, int width, int height, Uint32 flags, float* bgColor);

public:
    static float* CreateColor(float r = 50, float g = 100, float b = 150, float a = 255);
    static void DestroyColor(float** color);
    static Window* pWindow;
    static Window* CreateWindowObject(const char *title, float *bgColor, int width = 800, int height = 600,
                                Uint32 flags = 0);
    ~Window();
    static bool LibraryInit();
    static void LibraryQuit();
    void Update(bool& active);
    void ClearBG();
    void Render();
    SDL_Renderer* GetRenderer();
};


#endif //SERVER_WINDOW_H
