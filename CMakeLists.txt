cmake_minimum_required(VERSION 3.24)
project(program)
project(server)
project(AccountEntrance)

set(CMAKE_CXX_STANDARD 20)
set(NO_CONSOLE_FLAG
        $<$<CONFIG:RELEASE>:-mwindows>)
if(WIN32)
    set(OPENGL_LIB -lopengl32)
endif()

add_executable(AccountEntrance entrance.cpp)

include(libs/CMakeLists.txt)
target_link_libraries(AccountEntrance PRIVATE ${NO_CONSOLE_FLAG})

add_executable(program program.cpp src/common/Window.h src/common/Window.cpp src/common/Label.cpp src/common/Label.h)

file(GLOB FONT res/open_sans_reg.ttf)
file(COPY ${FONT} DESTINATION ".")

find_package(SDL2 REQUIRED)
include_directories(program ${SDL2_INCLUDE_DIRS})
target_link_libraries(program PRIVATE ${SDL2_LIBRARIES} ${OPENGL_LIB} -lSDL2_ttf -lSDL2_net ${NO_CONSOLE_FLAG})

add_executable(server server.cpp src/common/Window.h src/common/Window.cpp src/common/Label.cpp src/common/Label.h)

find_package(SDL2 REQUIRED)
include_directories(server ${SDL2_INCLUDE_DIRS})
target_link_libraries(server PRIVATE ${SDL2_LIBRARIES} ${OPENGL_LIB} -lSDL2_ttf -lSDL2_net ${NO_CONSOLE_FLAG})

