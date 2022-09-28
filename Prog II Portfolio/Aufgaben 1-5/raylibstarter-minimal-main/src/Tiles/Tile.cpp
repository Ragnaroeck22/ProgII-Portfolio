#include "Tile.h"
#include <string>

void Tile::Draw(Vector2 position)
{
    this->CustomDraw(position);

    if (IsKeyDown(KEY_G))
    {
        DrawText(std::to_string(costG).c_str(), position.x, position.y, 20, RED);
    }
    else if (IsKeyDown(KEY_H))
    {
        DrawText(std::to_string(costH).c_str(), position.x, position.y, 20, RED);
    }
    else if (IsKeyDown(KEY_J))
    {
        DrawText(std::to_string(getCostF()).c_str(), position.x, position.y, 20, RED);
    }
}

int Tile::getCostF()
{
    return costG + costH;
}

int Tile::calcPathLength()
{
    TraceLog(LOG_INFO, "Calling internal path length");
    return calcPathLengthInternal(parentTile);
}

int Tile::calcPathLengthInternal(std::shared_ptr<Tile> tileToInspect)
{
    if (tileToInspect == nullptr)
    {
        TraceLog(LOG_INFO, "Return value");
        return 0;
    }
    else
    {
        TraceLog(LOG_INFO, "Make call");
        return calcPathLengthInternal(tileToInspect->parentTile) + 1;
    }
}

