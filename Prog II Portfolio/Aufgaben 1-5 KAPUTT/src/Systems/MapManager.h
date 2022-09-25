#pragma once

#include <vector>
#include <memory>
#include <raylib.h>

#include "../Tiles/Tile.h"

class MapManager
{
    // Attributes
private:
    Vector2 mapSize;
    int tileSize;

    std::vector<std::vector<std::shared_ptr<Tile>>> map;

    // Methods
public:
    MapManager();

    void Update();
    void Draw();

    void generateMap();
    void setTile(Vector2 coordinates, TileType tileType);
    std::shared_ptr<Tile> getTile(Vector2 coordinates);
};