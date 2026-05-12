#pragma once
#include "Entity.h"
#include "GameDefs.h"
#include <vector>
#include <set>
#include <string>

class Unit;

class City : public Entity {
public:
    std::string name;
    olc::Pixel color;
    int population = 1;
    int ownerID;

    // Combat Stats
    int hp = 200;
    int maxHP = 200;

    // Economy & Growth
    int productionStored = 0;
    int foodStored = 0;

    bool isProducingUnit = true;
    UnitType unitQueue = UnitType::Warrior;
    BuildingType buildingQueue = BuildingType::None;

    int targetBuildX = -1;
    int targetBuildY = -1;

    std::set<BuildingType> builtBuildings;
    bool hasWalls = false;
    City(std::string cityName, olc::Pixel c, int owner);

    bool ProcessTurn(std::vector<Unit*>& newUnits, int extraProduction, int extraFood);

    void Draw(olc::PixelGameEngine* pge, int px, int py) override;
    void DrawInfo(olc::PixelGameEngine* pge, int x, int y) override;

    int GetProductionYield();
    int GetScienceYield();
    int GetCultureYield();
    int GetDefenseStrength();

    bool HasRequiredDistrict(BuildingType b);
};