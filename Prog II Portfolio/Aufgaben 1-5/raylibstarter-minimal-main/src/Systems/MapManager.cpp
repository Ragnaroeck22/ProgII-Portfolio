#include "MapManager.h"
#include "../Enums.h"
#include "../Tiles/TileTraversable.h"
#include "../Tiles/TileBlocked.h"
#include "../Tiles/TileStart.h"
#include "../Tiles/TileExit.h"
#include "../Tiles/TileChest.h"
#include "../Tiles/TilePath.h"

#include "../Items/Sword.h"
#include "../Items/Spear.h"
#include "../Items/Axe.h"
#include "../Items/Mace.h"
#include "../Items/Flamberge.h"
#include "../Items/Warhammer.h"
#include "../Items/PlainNecklace.h"
#include "../Items/BoneNecklace.h"
#include "../Items/RingMinor.h"
#include "../Items/RingMajor.h"


#include <string>
#include <algorithm>

MapManager::MapManager()
{
    // Load tile textures
    this->texBlocked = LoadTexture("assets/graphics/obstacles.png");
    this->texTraversable = LoadTexture("assets/graphics/paths.png");

    drawItemPickup = false;
    floorCounter = 0;

    this->mapSize.x = 19;
    this->mapSize.y = 19;
    this->tileSize = 50;
    this->generateMap();

    this->spawnPlayer();

    //this->calcCost();

    this->actors.push_back(this->player);
}

void MapManager::Update()
{
    checkWinCondition();

    this->checkActorCollisions();

    checkForChests();

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

    // Draw floor counter
    std::string drawFloor = "Floor ";
    drawFloor.append(std::to_string(floorCounter));
    DrawText(drawFloor.c_str(), tileSize * 0.1, tileSize * 0.1, tileSize * 0.8, WHITE);

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
    floorCounter++;
    path.clear();

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
        // Search random path tile (bad in theory, works fine in practice)
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
    // Shouldn't happen, this silences the warning
    return {-500, -500};
}


