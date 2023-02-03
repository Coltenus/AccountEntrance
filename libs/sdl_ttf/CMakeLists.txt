cmake_minimum_required(VERSION 3.24)

file(GLOB DLLS ${CMAKE_CURRENT_LIST_DIR}/*.dll)
file(COPY ${DLLS} DESTINATION ".")