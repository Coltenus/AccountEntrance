cmake_minimum_required(VERSION 3.24)

find_package(SDL2 REQUIRED)
include_directories(${CMAKE_PROJECT_NAME} ${SDL2_INCLUDE_DIRS})
find_package(GLEW REQUIRED)
include_directories(${CMAKE_PROJECT_NAME} ${GLEW_INCLUDE_DIRS})

set(IMGUI_LIBRARIES
        ${SDL2_LIBRARIES}
        -lopengl32
        GLEW::glew)

add_library(imgui STATIC
        ${CMAKE_CURRENT_LIST_DIR}/imconfig.h
        ${CMAKE_CURRENT_LIST_DIR}/imgui.h
        ${CMAKE_CURRENT_LIST_DIR}/imgui_internal.h
        ${CMAKE_CURRENT_LIST_DIR}/imstb_truetype.h
        ${CMAKE_CURRENT_LIST_DIR}/imstb_textedit.h
        ${CMAKE_CURRENT_LIST_DIR}/imstb_rectpack.h
        ${CMAKE_CURRENT_LIST_DIR}/imgui_impl_opengl3.h
        ${CMAKE_CURRENT_LIST_DIR}/imgui_impl_sdl.h
        ${CMAKE_CURRENT_LIST_DIR}/imgui_setup.h)

target_sources(imgui PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/imgui.cpp
        ${CMAKE_CURRENT_LIST_DIR}/imgui_demo.cpp
        ${CMAKE_CURRENT_LIST_DIR}/imgui_draw.cpp
        ${CMAKE_CURRENT_LIST_DIR}/imgui_tables.cpp
        ${CMAKE_CURRENT_LIST_DIR}/imgui_widgets.cpp
        ${CMAKE_CURRENT_LIST_DIR}/imgui_impl_opengl3.cpp
        ${CMAKE_CURRENT_LIST_DIR}/imgui_impl_sdl.cpp
        ${CMAKE_CURRENT_LIST_DIR}/imgui_setup.cpp)

file(GLOB DLLS ${CMAKE_CURRENT_LIST_DIR}/*.dll)
file(COPY ${DLLS} DESTINATION ".")

target_link_libraries(imgui PRIVATE ${IMGUI_LIBRARIES})
target_link_libraries(AccountEntrance PRIVATE imgui)