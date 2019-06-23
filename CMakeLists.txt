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
        src/CharactersServer/Characters/CharacterServer.cpp
        src/CharactersServer/Characters/CharacterServer.h
        src/CharactersClient/CharacterClient.cpp
        src/CharactersClient/CharacterClient.h
        src/CharactersClient/SpidermanClient.cpp
        src/CharactersClient/SpidermanClient.h
        src/CharactersClient/WolverineClient.cpp
        src/CharactersClient/WolverineClient.h
        src/CharactersServer/Characters/SpidermanServer.cpp
        src/CharactersServer/Characters/SpidermanServer.h
        src/CharactersServer/Characters/WolverineServer.cpp
        src/CharactersServer/Characters/WolverineServer.h
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
        src/Menu/MenuTwoPlayers.cpp
        src/Menu/MenuTwoPlayers.h
        src/Menu/MenuFourPlayers.cpp
        src/Menu/MenuFourPlayers.h
        src/Menu/MenuThreePlayers.cpp
        src/Menu/MenuThreePlayers.h
        src/EventHandler.cpp
        src/EventHandler.h
        src/CharactersServer/Projectiles/ProjectileServer.cpp
        src/CharactersServer/Projectiles/ProjectileServer.h
        src/CharactersClient/ProjectileClient.cpp
        src/CharactersClient/ProjectileClient.h
        src/Timer.cpp
        src/Timer.h
        src/RoundBanner.cpp
        src/RoundBanner.h
        src/TimeBanner.cpp
        src/TimeBanner.h
        src/CharactersServer/Characters/IronmanServer.cpp
        src/CharactersServer/Characters/IronmanServer.h
        src/CharactersServer/Characters/RyuServer.cpp
        src/CharactersServer/Characters/RyuServer.h
        src/CharactersClient/RyuClient.cpp
        src/CharactersClient/RyuClient.h
        src/CharactersClient/IronmanClient.cpp
        src/CharactersClient/IronmanClient.h
        src/Barra.cpp
        src/Barra.h
        src/CharactersServer/Projectiles/ProjectileSpidermanServer.cpp src/CharactersServer/Projectiles/ProjectileSpidermanServer.h src/CharactersServer/Projectiles/ProjectileIronmanServer.cpp src/CharactersServer/Projectiles/ProjectileIronmanServer.h src/CharactersServer/Projectiles/ProjectileRyuServer.cpp src/CharactersServer/Projectiles/ProjectileRyuServer.h src/CharactersClient/ProjectileIronmanClient.cpp src/CharactersClient/ProjectileIronmanClient.h src/CharactersClient/ProjectileRyuClient.cpp src/CharactersClient/ProjectileRyuClient.h src/CharactersClient/ProjectileSpidermanClient.cpp src/CharactersClient/ProjectileSpidermanClient.h src/CharactersClient/ProjectileWolverineClient.cpp src/CharactersClient/ProjectileWolverineClient.h src/CharactersServer/Projectiles/ProjectileWolverineServer.cpp src/CharactersServer/Projectiles/ProjectileWolverineServer.h)





# includes cmake/FindSDL2.cmake
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake-build-debug/CMakeFiles/marvel_vs_capcom.dir)

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)

include_directories(${SDL2_INCLUDE_DIR}
        ${SDL2_IMAGE_INCLUDE_DIR})



add_executable(marvel_vs_capcom ${SOURCE_FILES} src/Menu/MenuThreePlayers.cpp src/Menu/MenuThreePlayers.h)
target_link_libraries(marvel_vs_capcom ${SDL2_LIBRARY}
        ${SDL2_IMAGE_LIBRARIES})

