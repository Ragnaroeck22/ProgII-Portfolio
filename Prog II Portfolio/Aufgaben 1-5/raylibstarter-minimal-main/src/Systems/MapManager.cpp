#include "MapManager.h"
#include "../Enums.h"
#include "../Tiles/TileTraversable.h"
#include "../Tiles/TileBlocked.h"
#include "../Tiles/TileStart.h"
#include "../Tiles/TileExit.h"
#include "../Tiles/TileChest.h"
#include "../Tiles/TilePath.h"
#include <string>
#include <algorithm>

MapManager::MapManager()
{
    // Load tile textures
    this->texBlocked = LoadTexture("assets/graphics/obstacles.png");
    this->texTraversable = LoadTexture("assets/graphics/paths.png");

    this->mapSize.x = 19;
    this->mapSize.y = 19;
    this->tileSize = 50;
    this->generateMap();
    this->calcCost();

    drawItemPickup = false;

    this->spawnPlayer();

    this->actors.push_back(this->player);
}

void MapManager::Update()
{
    this->checkActorCollisions();

    this->autoTraverse();

    this->player->Update();

    // Check if item should be dropped
    if (player->shouldDropItem)
    {
        TraceLog(LOG_INFO, "Dropping item (map)");
        items.push_back(player->itemToDrop);
        player->shouldDropItem = false;
    }

    // Check if item can be picked up
    drawItemPickup = false;
    for (int i = 0; i < items.size(); i++)
    {
        if (player->getPosition().x == items[i].position.x && player->getPosition().y == items[i].position.y)
        {
            drawItemPickup = true;
            if (IsKeyPressed(KEY_E))
            {
                // Remove item from level if pickup was successful
                if (player->myInventory->addItem(items[i]))
                {
                    items.erase(items.cbegin() + i);
                }
            }
        }
    }

}

void MapManager::Draw()
{
    // Draw map
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

    // Draw items
    for (int i = 0; i < items.size(); i++)
    {
        items[i].Draw({items[i].position.x * tileSize, items[i].position.y * tileSize});
    }
    if (drawItemPickup)
    {
        DrawText("[E]", player->getPosition().x * tileSize + tileSize / 2 - MeasureText("[E]", 30) / 2, player->getPosition().y * tileSize - tileSize / 1.4, 30, WHITE);
    }

    // Draw player
    Vector2 playerDrawPos = this->player->getPosition();
    playerDrawPos.x = playerDrawPos.x * this->tileSize;
    playerDrawPos.y = playerDrawPos.y * this->tileSize;
    this->player->Draw(playerDrawPos);


}

