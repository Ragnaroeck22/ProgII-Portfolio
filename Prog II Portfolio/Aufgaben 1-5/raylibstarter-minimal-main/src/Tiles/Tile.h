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
    int costG = 999;
    int costH = 999;
    int costF = 1998;
    //std::vector<std::shared_ptr<Tile>> neighbors;
    std::shared_ptr<Tile> parentTile = nullptr;
    bool wasVisited = false;
    Vector2 position; // Coordinates, only really used for pathfinding

protected:

    // Methods
public:
    virtual void Draw(Vector2 position);

    // For A* algorithm
    virtual int getCostF();
    virtual int calcPathLength();

protected:
    virtual int calcPathLengthInternal(std::shared_ptr<Tile> tileToInspect); // For A*

    virtual void CustomDraw(Vector2 position) = 0;
};