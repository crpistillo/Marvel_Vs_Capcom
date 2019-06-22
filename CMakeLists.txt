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
        src/signal_handler.cpp src/signal_handler.h
        src/Box.cpp
        src/Box.h
        src/Menu/Menu.h
        src/Menu/Menu.cpp
        src/Menu/MenuTwoPlayers.cpp src/Menu/MenuTwoPlayers.h src/Menu/MenuFourPlayers.cpp src/Menu/MenuFourPlayers.h src/Menu/MenuThreePlayers.cpp src/Menu/MenuThreePlayers.h
        src/EventHandler.cpp src/EventHandler.h src/CharactersServer/Projectile.cpp src/CharactersServer/Projectile.h src/CharactersClient/ProjectileClient.cpp src/CharactersClient/ProjectileClient.h src/Timer.cpp src/Timer.h src/RoundBanner.cpp src/RoundBanner.h src/TimeBanner.cpp src/TimeBanner.h
        )





# includes cmake/FindSDL2.cmake
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake-build-debug/CMakeFiles/marvel_vs_capcom.dir)

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)

include_directories(${SDL2_INCLUDE_DIR}
        ${SDL2_IMAGE_INCLUDE_DIR})



add_executable(marvel_vs_capcom ${SOURCE_FILES} src/Menu/MenuThreePlayers.cpp src/Menu/MenuThreePlayers.h)
target_link_libraries(marvel_vs_capcom ${SDL2_LIBRARY}
        ${SDL2_IMAGE_LIBRARIES})