void MapManager::generateMap() {
    this->map.clear();

    std::vector<std::shared_ptr<Tile>> workingVec;

    for (int i = 0; i < this->mapSize.x; i++)
    {
        workingVec.clear();
        for (int j = 0; j < this->mapSize.y; j++)
        {
            if (i % 2 == 0 && j % 2 == 0)
            {
                workingVec.push_back(std::make_shared<TileBlocked>(this->texBlocked, this->texTraversable));
            }
            else
            {
                workingVec.push_back(std::make_shared<TileTraversable>(this->texTraversable));
            }
        }
        this->map.push_back(workingVec);
    }

    int targetX;
    do
    {
        targetX = rand() % (int) this->mapSize.x;
    } while (targetX % 2 == 0);
    this->setTile({static_cast<float>(targetX), 0}, Start);

    do
    {
        targetX = rand() % (int) this->mapSize.x;
    } while (targetX % 2 == 0);

    this->setTile({static_cast<float>(targetX), this->mapSize.y - 1}, Exit);

    // Generate path
    Vector2 homeCoords = {this->startPos.x, this->startPos.y + 1};
    this->setTile(homeCoords, Path);

    for (int i = 1; i < mapSize.y / 2 - 1; i++)
    {
        do
        {
            targetX = rand() % (int) this->mapSize.x;
        } while (targetX % 2 == 0);

        if (targetX != homeCoords.x)
        {
            int startX;
            int stopX;

            if (targetX < homeCoords.x)
            {
                startX = targetX;
                stopX = homeCoords.x;
            }
            else
            {
                startX = homeCoords.x;
                stopX = targetX;
            }
            TraceLog(LOG_INFO, "targetX:");
            TraceLog(LOG_INFO, std::to_string(targetX).c_str());
            while (startX <= stopX)
            {
                this->setTile({static_cast<float>(startX), homeCoords.y}, Path);
                startX++;
            }
        }
        setTile({static_cast<float>(targetX), homeCoords.y + 1}, Path);
        homeCoords = {static_cast<float>(targetX), homeCoords.y + 2};
        setTile(homeCoords, Path);
    }
    targetX = this->exitPos.x;
    if (targetX != homeCoords.x)
    {
        int startX;
        int stopX;

        if (targetX < homeCoords.x)
        {
            startX = targetX;
            stopX = homeCoords.x;
        }
        else
        {
            startX = homeCoords.x;
            stopX = targetX;
        }
        while (startX <= stopX)
        {
            this->setTile({static_cast<float>(startX), homeCoords.y}, Path);
            startX++;
        }
    }

    // Randomize tiles
    for (int i = 0; i < this->mapSize.x; i++) {
        for (int j = 0; j < this->mapSize.y; j++) {
            if (i % 2 == 1 || j % 2 == 1) {
                if (this->getTile({static_cast<float>(i), static_cast<float>(j)})->type == Traversable)
                {
                    int randTile = rand() % 3; // Determines the chance of generating blocked tiles
                    if (randTile == 0)
                    {
                        setTile({static_cast<float>(i), static_cast<float>(j)}, Blocked);
                    }
                }
            }
                workingVec.push_back(std::make_shared<TileBlocked>(this->texBlocked, this->texTraversable));

        }
        this->map.push_back(workingVec);
    }

    // Spawn chests
    int chestsSpawned = 0;
    while (chestsSpawned < 7) {
        // Search random path tile (bad in theory, works in practice)
        int randX;
        int randY;
        do {
            randX = rand() % (int) mapSize.x;
            randY = rand() % (int) mapSize.y;
        } while (this->getTile({(float) randX, (float) randY})->type != Path);

        bool setChest = false;
        std::vector<Direction> directionsToCheck = {Left, Right, Up, Down};
        int randDirection = rand() % 4;


        Vector2 targetCoords;
        switch (directionsToCheck[randDirection])
        {
            case Left:
                targetCoords = {(float) randX - 1, (float) randY};
                break;
            case Right:
                targetCoords = {(float) randX + 1, (float) randY};
                break;
            case Up:
                targetCoords = {(float) randX, (float) randY - 1};
                break;
            case Down:
                targetCoords = {(float) randX, (float) randY + 1};
                break;
            default:
                targetCoords = {(float) randX - 1, (float) randY};
        }
        if (this->getTile(targetCoords)->type == Traversable)
        {
            this->setTile(targetCoords, Chest);
            setChest = true;
            chestsSpawned++;
        }

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
                workingPtr = std::make_shared<TileStart>(this->texBlocked, this->texTraversable);
                this->start = workingPtr;
                this->startPos = coordinates;
                break;
            case Exit:
                workingPtr = std::make_shared<TileExit>(this->texBlocked, this->texTraversable);
                this->exit = workingPtr;
                this->exitPos = coordinates;
                break;
            case Blocked:
                workingPtr = std::make_shared<TileBlocked>(this->texBlocked, this->texTraversable);
                break;
            case Traversable:
                workingPtr = std::make_shared<TileTraversable>(this->texTraversable);
                break;
            case Chest:
                workingPtr = std::make_shared<TileChest>(this->texBlocked, this->texTraversable);
                break;
            case Path:
                workingPtr = std::make_shared<TilePath>(this->texTraversable);
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
        TraceLog(LOG_INFO, "Map border reached, returning NULL [MapManager::getTile()]");
        return NULL;
    }
}

void MapManager::spawnPlayer()
{

    this->player = std::make_shared<Player>();
    this->player->setPosition(this->startPos);
    TraceLog(LOG_INFO, "Setting player position:");
    TraceLog(LOG_INFO, std::to_string(this->startPos.x).c_str());
    TraceLog(LOG_INFO, std::to_string(this->startPos.y).c_str());

}

