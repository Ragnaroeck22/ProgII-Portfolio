cmake_minimum_required(VERSION 3.11)

include(../cmake/raylib.cmake)

project(game VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(${PROJECT_NAME} main.cpp Enums.h Tiles/Tile.cpp Tiles/Tile.h Tiles/TileBlocked.cpp Tiles/TileBlocked.h Tiles/TileChest.cpp Tiles/TileChest.h Tiles/TileExit.cpp Tiles/TileExit.h Tiles/TileStart.cpp Tiles/TileStart.h Tiles/TileTraversable.cpp Tiles/TileTraversable.h Systems/MapManager.cpp Systems/MapManager.h Systems/Inventory.h Items/ItemBase.cpp Items/ItemBase.h Characters/Char.cpp Characters/Char.h Characters/Player.cpp Characters/Player.h Tiles/TilePath.cpp Tiles/TilePath.h Items/Sword.cpp Items/Sword.h Items/Weapon.cpp Items/Weapon.h Items/Spear.cpp Items/Spear.h Items/Axe.cpp Items/Axe.h Items/Ring.cpp Items/Ring.h Items/RingMinor.cpp Items/RingMinor.h Items/RingMajor.cpp Items/RingMajor.h Items/Necklace.cpp Items/Necklace.h Items/PlainNecklace.cpp Items/PlainNecklace.h Items/BoneNecklace.cpp Items/BoneNecklace.h Characters/Npc.cpp Characters/Npc.h)

# Add config.h.in file
configure_file(config.h.in config.h)
target_include_directories(${PROJECT_NAME} PUBLIC
        "${PROJECT_BINARY_DIR}"
        )

#  Link to Raylib
target_link_libraries(${PROJECT_NAME} raylib)

# Copy assets to assets path
add_custom_target(copy_assets
        COMMAND ${CMAKE_COMMAND} -P "${CMAKE_CURRENT_LIST_DIR}/../cmake/copy-assets.cmake"
        )
add_dependencies(${PROJECT_NAME} copy_assets)

if (APPLE)
    target_link_libraries(${PROJECT_NAME} "-framework IOKit")
    target_link_libraries(${PROJECT_NAME} "-framework Cocoa")
    target_link_libraries(${PROJECT_NAME} "-framework OpenGL")
endif ()

# CPack instructions
# call e.g. cpack -G ZIP -C Debug to create a binary distribution
install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION "${PROJECT_NAME}_Bundle")
install(DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/assets" DESTINATION "${PROJECT_NAME}_Bundle")