cmake_minimum_required(VERSION 3.5)
project(FlarialDumper)

set(CMAKE_CXX_STANDARD 23)

include(FetchContent)

FetchContent_Declare(
        libhat
        GIT_REPOSITORY  https://github.com/BasedInc/libhat.git
        GIT_TAG         9ef05d6961ce37a4c801f11159de895aa21878a9
)

FetchContent_MakeAvailable(libhat)

add_executable(FlarialDumper main.cpp)
target_link_libraries(FlarialDumper PRIVATE libhat)
