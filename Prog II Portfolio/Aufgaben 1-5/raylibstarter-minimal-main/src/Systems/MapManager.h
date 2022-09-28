#pragma once

#include <vector>
#include <memory>
#include <raylib.h>

#include "../Tiles/Tile.h"
#include "../Characters/Char.h"
#include "../Characters/Player.h"

class MapManager
{
    // Attributes
private:
    Vector2 mapSize;
    int tileSize;

    // Tile textures
    Texture2D texTraversable;
    Texture2D texBlocked;

    // UI
    bool drawItemPickup;


    std::vector<std::vector<std::shared_ptr<Tile>>> map;

    std::vector<std::shared_ptr<Char>> actors;
    std::vector<ItemBase> items;
    std::shared_ptr<Player> player;

    std::shared_ptr<Tile> start;
    std::shared_ptr<Tile> exit;
    Vector2 startPos;
    Vector2 exitPos;

    // Methods
public:
    MapManager();

    void Update();
    void Draw();

    void generateMap();
    void setTile(Vector2 coordinates, TileType tileType);
    std::shared_ptr<Tile> getTile(Vector2 coordinates);

    void spawnPlayer();

    void checkActorCollisions();

    void checkForChests();

    // For A* algorithm
    std::vector<std::shared_ptr<Tile>> getNeighbors(Vector2 coordinates);
    Vector2 getTileCoords(std::shared_ptr<Tile> tile);
    void calcCost();
    int calcCostG(std::shared_ptr<Tile> tile);
    int calcCostH(std::shared_ptr<Tile> tile);
    void autoTraverse();

    bool autoTraversing = false;
};