void MapManager::calcCost()
{
    resetPath();
    std::shared_ptr<Tile> currentTile = getTile(player->getPosition());
    currentTile->fLocalGoal = 0;
    currentTile->fGlobalGoal = calcCostH(currentTile);

    std::vector<std::shared_ptr<Tile>> notTestedTiles;
    notTestedTiles.push_back(currentTile);


    while (!notTestedTiles.empty())
    {
        TraceLog(LOG_INFO, "Sorting vec (ascending)");
        for (int i = 0; i < notTestedTiles.size() - 1; i++)
        {
            for (int j = 0; j < (notTestedTiles.size() - i - 1); j++)
            {
                if (notTestedTiles[j]->fGlobalGoal > notTestedTiles[j + 1]->fGlobalGoal)
                {
                    std::shared_ptr<Tile> sortHelper = notTestedTiles[j];
                    notTestedTiles[j] = notTestedTiles[j + 1];
                    notTestedTiles[j + 1] = sortHelper;
                }
            }
        }

        while (!notTestedTiles.empty() && notTestedTiles.front()->wasVisited)
            notTestedTiles.erase(notTestedTiles.cbegin());


        if (notTestedTiles.empty())
            break;

        currentTile = notTestedTiles.front();
        currentTile->wasVisited = true;

        currentTile->neighbors = getNeighbors(getTileCoords(currentTile));

        for (auto tileNeighbor : currentTile->neighbors)
        {
            if (!tileNeighbor->wasVisited && tileNeighbor->type != Blocked)
                notTestedTiles.push_back(tileNeighbor);

            float fPossiblyLowerGoal = currentTile->fLocalGoal + 1;

            if (fPossiblyLowerGoal < tileNeighbor->fLocalGoal)
            {
                tileNeighbor->parentTile = currentTile;
                tileNeighbor->fLocalGoal = fPossiblyLowerGoal;

                tileNeighbor->fGlobalGoal = tileNeighbor->fLocalGoal + calcCostH(tileNeighbor);
            }
        }
    }
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

void MapManager::autoTraverse() {

    if (!player->myInventory->getOverencumbered()) {

        if (IsKeyPressed(KEY_K))
        {
            calcCost();
            std::shared_ptr<Tile> tileToTest = exit;
            path.clear();
            while (tileToTest != nullptr)
            {
                path.push_back(tileToTest);
                tileToTest->drawPathIndicator = true;
                tileToTest = tileToTest->parentTile;
            }
            autoTraversing = true;
        }

        if (autoTraversing) {
            // if (player->getPosition() != exitPos) doesn't work for some reason
            if (player->getPosition().x != exitPos.x || player->getPosition().y != exitPos.y)
            {
                if (!path.empty())
                {
                    Vector2 targetPos = getTileCoords(path.back());
                    if (targetPos.x != player->getPosition().x || targetPos.y != player->getPosition().y)
                    {

                        int diffX = player->getPosition().x - targetPos.x;
                        int diffY = player->getPosition().y - targetPos.y;


                        if (diffX > 0)
                        {
                            player->move(Left);
                        }
                        else if (diffX < 0)
                        {
                            player->move(Right);
                        }
                        else if (diffY > 0)
                        {
                            player->move(Up);
                        }
                        else if (diffY < 0)
                        {
                            player->move(Down);
                        }

                    }
                    if (!path.empty())
                    {
                        path.pop_back();
                    }
                }
            }
        }
        else
        {
            autoTraversing = false;
        }
    }
}


void MapManager::checkForChests()
{
    if (getTile(player->getPosition())->type == Chest)
    {
        int randomNumber = rand() % 3;

        switch (randomNumber)
        {
            case 0: // Spawn weapon
                randomNumber = rand() % 6;
                if (randomNumber == 0) // Doesn't work with Switch because items have to be instantiated
                {
                    Sword sword(player->getPosition().x, player->getPosition().y);
                    if (player->myInventory->getCurrentWeight() + sword.weight > player->myInventory->getMaxWeight())
                    {
                        items.push_back(sword);
                    }
                    else if (!player->myInventory->addItem(sword))
                    {
                        items.push_back(sword);
                    }
                }
                else if (randomNumber == 1)
                {
                    Spear spear(player->getPosition().x, player->getPosition().y);
                    if (player->myInventory->getCurrentWeight() + spear.weight > player->myInventory->getMaxWeight())
                    {
                        items.push_back(spear);
                    }
                    else if (!player->myInventory->addItem(spear))
                    {
                        items.push_back(spear);
                    }
                }
                else if (randomNumber == 2)
                {
                    Axe axe(player->getPosition().x, player->getPosition().y);
                    if (player->myInventory->getCurrentWeight() + axe.weight > player->myInventory->getMaxWeight())
                    {
                        items.push_back(axe);
                    }
                    else if (!player->myInventory->addItem(axe))
                    {
                        items.push_back(axe);
                    }
                }
                else if (randomNumber == 3)
                {
                    Mace mace(player->getPosition().x, player->getPosition().y);
                    if (player->myInventory->getCurrentWeight() + mace.weight > player->myInventory->getMaxWeight())
                    {
                        items.push_back(mace);
                    }
                    else if (!player->myInventory->addItem(mace))
                    {
                        items.push_back(mace);
                    }
                }
                else if (randomNumber == 4)
                {
                    Flamberge item(player->getPosition().x, player->getPosition().y);
                    if (player->myInventory->getCurrentWeight() + item.weight > player->myInventory->getMaxWeight())
                    {
                        items.push_back(item);
                    }
                    else if (!player->myInventory->addItem(item))
                    {
                        items.push_back(item);
                    }
                }
                else if (randomNumber == 5)
                {
                    Warhammer item(player->getPosition().x, player->getPosition().y);
                    if (player->myInventory->getCurrentWeight() + item.weight > player->myInventory->getMaxWeight())
                    {
                        items.push_back(item);
                    }
                    else if (!player->myInventory->addItem(item))
                    {
                        items.push_back(item);
                    }
                }
                break;
            case 1: // Spawn necklace
                randomNumber = rand() % 2;
                if (randomNumber == 0)
                {
                    PlainNecklace item(player->getPosition().x, player->getPosition().y);
                    if (player->myInventory->getCurrentWeight() + item.weight > player->myInventory->getMaxWeight())
                    {
                        items.push_back(item);
                    }
                    else if (!player->myInventory->addItem(item))
                    {
                        items.push_back(item);
                    }
                }
                else
                {
                    BoneNecklace item(player->getPosition().x, player->getPosition().y);
                    if (player->myInventory->getCurrentWeight() + item.weight > player->myInventory->getMaxWeight())
                    {
                        items.push_back(item);
                    }
                    else if (!player->myInventory->addItem(item))
                    {
                        items.push_back(item);
                    }
                }
                break;
            case 2: // Spawn ring
                randomNumber = rand() % 2;
                if (randomNumber == 0)
                {
                    RingMinor item(player->getPosition().x, player->getPosition().y);
                    if (player->myInventory->getCurrentWeight() + item.weight > player->myInventory->getMaxWeight())
                    {
                        items.push_back(item);
                    }
                    else if (!player->myInventory->addItem(item))
                    {
                        items.push_back(item);
                    }
                }
                else
                {
                    RingMajor item(player->getPosition().x, player->getPosition().y);
                    if (player->myInventory->getCurrentWeight() + item.weight > player->myInventory->getMaxWeight())
                    {
                        items.push_back(item);
                    }
                    else if (!player->myInventory->addItem(item))
                    {
                        items.push_back(item);
                    }
                }
                break;
        }
        setTile(player->getPosition(), Traversable);
    }
}


void MapManager::resetPath()
{
    for (int i = 0; i < this->mapSize.x; i++)
    {
        for (int j = 0; j < this->mapSize.y; j++)
        {
            this->map[i][j]->fGlobalGoal = 999;
            this->map[i][j]->fLocalGoal = 999;
            this->map[i][j]->drawPathIndicator = false;
            this->map[i][j]->parentTile = nullptr;
            this->map[i][j]->wasVisited = false;
        }
    }
}


void MapManager::checkWinCondition()
{
    if (player->getPosition().x == exitPos.x && player->getPosition().y == exitPos.y)
    {
        if (autoTraversing)
        {
            autoTraversing = false; // Just in case, should happen automatically
            player->myInventory->open();
        }
        if (!player->myInventory->getOpen()) // Changes map only when player is finished with his inventory (is closed)
        {
            items.clear();
            this->generateMap();

            for (int i = 0; i < actors.size(); i++)
            {
                actors[i]->updateCollision = true;
            }

            this->player->setPosition(this->startPos);
            TraceLog(LOG_INFO, "Setting player position:");
            TraceLog(LOG_INFO, std::to_string(this->startPos.x).c_str());
            TraceLog(LOG_INFO, std::to_string(this->startPos.y).c_str());
        }
    }
}
