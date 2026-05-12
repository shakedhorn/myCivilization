#pragma once
#include <set>
#include "GameDefs.h"
#include "olcPixelGameEngine.h"
#include "Unit.h"
#include "City.h"

class City;
class Unit;

class Tile {
public:
    TerrainType terrain = TerrainType::Plains;
    Unit* unit = nullptr;
    City* city = nullptr;
    ImprovementType improvement = ImprovementType::None;
    BuildingType externalBuilding = BuildingType::None;
    BuildingType pendingBuilding = BuildingType::None;
    std::set<BuildingType> internalBuildings;
    int ownerID = -1;
    void Draw(olc::PixelGameEngine* pge, int x, int y);
};

inline void Tile::Draw(olc::PixelGameEngine* pge, int x, int y) {
    // Terrain
    switch (terrain) {
    case TerrainType::Plains: pge->FillRect(x, y, 64, 64, olc::Pixel(100, 180, 100)); break;
    case TerrainType::Forest:
        pge->FillRect(x, y, 64, 64, olc::Pixel(34, 139, 34));
        pge->FillTriangle(x + 32, y + 10, x + 15, y + 50, x + 49, y + 50, olc::Pixel(20, 100, 20));
        break;
    case TerrainType::Mountain:
        pge->FillRect(x, y, 64, 64, olc::Pixel(169, 169, 169));
        pge->FillTriangle(x + 10, y + 60, x + 32, y + 10, x + 54, y + 60, olc::Pixel(80, 80, 80));
        break;
    case TerrainType::Water: pge->FillRect(x, y, 64, 64, olc::Pixel(30, 144, 255)); break;
    case TerrainType::Desert: pge->FillRect(x, y, 64, 64, olc::Pixel(238, 214, 175)); break;
    }
    // Improvements
    if (improvement == ImprovementType::Farm) {
        pge->FillRect(x + 10, y + 40, 15, 10, olc::YELLOW);
        pge->FillRect(x + 30, y + 35, 15, 10, olc::YELLOW);
    }
    else if (improvement == ImprovementType::Mine) {
        pge->FillCircle(x + 45, y + 45, 8, olc::BLACK); pge->DrawCircle(x + 45, y + 45, 8, olc::GREY);
    }
    else if (improvement == ImprovementType::BarbarianCamp) {
        pge->FillTriangle(x + 20, y + 50, x + 44, y + 50, x + 32, y + 20, olc::RED);
        pge->DrawTriangle(x + 20, y + 50, x + 44, y + 50, x + 32, y + 20, olc::BLACK);
    }
    if (pendingBuilding != BuildingType::None) {
        // Scaffolding look (Yellow Construction Frame)
        pge->DrawRect(x + 5, y + 5, 54, 54, olc::YELLOW); // Frame
        pge->DrawLine(x + 5, y + 5, x + 59, y + 59, olc::YELLOW); // Cross
        pge->DrawLine(x + 59, y + 5, x + 5, y + 59, olc::YELLOW); // Cross
        // Darken the background slightly to show activity
        pge->FillRect(x + 6, y + 6, 52, 52, olc::Pixel(0, 0, 0, 50));
        // Draw the letter of the building being built
        char symbol = '?';
        if (pendingBuilding == BuildingType::Campus) symbol = 'S';
        if (pendingBuilding == BuildingType::TheaterSquare) symbol = 'C';
        if (pendingBuilding == BuildingType::Workshop) symbol = 'P';
        pge->DrawString(x + 24, y + 24, std::string(1, symbol) + "...", olc::WHITE, 2);
    }

    // External Building (District)
    if (externalBuilding != BuildingType::None) {
        olc::Pixel distColor = olc::WHITE;
        std::string distName = "DIST";
        switch (externalBuilding) {
        case BuildingType::Campus:
            distColor = olc::CYAN;
            distName = "CAMPUS";
            break;
        case BuildingType::TheaterSquare:
            distColor = olc::MAGENTA;
            distName = "THEAT";
            break;
        case BuildingType::Workshop:
            distColor = olc::Pixel(255, 165, 0);
            distName = "WORK";
            break;
        default: break;
        }
        pge->FillRect(x + 8, y + 8, 48, 48, olc::Pixel(40, 40, 40));
        pge->DrawRect(x + 8, y + 8, 48, 48, distColor);
        pge->DrawString(x + 12, y + 15, distName, distColor, 1);
        int iconX = x + 12;
        int iconY = y + 40;
        for (auto b : internalBuildings) {
            olc::Pixel subColor = olc::WHITE;
            std::string sym = "";
            switch (b) {
            case BuildingType::Library:
                subColor = olc::BLUE;
                sym = "L";
                break;
            case BuildingType::Amphitheater:
                subColor = olc::RED;
                sym = "A";
                break;
            case BuildingType::Workshop:
                subColor = olc::GREY;
                sym = "+";
                break;
            default: break;
            }
            if (sym != "") {
                pge->FillRect(iconX, iconY, 12, 12, olc::BLACK);
                pge->DrawRect(iconX, iconY, 12, 12, subColor);
                pge->DrawString(iconX + 2, iconY + 2, sym, subColor, 1);
                iconX += 15;
            }
        }
    }
    pge->DrawRect(x, y, 64, 64, olc::Pixel(0, 0, 0, 50)); // Grid
    if (city) city->Draw(pge, x, y);
    if (unit) unit->Draw(pge, x, y);
}