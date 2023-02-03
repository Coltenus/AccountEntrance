//
// Created by colte on 31.01.2023.
//

#include <string>
#include "Label.h"

bool Label::LibraryInit() {

    if(SDL_WasInit(SDL_INIT_EVERYTHING) == 0)
        SDL_Init(SDL_INIT_EVERYTHING) != 0;
    if(TTF_WasInit() == 0)
        return TTF_Init() != 0;
    else return false;
}

void Label::LibraryQuit(bool needQuitSDL) {
    if(needQuitSDL && SDL_WasInit(SDL_INIT_EVERYTHING) != 0)
        SDL_Quit();
    while(TTF_WasInit() > 0) {
        TTF_Quit();
    }
}

Label::Label(SDL_Renderer* renderer, std::string* text, int x, int y, int height)
: _renderer(renderer), _textData(text), _height(height), _x(x), _y(y) {
    if(Font == nullptr)
        OpenFont("open_sans_reg.ttf");
    UpdateText();
}

void Label::OpenFont(const char *dir) {
    if(Font != nullptr)
        TTF_CloseFont(Font);
    Font = TTF_OpenFont(dir, 60);
    if(Font == nullptr)
        printf("There is no font file.\n");
}

void Label::CloseFont() {
    if(Font != nullptr) {
        TTF_CloseFont(Font);
        Font = nullptr;
    }
}

Label::~Label() {
    SDL_DestroyTexture(_text);
}

void Label::Draw() {
    SDL_RenderCopy(_renderer, _text, nullptr, &_textRect);
}

void Label::UpdateText(bool isTextureExists) {
    SDL_Surface* textSurface = TTF_RenderText_Solid(Font, _textData->c_str(), (SDL_Color){0, 0, 0, 255});
    if(isTextureExists)
        SDL_DestroyTexture(_text);
    _text = SDL_CreateTextureFromSurface(_renderer, textSurface);
    _textRect = {_x, _y, (int)_textData->length()*_height/10*4, _height};
    SDL_FreeSurface(textSurface);
}
