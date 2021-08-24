cmake_minimum_required(VERSION 3.15)
project(code)
set(CMAKE_CXX_STANDARD 11)
SET(CMAKE_BUILD_TYPE "Debug")
SET(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g -ggdb")

add_subdirectory(Epoll)
add_subdirectory(Timer)
add_subdirectory(Thread)

add_executable(code main.cpp)
add_executable(Client client.cpp)

target_link_libraries(code Server Timer Thread)
target_link_libraries(Server -lrt)
