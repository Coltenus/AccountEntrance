// Dear ImGui: standalone example application for SDL2 + OpenGL
// (SDL is a cross-platform general purpose library for handling windows, inputs, OpenGL/Vulkan/Metal graphics context creation, etc.)
// If you are new to Dear ImGui, read documentation from the docs/ folder + read the top of imgui.cpp.
// Read online: https://github.com/ocornut/imgui/tree/master/docs


#include <string>
#include <map>
#include "libs/imgui/imgui_setup.h"

#ifdef __unix
#define fopen_s(pFile,filename,mode) ((*(pFile))=fopen((filename),(mode)))==NULL
#endif

// Main code
int main(int, char**)
{
    SDL_Window* window = nullptr;
    SDL_GLContext gl_context;
    ImGuiIO& io = ImGuiSDLInit("Account Entrance", ImVec2(800, 600), &window, gl_context);

    // Our state
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    char login[21] = "", password[21] = "";
    std::map<std::string, std::pair<std::string, std::string>> data = {
            {"Coltenus", {"qwerty123", "zxc"}},
            {"trekfest", {"12345", "cxz"}}
    };
    std::string msg("Enter your data");
    float color[] = {0, 0, 0};

    // Main loop
    bool done = false;
    while (!done)
    {
        // Poll and handle events (inputs, window resize, etc.)
        // You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
        // - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application, or clear/overwrite your copy of the mouse data.
        // - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application, or clear/overwrite your copy of the keyboard data.
        // Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            ImGui_ImplSDL2_ProcessEvent(&event);
            if (event.type == SDL_QUIT)
                done = true;
            if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE && event.window.windowID == SDL_GetWindowID(window))
                done = true;
        }

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplSDL2_NewFrame(window);
        ImGui::NewFrame();

        // 2. Show a simple window that we create ourselves. We use a Begin/End pair to create a named window.
        {
            ImGui::Begin("Entrance", NULL, ImGuiWindowFlags_AlwaysAutoResize
            | ImGuiWindowFlags_NoResize);                          // Create a window called "Hello, world!" and append into it.

            ImGui::InputText("Login", login, 20, ImGuiInputTextFlags_NoHorizontalScroll | ImGuiInputTextFlags_CharsNoBlank);
            ImGui::Spacing();
            ImGui::Spacing();
            ImGui::InputText("Password", password, 20, ImGuiInputTextFlags_NoHorizontalScroll | ImGuiInputTextFlags_CharsNoBlank
            | ImGuiInputTextFlags_Password);
            ImGui::Spacing();
            ImGui::Spacing();
            if(ImGui::Button("Enter", ImVec2(80, 25))) {
                for(auto& el: data)
                {
                    if(std::string(login) == el.first && std::string(password) == el.second.first)
                    {
                        done = true;
                        FILE *file;
                        fopen_s(&file, "data", "wb");
                        fwrite(color, sizeof(float), 3, file);
                        fclose(file);
                        system((std::string("start program.exe ") + login + ' ' + data[std::string(login)].second).c_str());
                        break;
                    }
                }
                if(!done)
                    msg = "Wrong password";
            }
            ImGui::SameLine(120.f);
            ImGui::Text(msg.c_str());

            ImGui::End();
        }

        {
            ImGui::Begin("Settings", NULL, ImGuiWindowFlags_AlwaysAutoResize
                                           | ImGuiWindowFlags_NoResize);

            ImGui::ColorPicker3("Choose your background", color);

            ImGui::End();
        }

        ImGuiSDLUpdate(window, io, clear_color);
    }

    ImGuiSDLQuit(&window, gl_context);

    return 0;
}