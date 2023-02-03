//
// Created by colte on 31.01.2023.
//

#ifndef SERVER_LABEL_H
#define SERVER_LABEL_H

#include <SDL.h>
#include <SDL_ttf.h>

class Label {
private:
    SDL_Renderer* _renderer;
    SDL_Texture* _text;
    SDL_Rect _textRect;
    std::string* _textData;
    int _height, _x, _y;

public:
    static TTF_Font* Font;
    static bool LibraryInit();
    static void LibraryQuit(bool needQuitSDL = false);
    static void OpenFont(const char* dir);
    static void CloseFont();
    Label(SDL_Renderer* renderer, std::string* text, int x = 0, int y = 0, int height = 30);
    ~Label();
    void Draw();
    void UpdateText(bool isTextureExists = false);
};


#endif //SERVER_LABEL_H
