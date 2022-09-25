#include "MapManager.h"
#include "../Tiles/TileTraversable.h"
#include "../Tiles/TileBlocked.h"
#include "../Tiles/TileStart.h"
#include "../Tiles/TileExit.h"
#include "../Tiles/TileChest.h"

MapManager::MapManager()
{
    this->mapSize.x = 18;
    this->mapSize.y = 18;
    this->tileSize = 50;
    this->generateMap();
}

void MapManager::Update()
{

}

void MapManager::Draw()
{
    Vector2 position;
    position.x = 0;
    for (int i = 0; i < this->mapSize.x; i++)
    {
        position.y = 0;
        for (int j = 0; j < this->mapSize.y; j++)
        {
           this->map[i][j]->Draw(position);

            position.y = position.y + this->tileSize;

        }
        position.x = position.x + this->tileSize;
    }
}

void MapManager::generateMap()
{
    std::vector<std::shared_ptr<Tile>> workingVec;

    for (int i = 0; i < this->mapSize.x; i++)
    {
        for (int j = 0; j < this->mapSize.y; j++)
        {
            std::shared_ptr<Tile> workingPtr;

            workingPtr = std::make_shared<TileTraversable>();

            workingVec.push_back(workingPtr);
        }
        this->map.push_back(workingVec);
    }
}

void MapManager::setTile(Vector2 coordinates, TileType tileType)
{
    std::shared_ptr<Tile> workingPtr;

    if (coordinates.x < mapSize.x && coordinates.y < mapSize.y && coordinates.x >= 0 && coordinates.y >= 0)
    {
        switch(tileType)
        {
            case Start:
                workingPtr = std::make_shared<TileStart>();
                break;
            case Exit:
                workingPtr = std::make_shared<TileExit>();
                break;
            case Blocked:
                workingPtr = std::make_shared<TileBlocked>();
                break;
            case Traversable:
                workingPtr = std::make_shared<TileTraversable>();
                break;
            case Chest:
                workingPtr = std::make_shared<TileChest>();
        }
        this->map[coordinates.x][coordinates.y] = workingPtr;
    }
    else
    {
        TraceLog(LOG_INFO, "Error: Invalid coordinates for MapManager::setTile()");
    }
}

std::shared_ptr<Tile> MapManager::getTile(Vector2 coordinates)
{
    std::shared_ptr<Tile> workingPtr;

    if (coordinates.x < mapSize.x && coordinates.y < mapSize.y && coordinates.x >= 0 && coordinates.y >= 0)
    {
        workingPtr = this->map[coordinates.x][coordinates.y];
        return workingPtr;
    }
    else
    {
        TraceLog(LOG_INFO, "Error: Invalid coordinates for MapManager::setTile()");
        return nullptr;
    }
}
