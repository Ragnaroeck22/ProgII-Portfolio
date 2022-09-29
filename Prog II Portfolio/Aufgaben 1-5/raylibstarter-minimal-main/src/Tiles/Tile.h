#pragma once
#include "raylib.h"
#include <vector>
#include <memory>
#include "../Enums.h"

class Tile
{
    // Attributes
public:
    Texture2D texSheet;
    Rectangle texRec;
    TileType type;

    // For A* algorithm
    int fGlobalGoal = 999;
    int fLocalGoal = 999;

    std::vector<std::shared_ptr<Tile>> neighbors;
    std::shared_ptr<Tile> parentTile = nullptr;
    bool wasVisited = false;
    bool drawPathIndicator = false;
    Vector2 position; // Coordinates, only really used for pathfinding

protected:

    // Methods
public:
    virtual void Draw(Vector2 position);

protected:

    virtual void CustomDraw(Vector2 position) = 0;
};