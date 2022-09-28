//
// Created by 22max on 03.08.2022.
//

#pragma once

enum TileType
{
    Start,
    Exit,
    Blocked,
    Traversable,
    Path,
    Chest
};

enum Direction
{
    Left = 0,
    Right,
    Up,
    Down
};

enum ItemType
{
    TypeWeapon,
    TypeNecklace,
    TypeRing,
};

enum SortType
{
    Weight,
    Price,
    Name,
};