void MapManager::checkActorCollisions() {
    for (int i = 0; i < this->actors.size(); i++)
    {
        if (actors[i]->updateCollision)
        {
            actors[i]->updateCollision = false;
            actors[i]->unblockAll();
            Vector2 actorPos = this->actors[i]->getPosition();

            // Check Up
            if (this->getTile({actorPos.x, actorPos.y - 1}) != NULL) // Should prevent crashes
            {
                if (this->getTile({actorPos.x, actorPos.y - 1})->type == Blocked)
                {
                    this->actors[i]->blockDirection(Up);
                }
            }
            else // If getTile() returns NULL (indicates map border), block that direction
            {
                this->actors[i]->blockDirection(Up);
            }
            // Check Down
            if (this->getTile({actorPos.x, actorPos.y + 1}) != NULL)
            {
                if (this->getTile({actorPos.x, actorPos.y + 1})->type == Blocked)
                {
                    this->actors[i]->blockDirection(Down);
                }
            }
            else
            {
                this->actors[i]->blockDirection(Down);
            }
            // Check Left
            if (this->getTile({actorPos.x - 1, actorPos.y}) != NULL)
            {
                if (this->getTile({actorPos.x - 1, actorPos.y})->type == Blocked)
                {
                    this->actors[i]->blockDirection(Left);
                }
            }
            else
            {
                this->actors[i]->blockDirection(Left);
            }
            // Check Right
            if (this->getTile({actorPos.x + 1, actorPos.y}) != NULL)
            {
                if (this->getTile({actorPos.x + 1, actorPos.y})->type == Blocked)
                {
                    this->actors[i]->blockDirection(Right);
                }
            }
            else
            {
                this->actors[i]->blockDirection(Right);
            }
        }
    }
}

std::vector<std::shared_ptr<Tile>> MapManager::getNeighbors(Vector2 coordinates)
{
    std::vector<std::shared_ptr<Tile>> returnVec;

    std::shared_ptr<Tile> tileToPush;

    tileToPush = getTile({coordinates.x - 1, coordinates.y});
    if (tileToPush != NULL)
    {
        returnVec.push_back(tileToPush);
    }
    tileToPush = getTile({coordinates.x + 1, coordinates.y});
    if (tileToPush != NULL)
    {
        returnVec.push_back(tileToPush);
    }
    tileToPush = getTile({coordinates.x, coordinates.y - 1});
    if (tileToPush != NULL)
    {
        returnVec.push_back(tileToPush);
    }
    tileToPush = getTile({coordinates.x, coordinates.y + 1});
    if (tileToPush != NULL)
    {
        returnVec.push_back(tileToPush);
    }
    return returnVec;
}

Vector2 MapManager::getTileCoords(std::shared_ptr<Tile> tile)
{
    for (int i = 0; i < this->mapSize.x; i++)
    {
        for (int j = 0; j < this->mapSize.y; j++)
        {
            if (this->map[i][j] == tile)
                return {(float) i, (float) j};
        }
    }
}


void MapManager::calcCost()
{
    std::vector<std::shared_ptr<Tile>> vecOpen;

    //Set up the start tile
    start->costG = 0;
    start->costH = calcCostH(start);

    vecOpen.push_back(start);

    //Begin the loop
    bool pathFound = false;
    std::shared_ptr<Tile> currentTile = nullptr;

    while (!pathFound)
    {
        //Choose the most suitable tile to visit
        if (!vecOpen.empty()) {
            //Sort the vector, tile with the lowest cost is at the end
            for (int i = 0; i < vecOpen.size() - 1; i++)
                for (int j = 0; j < vecOpen.size() - i - 1; j++)
                    if (vecOpen[j]->getCostF() < vecOpen[j + 1]->getCostF())
                    {
                        // Swap cells
                        std::shared_ptr<Tile> helper = vecOpen[j];
                        vecOpen[j] = vecOpen[j + 1];
                        vecOpen[j + 1] = helper;
                    }

            currentTile = vecOpen[vecOpen.size() - 1];
        }
        else
        {
            //Out of open tiles, path not found
            break;
        }
        //Remove the pointer to the current tile from the openTiles vector, as I'm about to visit the tile
        vecOpen.pop_back();

        //Set the current tile as visited
        currentTile->wasVisited = true;

        //Analyze neighbours
        std::vector<std::shared_ptr<Tile>> neighbours = getNeighbors(getTileCoords(currentTile));
        for (int i = 0; i < neighbours.size(); i++)
        {
            //If the neighbour tile was already checked, skip it.
            //If the unit cannot move through the neighbour tile, also skip it.

            if (!neighbours[i]->wasVisited && neighbours[i]->type != Blocked) {

                //This can happen multiple times per tile

                //Set G value
                /*
                I first need to check if the neighbour tile is diagonal or not.
                If it's diagonal, I would add 14 to the current G, otherwise 10.
                I only change the G value if the new value would be smaller than
                the current one.
                */

                if (currentTile->costG + 1 < neighbours[i]->costG) {
                    neighbours[i]->costG = currentTile->costG + 1;

                    //Set the parent
                    //Only if the new G is smaller than the previous G
                    neighbours[i]->parentTile = currentTile;

                }

                //This can only happen once per tile
                if (neighbours[i]->costH == 999) {
                    //Set H value
                    int h = calcCostH(neighbours[i]);
                    neighbours[i]->costH = h;

                    if (h == 0)
                    {
                        pathFound = true;
                    }

                    //Add this tile to the vector of open tiles
                    vecOpen.push_back(neighbours[i]);
                }
            }
        }

    }







/*
    std::vector<std::shared_ptr<Tile>> vecOpen;
    std::vector<std::shared_ptr<Tile>> vecClosed;

    this->start->costG = 0;
    vecOpen.push_back(this->start);

    while(true)
    {
        std::shared_ptr<Tile> currentTile = vecOpen[0];

        for (int i = 0; i < vecOpen.size(); i++)
        {
            calcCostG(vecOpen[i]);
            calcCostH(vecOpen[i]);

            if (vecOpen[i]->getCostF() > currentTile->getCostF())
            {
                // Remove current cell from vecOpen
                for (int i = 0; i < vecOpen.size(); i++)
                {
                    if (vecOpen[i] == currentTile)
                        vecOpen.erase(vecOpen.cbegin() + i);
                }
                vecClosed.push_back(currentTile);

                if (currentTile == this->exit)
                {
                    return;
                }

                // Check neighbors of current tile
                std::vector<std::shared_ptr<Tile>> tilesToCheck = getNeighbors(getTileCoords(currentTile));
                for (int i = 0; i < tilesToCheck.size(); i++)
                {
                    // Check if neighbor is in vecClosed
                    bool inClosedVec = false;
                    for (int j = 0; j < vecClosed.size(); j++)
                    {
                        if (tilesToCheck[i] == vecClosed[j])
                            inClosedVec = true;
                    }
                    if (!inClosedVec && tilesToCheck[i]->type != Blocked)
                    {
                        bool inOpenVec = false;
                        for (int j = 0; j < vecClosed.size(); j++)
                        {
                            if (tilesToCheck[i] == vecClosed[j])
                                inOpenVec = true;
                        }
                        if (currentTile->costG + 1 < tilesToCheck[i]->costG || !inOpenVec)
                        {
                            tilesToCheck[i]->costG = currentTile->costG + 1;
                            tilesToCheck[i]->parentTile = currentTile;
                            if (!inOpenVec)
                            {
                                vecOpen.push_back(tilesToCheck[i]);
                            }
                        }
                    }
                }

            }
        }
    }
    */
}

