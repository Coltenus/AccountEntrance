//
// Created by colte on 31.01.2023.
//
#include <SDL.h>
#include <SDL_net.h>
#include <vector>
#include <string>
#include "src/common/Window.h"
#include "src/common/Label.h"

#define PLAYER_COUNT_LABEL "Player count: "
#define MAX_PLAYER_COUNT 10
#define TIMEOUT_TIME 5
#define POLLING_TIME 10

struct Data {
    TCPsocket socket;
    Uint32 timeout;
    Uint32 id;
    Data(TCPsocket sock, Uint32 t, short i)
    : socket(sock), timeout(t), id(i) {}
};

Window* Window::pWindow = nullptr;
TTF_Font* Label::Font = nullptr;

int main(int argc, char* argv[]) {
    SDL_Init(SDL_INIT_EVERYTHING);
    SDLNet_Init();
    char buffer[512];
    short curId = 1;
    short playersCount = 0;
    std::string strPlCount = PLAYER_COUNT_LABEL;
    strPlCount += std::to_string(playersCount);
    SDL_Event event;
    std::vector<Data> socketsData;
    bool active = true;
    SDLNet_SocketSet sockets = SDLNet_AllocSocketSet(MAX_PLAYER_COUNT);
    float* color = Window::CreateColor();
    Window* window = Window::CreateWindowObject("Server", color);
    SDL_Renderer* renderer = window->GetRenderer();
    Label::LibraryInit();
    Label::OpenFont("open_sans_reg.ttf");
    Label* lPlayerCount = new Label(renderer, &strPlCount, 10, 10);
    IPaddress ip;
    SDLNet_ResolveHost(&ip, nullptr, 2000);
    TCPsocket server = SDLNet_TCP_Open(&ip);

    Uint32 PollingMsgTime = SDL_GetTicks()/1000;
    while (active) {
        if(SDL_GetTicks()/1000 - PollingMsgTime >= POLLING_TIME) {
            PollingMsgTime = SDL_GetTicks()/1000;
            printf("Server is polling.\n");
        }
        window->Update(active);

        TCPsocket tempSocket = SDLNet_TCP_Accept(server);
        if(tempSocket) {
            if(playersCount < MAX_PLAYER_COUNT) {
                SDLNet_TCP_AddSocket(sockets, tempSocket);
                socketsData.push_back(Data(tempSocket, SDL_GetTicks()/1000, curId));
                curId++; playersCount++;
                strPlCount = PLAYER_COUNT_LABEL;
                strPlCount += std::to_string(playersCount);
                lPlayerCount->UpdateText(true);
                memset(buffer, 0, 512);
                sprintf(buffer, "Your ID is %d\n", (socketsData.end()-1)->id);
                printf("New connection: %d\n", (socketsData.end()-1)->id);
            } else {
                memset(buffer, 0, 512);
                sprintf(buffer, "Server has maximum connections(%d)\n", MAX_PLAYER_COUNT);
            }
            SDLNet_TCP_Send(tempSocket, buffer, strlen(buffer)+1);
        }

        while (SDLNet_CheckSockets(sockets, 0) > 0) {
            for(short i = 0; i<socketsData.size(); i++) {
                if(SDLNet_SocketReady(socketsData[i].socket)) {
                    memset(buffer, 0, 512);
                    SDLNet_TCP_Recv(socketsData[i].socket, buffer, 512);
                    short num = buffer[0] - '0';
                    short j = 1;
                    while(buffer[j] > '0' && buffer[j] <= '9') {
                        num *= 10;
                        num += buffer[j] - '0';
                        j++;
                    }

                    switch(num) {
                        case 0:
                            socketsData[i].timeout = SDL_GetTicks()/1000;
                            memset(buffer, 0, 512);
                            sprintf(buffer, "3 \n");
                            SDLNet_TCP_Send(socketsData[i].socket, buffer, strlen(buffer) + 1);
                            break;
                        case 1:
                            memset(buffer, 0, 512);
                            sprintf(buffer, "Player %d has disconnected\n", socketsData[i].id);
                            printf("%s", buffer);
                            sprintf(buffer, "1 %s", std::string(buffer).c_str());
                            for(short k = 0; k<socketsData.size(); k++) {
                                if(k == i)
                                    continue;
                                else {
                                    SDLNet_TCP_Send(socketsData[k].socket, buffer, strlen(buffer)+1);
                                }
                            }
                            SDLNet_TCP_DelSocket(sockets, socketsData[i].socket);
                            SDLNet_TCP_Close(socketsData[i].socket);
                            socketsData.erase(socketsData.begin()+i);
                            playersCount--;
                            strPlCount = PLAYER_COUNT_LABEL;
                            strPlCount += std::to_string(playersCount);
                            lPlayerCount->UpdateText(true);
                            break;
                        case 2:
                            std::string buf(buffer);
                            buf.erase(buf.begin(), buf.begin() + j + 1);
                            memset(buffer, 0, 512);
                            sprintf(buffer, "Player %d said: %s", socketsData[i].id, buf.c_str());
                            printf("%s", buffer);
                            sprintf(buffer, "1 %s", std::string(buffer).c_str());
                            for(short k = 0; k<socketsData.size(); k++) {
                                if (k == i)
                                    continue;
                                else {
                                    SDLNet_TCP_Send(socketsData[k].socket, buffer, strlen(buffer) + 1);
                                }
                            }
                            break;
                    }
                }
            }
        }

        for(short i = 0; i<socketsData.size(); i++) {
            if(SDL_GetTicks()/1000-socketsData[i].timeout > TIMEOUT_TIME) {
                memset(buffer, 0, 512);
                sprintf(buffer, "Player %d has disconnected\n", socketsData[i].id);
                printf("%s", buffer);
                sprintf(buffer, "1 %s", std::string(buffer).c_str());
                for(short k = 0; k<socketsData.size(); k++) {
                    if(k == i)
                        continue;
                    else {
                        SDLNet_TCP_Send(socketsData[k].socket, buffer, strlen(buffer)+1);
                    }
                }
                SDLNet_TCP_DelSocket(sockets, socketsData[i].socket);
                SDLNet_TCP_Close(socketsData[i].socket);
                socketsData.erase(socketsData.begin()+i);
                playersCount--;
                strPlCount = PLAYER_COUNT_LABEL;
                strPlCount += std::to_string(playersCount);
                lPlayerCount->UpdateText(true);
            }
        }

        window->ClearBG();
        lPlayerCount->Draw();
        window->Render();
    }

    memset(buffer, 0, 512);
    sprintf(buffer, "2 \n");
    for(short i = 0; i<socketsData.size(); i++) {
        SDLNet_TCP_Send(socketsData[i].socket, buffer, strlen(buffer) + 1);
        SDLNet_TCP_Close(socketsData[i].socket);
    }

    SDLNet_FreeSocketSet(sockets);
    SDLNet_TCP_Close(server);

    delete lPlayerCount;
    lPlayerCount = nullptr;
    Label::CloseFont();
    Label::LibraryQuit();
    delete window;
    window= nullptr;
    Window::DestroyColor(&color);
    SDLNet_Quit();
    SDL_Quit();

    return EXIT_SUCCESS; // video stopped at 39:35
}