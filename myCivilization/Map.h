#pragma once
#include <vector>
#include <cstdlib>
#include "Tile.h"
#include "olcPixelGameEngine.h"

class Map {
private:
    int width;
    int height;

public:
    std::vector<Tile> tiles;
    Map(int w, int h);
    void GenerateRandom();
    Tile& GetTile(int x, int y);
    void Draw(olc::PixelGameEngine* pge);
    Tile* GetTileAtMouse(olc::PixelGameEngine* pge);
    void AddInternalBuilding(int ownerID, BuildingType parentDistrict, BuildingType newBuilding);
    int GetWidth() const { return width; }
    int GetHeight() const { return height; }
};

inline Map::Map(int w, int h) : width(w), height(h) {
    tiles.resize(width * height);
    GenerateRandom();
}

inline void Map::GenerateRandom() {
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            Tile& t = GetTile(x, y);
            int r = rand() % 100;
            if (r < 15) t.terrain = TerrainType::Water;
            else if (r < 25) t.terrain = TerrainType::Mountain;
            else if (r < 45) t.terrain = TerrainType::Forest;
            else if (r < 55) t.terrain = TerrainType::Desert;
            else t.terrain = TerrainType::Plains;
        }
    }
}

inline Tile& Map::GetTile(int x, int y) {
    if (x < 0) x = 0; if (x >= width) x = width - 1;
    if (y < 0) y = 0; if (y >= height) y = height - 1;
    return tiles[y * width + x];
}

inline void Map::Draw(olc::PixelGameEngine* pge) {
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            GetTile(x, y).Draw(pge, x * 64, y * 64);
        }
    }
}

inline Tile* Map::GetTileAtMouse(olc::PixelGameEngine* pge) {
    int mx = pge->GetMouseX() / 64;
    int my = pge->GetMouseY() / 64;
    if (mx < 0 || my < 0 || mx >= width || my >= height) return nullptr;
    return &GetTile(mx, my);
}

inline void Map::AddInternalBuilding(int ownerID, BuildingType parentDistrict, BuildingType newBuilding) {
    for (auto& tile : tiles) {
        if (tile.ownerID == ownerID && tile.externalBuilding == parentDistrict) {
            tile.internalBuildings.insert(newBuilding);
            return;
        }
    }
    for (auto& tile : tiles) {
        if (tile.externalBuilding == parentDistrict && tile.ownerID == -1) {
            tile.ownerID = ownerID;
            tile.internalBuildings.insert(newBuilding);
            return;
        }
    }
}