int MapManager::calcCostG(std::shared_ptr<Tile> tile)
{
    Vector2 position = getTileCoords(tile);

    int diffX;
    int diffY;
    if (position.x <= startPos.x)
        diffX = startPos.x - position.x;
    else
        diffX = position.x - startPos.x;

    if (position.y <= startPos.y)
        diffY = startPos.y - position.y;
    else
        diffY = position.y - startPos.y;

    return diffX + diffY;
}

int MapManager::calcCostH(std::shared_ptr<Tile> tile)
{
    Vector2 position = getTileCoords(tile);

    int diffX;
    int diffY;
    if (position.x <= exitPos.x)
        diffX = exitPos.x - position.x;
    else
        diffX = position.x - exitPos.x;

    if (position.y <= exitPos.y)
        diffY = exitPos.y - position.y;
    else
        diffY = position.y - exitPos.y;

    return diffX + diffY;
}

void MapManager::autoTraverse()
{
    if (IsKeyPressed(KEY_K))
    {
        autoTraversing = true;
    }

    if (autoTraversing)
    {
        // if (player->getPosition() != exitPos) doesn't work for some reason
        if (player->getPosition().x != exitPos.x || player->getPosition().y != exitPos.y)
        {
            std::shared_ptr<Tile> currentTile = getTile(player->getPosition());
            std::vector<std::shared_ptr<Tile>> neighbors = getNeighbors(player->getPosition());

            std::shared_ptr<Tile> moveTo = neighbors[0];
            for (int i = 0; i < neighbors.size(); i++)
            {
                if (neighbors[i]->getCostF() < moveTo->getCostF())
                {
                    moveTo = neighbors[i];
                }
                else if (neighbors[i]->getCostF() == moveTo->getCostF() && neighbors[i]->costH < moveTo->costH)
                    moveTo = neighbors[i];

            }

            Vector2 coords = getTileCoords(moveTo);

            if (coords.x < player->getPosition().x)
                player->move(Left);
            else if (coords.x > player->getPosition().x)
                player->move(Right);
            else if (coords.y < player->getPosition().y)
                player->move(Up);
            else if (coords.y > player->getPosition().y)
                player->move(Down);
        }
        else
        {
            autoTraversing = false;
        }
    }
}