//
// Created by colte on 30.01.2023.
//
#include <string>
#include "src/common/Window.h"
#include "src/common/Label.h"
#include <SDL_net.h>

#define TIMEOUT_TIME 5

#ifdef __unix
#define fopen_s(pFile,filename,mode) ((*(pFile))=fopen((filename),(mode)))==NULL
#endif

Window* Window::pWindow = nullptr;
TTF_Font* Label::Font = nullptr;

int main(int argc, char** argv) {
    std::string nickname, id, sError;
    float color[4];
    color[0] = 150;
    color[1] = 150;
    color[2] = 150;
    color[3] = 255;
    char ipchar[20];
    short error = 0;
    if(argc == 3) {
        nickname = argv[1];
        id = argv[2];
    }
    else error = -1;
    if(error == 0){
        FILE* file;
        if(fopen_s(&file, "data", "rb") != 0)
            error = -2;
        if(error == 0) {
            fread(color, sizeof(float), 3, file);
            color[0] *= 255;
            color[1] *= 255;
            color[2] *= 255;
            color[3] = 255;
            fclose(file);
        }
    }

    if(Window::LibraryInit() || Label::LibraryInit())
        error = -3;

    //network
    sprintf(ipchar, "127.0.0.1");
    SDLNet_SocketSet server;
    TCPsocket connection;
    char buffer[512];
    if(error == 0) {
        SDLNet_Init();
        IPaddress ip;
        Uint16 port = 2000;
        SDLNet_ResolveHost(&ip, ipchar, port);
        connection = SDLNet_TCP_Open(&ip);
        if (connection == nullptr)
            error = -4;
    }
    if(error == 0) {
        server = SDLNet_AllocSocketSet(1);
        SDLNet_TCP_AddSocket(server, connection);
    }

    if(error == 0) {
        memset(buffer, 0, 512);
        SDLNet_TCP_Recv(connection, buffer, 512);
        printf("%s", buffer);
    }

    if(error != -3)
        Label::OpenFont("open_sans_reg.ttf");
    Window* window;
    Label* label;
    switch (error) {
        case 0:
            window = Window::CreateWindowObject("Program", color);
            label = new Label(window->GetRenderer(), &nickname, 10, 10);
            break;
        case -1:
            sError = "There is no args.";
            window = Window::CreateWindowObject("Error", color, 250, 100);
            label = new Label(window->GetRenderer(), &sError, 20, 30, 30);
            break;
        case -2:
            sError = "There is no file with data";
            window = Window::CreateWindowObject("Error", color, 360, 100);
            label = new Label(window->GetRenderer(), &sError, 20, 30, 30);
            break;
        case -4:
            sError = "The server is unavailable.";
            window = Window::CreateWindowObject("Error", color, 350, 100);
            label = new Label(window->GetRenderer(), &sError, 20, 30, 30);
            break;
    }

    if(error != -3) {
        bool active = true;
        bool receive;
        Uint32 timeout = SDL_GetTicks() / 1000;
        while (active) {
            SDL_Event event;

            // Events management
            while (SDL_PollEvent(&event)) {
                switch (event.type) {

                    case SDL_QUIT:
                        // handling of close button
                        active = false;
                        break;
                    case SDL_KEYDOWN:
                        if(error == 0) {
                            switch (event.key.keysym.sym) {
                                case 13:
                                    memset(buffer, 0, 512);
                                    sprintf(buffer, "2 Hey!\n");
                                    SDLNet_TCP_Send(connection, buffer, strlen(buffer) + 1);
                                    break;
                            }
                        }
                        break;
                }
            }

            //
            if(error == 0) {
                receive = true;
                while (receive && SDLNet_CheckSockets(server, 0) > 0
                       && SDLNet_SocketReady(connection)) {
                    short offset = 0;
                    short act;
                    bool close = false;
                    do {
                        memset(buffer, 0, 512);
                        offset += SDLNet_TCP_Recv(connection, buffer, 512);
                        act = buffer[0] - '0';
                        short j = 1;
                        while (buffer[j] > '0' && buffer[j] <= '9') {
                            act *= 10;
                            act += buffer[j] - '0';
                            j++;
                        }

                        {
                            std::string buf(buffer);
                            if (buf.length() > 0) {
                                buf.erase(buf.begin(), buf.begin() + j + 1);
                                memset(buffer, 0, 512);
                                sprintf(buffer, "%s", buf.c_str());
                            }
                        }

                        switch (act) {
                            case 1:
                                printf("%s", buffer);
                                break;
                            case 2:
                                active = false;
                                receive = false;
                                close = true;
                                break;
                            case 3:
                                timeout = SDL_GetTicks() / 1000;
                                break;
                        }

                        if (offset <= 0) {
                            receive = false;
                            break;
                        }
                    } while (!close && buffer[strlen(buffer) - 1] != '\n');
                }
                if (SDL_GetTicks() / 1000 - timeout >= TIMEOUT_TIME - 1) {
                    memset(buffer, 0, 512);
                    sprintf(buffer, "0 \n");
                    SDLNet_TCP_Send(connection, buffer, strlen(buffer) + 1);
                }

                if (SDL_GetTicks() / 1000 - timeout > TIMEOUT_TIME)
                    active = false;
            }

            window->ClearBG();
            label->Draw();
            window->Render();
        }
    }

    //network
    if(error == 0) {
        memset(buffer, 0, 512);
        sprintf(buffer, "1 \n");
        SDLNet_TCP_Send(connection, buffer, strlen(buffer) + 1);
        SDLNet_TCP_Close(connection);
        SDLNet_FreeSocketSet(server);
        SDLNet_Quit();
    }

    if(label != nullptr) {
        delete label;
        label = nullptr;
    }
    Label::CloseFont();
    Label::LibraryQuit();
    if(window != nullptr) {
        delete window;
        window = nullptr;
    }
    Window::LibraryQuit();

    return 0;
}