cmake_minimum_required(VERSION 3.13)
project(marvel_vs_capcom)


SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread")

set(CMAKE_CXX_STANDARD 11)


set(SOURCE_FILES
        src/main.cpp
        src/tools/json/ConfigFileParser/ConfigFileParser.h
        src/tools/json/ConfigFileParser/ConfigFileParser.cpp
        src/tools/logger/Logger.cpp
        src/tools/logger/Logger.h
        src/MCGame.cpp
        src/MCGame.h
        src/Texture/Texture.cpp
        src/Texture/Texture.h
        src/tools/FPSManager/FPSManager.cpp
        src/tools/FPSManager/FPSManager.h
        src/InputManager.cpp
        src/InputManager.h
        src/tools/ImageLoader/ImageLoader.h
        src/tools/ImageLoader/ImageLoader.cpp
        src/Player.cpp
        src/Player.h
        src/Controls/Controls.cpp
        src/Controls/Controls.h
        src/Controls/ArrowControls.cpp
        src/Controls/ArrowControls.h
        src/Controls/WASDControls.cpp
        src/Controls/WASDControls.h
        src/Layer.cpp
        src/Layer.h
        src/Parallax.cpp
        src/Parallax.h
        src/TCPServer.cpp
        src/TCPServer.h
        src/Socket.cpp
        src/Socket.h
        src/Client/TCPClient.cpp
        src/Client/TCPClient.h
        src/CharactersServer/CharacterServer.cpp
        src/CharactersServer/CharacterServer.h
        src/CharactersClient/CharacterClient.cpp
        src/CharactersClient/CharacterClient.h
        src/CharactersClient/SpidermanClient.cpp
        src/CharactersClient/SpidermanClient.h
        src/CharactersClient/WolverineClient.cpp
        src/CharactersClient/WolverineClient.h
        src/CharactersServer/SpidermanServer.cpp
        src/CharactersServer/SpidermanServer.h
        src/CharactersServer/WolverineServer.cpp
        src/CharactersServer/WolverineServer.h
        src/Team.h
        src/Team.cpp
        src/action_constants.h
        src/ClientCursor.cpp
        src/ClientCursor.h
        src/ServerCursor.cpp
        src/ServerCursor.h
)


add_executable(marvel_vs_capcom ${SOURCE_FILES})


INCLUDE(FindPkgConfig)

PKG_SEARCH_MODULE(SDL2 REQUIRED SDL2/SDL)
PKG_SEARCH_MODULE(SDL2IMAGE REQUIRED SDL2/SDL_image)

INCLUDE_DIRECTORIES(${SDL2_INCLUDE_DIRS} ${SDL2IMAGE_INCLUDE_DIRS})
TARGET_LINK_LIBRARIES(${PROJECT_NAME} ${SDL2_LIBRARIES} ${SDL2IMAGE_LIBRARIES})


