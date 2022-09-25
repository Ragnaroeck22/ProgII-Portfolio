#include <cstdlib>

#include "raylib.h"

#include "config.h"

#include "Systems/MapManager.h"
#include "Tiles/Tile.h"
#include "Enums.h"

int main() {
    // Raylib initialization
    // Project name, screen size, fullscreen mode etc. can be specified in the config.h.in file
    InitWindow(Game::ScreenWidth, Game::ScreenHeight, Game::PROJECT_NAME);
    SetTargetFPS(60);

#ifdef GAME_START_FULLSCREEN
    ToggleFullscreen();
#endif

    // ===== INITIALISATION =====
    MapManager mapManager;

    // For test purposes
    Vector2 coords = {6, 6};
    mapManager.setTile(coords, Blocked);
    coords = {2, 2};
    mapManager.setTile(coords, Start);
    coords = {7, 3};
    mapManager.setTile(coords, Exit);
    coords = { 4, 8 };
    mapManager.setTile(coords, Chest);

    // Main game loop
    while (!WindowShouldClose()) // Detect window close button or ESC key
    {
        // ====== UPDATE ======

        //testMapManager.Update();


        // ======= DRAW =======
        BeginDrawing();

            ClearBackground(BLACK);

            mapManager.Draw();

        EndDrawing();
    } // Main game loop end

    // De-initialization here
    // ...
    // ...


    // Close window and OpenGL context
    CloseWindow();

    return EXIT_SUCCESS;
}
