//
// Created by colte on 29.01.2023.
//

#ifndef IMGUITESTS_IMGUI_SETUP_H
#define IMGUITESTS_IMGUI_SETUP_H

#include "imgui.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_opengl3.h"
#include <SDL.h>
#include <GL/glew.h>
#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <SDL_opengles2.h>
#else
#include <SDL_opengl.h>
#endif

ImGuiIO& ImGuiSDLInit(const char* title, ImVec2 size, SDL_Window** window, SDL_GLContext& gl_context);
void ImGuiSDLQuit(SDL_Window** window, SDL_GLContext& gl_context);
void ImGuiSDLUpdate(SDL_Window* window, ImGuiIO &io, ImVec4 &clear_color);

#endif //IMGUITESTS_IMGUI_SETUP_H
