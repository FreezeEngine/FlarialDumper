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

FetchContent_Declare(
        json
        GIT_REPOSITORY https://github.com/nlohmann/json.git
        GIT_TAG v3.9.1
)

FetchContent_GetProperties(json)

if(NOT json_POPULATED)
    FetchContent_Populate(json)
    add_library(nlohmann_json INTERFACE)
    target_include_directories(nlohmann_json INTERFACE ${json_SOURCE_DIR}/include)
endif()

add_executable(FlarialDumper main.cpp
        src/Resolver/Resolvers/VtableResolver/VtableResolver.cpp
        src/Resolver/Resolvers/SignatureResolver/SignatureResolver.cpp
        src/Utils/Utils.cpp)
target_link_libraries(FlarialDumper PRIVATE libhat nlohmann_json)
