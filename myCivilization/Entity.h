#pragma once
#include "olcPixelGameEngine.h"

class Entity {
public:
    virtual ~Entity() = default;
    virtual void Draw(olc::PixelGameEngine* pge, int px, int py) = 0;
    virtual void DrawInfo(olc::PixelGameEngine* pge, int x, int y) = 0;
};