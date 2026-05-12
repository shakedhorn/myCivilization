#include "City.h"
#include "Unit.h"
#include <algorithm> // For std::max

City::City(std::string cityName, olc::Pixel c, int owner) : name(cityName), color(c), ownerID(owner) {
    hp = 200; maxHP = 200;
}

int City::GetProductionYield() {
    int p = 1 + population;
    for (auto b : builtBuildings) p += GameData::Buildings[b].productionBonus;
    return p;
}

int City::GetScienceYield() {
    int s = 1 + (population / 2);
    for (auto b : builtBuildings) s += GameData::Buildings[b].scienceBonus;
    return s;
}

int City::GetCultureYield() {
    int c = 1;
    for (auto b : builtBuildings) c += GameData::Buildings[b].cultureBonus;
    return c;
}

int City::GetDefenseStrength() {
    int def = 10 + (population * 2);
    if (hasWalls) def += 20;
    return def;
}

bool City::HasRequiredDistrict(BuildingType b) {
    BuildingType parent = GameData::Buildings[b].requiredParent;
    if (parent == BuildingType::None) return true;
    return builtBuildings.count(parent) > 0;
}

bool City::ProcessTurn(std::vector<Unit*>& newUnits, int extraProduction, int extraFood) {
    if (hp < maxHP) hp += 10;
    if (hp > maxHP) hp = maxHP;

    // Growth Logic
    int foodPerTurn = 2 + extraFood;
    if (builtBuildings.count(BuildingType::Granary)) foodPerTurn += 2;

    foodStored += foodPerTurn;
    int growthThreshold = 15 + (population * 6);

    if (foodStored >= growthThreshold) {
        population++;
        foodStored = 0;
    }

    // Production Logic
    // If not producing anything, don't accumulate production (waste) or keep it stored? 
    // Usually in Civ it halts. We'll just skip adding logic if Idle.
    if (!isProducingUnit && buildingQueue == BuildingType::None) {
        return false;
    }

    int productionPerTurn = GetProductionYield() + extraProduction;
    productionStored += productionPerTurn;
    bool buildingFinishedOnMap = false;

    if (isProducingUnit) {
        int cost = GameData::Units[unitQueue].productionCost;
        if (productionStored >= cost) {
            productionStored = 0;
            if (unitQueue == UnitType::Settler) {
                if (population > 1) population--;
            }
            newUnits.push_back(new Unit(color, unitQueue, ownerID));

            // STOP PRODUCING - WAIT FOR ORDERS
            isProducingUnit = false;
            unitQueue = UnitType::Warrior; // Default but flag is false
            // Note: In main.cpp we will check if (isProducingUnit == false && buildingQueue == None)
        }
    }
    else {
        if (buildingQueue != BuildingType::None) {
            int cost = GameData::Buildings[buildingQueue].productionCost;
            if (productionStored >= cost) {
                productionStored = 0;
                builtBuildings.insert(buildingQueue);
                if (buildingQueue == BuildingType::Walls) hasWalls = true;
                if (GameData::Buildings[buildingQueue].requiresTile) buildingFinishedOnMap = true;

                // STOP PRODUCING - WAIT FOR ORDERS
                buildingQueue = BuildingType::None;
                isProducingUnit = false;
            }
        }
    }
    return buildingFinishedOnMap;
}

void City::Draw(olc::PixelGameEngine* pge, int px, int py) {
    // --- Draw City Base ---
    pge->FillRect(px + 16, py + 24, 32, 24, olc::DARK_GREY);
    pge->DrawLine(px + 32, py + 16, px + 32, py + 8, olc::WHITE);
    pge->FillRect(px + 28, py + 8, 8, 8, color);

    // Buildings
    if (builtBuildings.count(BuildingType::Monument)) {
        pge->FillRect(px + 18, py + 30, 4, 12, olc::GREY);
        pge->FillTriangle(px + 18, py + 30, px + 22, py + 30, px + 20, py + 26, olc::WHITE);
    }
    if (builtBuildings.count(BuildingType::Granary)) {
        pge->FillRect(px + 42, py + 32, 6, 10, olc::Pixel(139, 69, 19));
        pge->DrawRect(px + 42, py + 32, 6, 10, olc::BLACK);
    }
    if (hasWalls) {
        pge->DrawRect(px + 12, py + 20, 40, 32, olc::GREY);
        pge->DrawRect(px + 11, py + 19, 42, 34, olc::DARK_GREY);
    }

    // --- UI Elements ---
    int barX = px + 2; int barY = py - 12; int barW = 30; int barH = 4;

    // HP
    float hpRatio = (float)hp / (float)maxHP;
    pge->FillRect(barX, barY, barW, barH, olc::BLACK);
    pge->FillRect(barX, barY, (int)(barW * hpRatio), barH, olc::RED);
    pge->DrawRect(barX, barY, barW, barH, olc::WHITE);

    // Defense
    int defY = barY + 6;
    float defRatio = (float)GetDefenseStrength() / 50.0f;
    if (defRatio > 1.0f) defRatio = 1.0f;
    pge->FillRect(barX, defY, barW, barH, olc::BLACK);
    pge->FillRect(barX, defY, (int)(barW * defRatio), barH, olc::CYAN);
    pge->DrawRect(barX, defY, barW, barH, olc::WHITE);

    // Pop Bubble
    int bubbleX = px + 45; int bubbleY = py - 6;
    pge->FillCircle(bubbleX, bubbleY, 9, olc::WHITE);
    pge->DrawCircle(bubbleX, bubbleY, 9, olc::BLACK);
    std::string popStr = std::to_string(population);
    int txtOff = (popStr.length() > 1) ? 5 : 2;
    pge->DrawString(bubbleX - txtOff, bubbleY - 4, popStr, olc::BLACK);
}

void City::DrawInfo(olc::PixelGameEngine* pge, int x, int y) {
    pge->DrawString(x, y, name + " (Pop: " + std::to_string(population) + ")", color, 2);
    int threshold = 15 + (population * 6);
    pge->DrawString(x, y + 20, "Food: " + std::to_string(foodStored) + "/" + std::to_string(threshold), olc::GREEN);
    pge->DrawString(x + 120, y + 20, "Def: " + std::to_string(GetDefenseStrength()), olc::CYAN);
    std::string current = isProducingUnit ? GameData::Units[unitQueue].name : GameData::Buildings[buildingQueue].name;
    int cost = isProducingUnit ? GameData::Units[unitQueue].productionCost : GameData::Buildings[buildingQueue].productionCost;
    if (!isProducingUnit && buildingQueue == BuildingType::None) {
        current = "IDLE (Select Prod)";
        cost = 0;
    }
    int yield = GetProductionYield();
    int turnsLeft = 0;
    if (cost > 0 && yield > 0) {
        int remaining = cost - productionStored;
        if (remaining <= 0) turnsLeft = 1;
        else turnsLeft = (remaining + yield - 1) / yield;
    }
    pge->DrawString(x, y + 40, "Prod: " + current);
    if (cost > 0) {
        pge->DrawString(x, y + 50, std::to_string(productionStored) + "/" + std::to_string(cost) + " (" + std::to_string(turnsLeft) + "T)");
    }
    else {
        pge->DrawString(x, y + 50, "Waiting for orders...", olc::RED);
    }
    pge->DrawString(x, y + 65, "Y: P+" + std::to_string(GetProductionYield()) + " S+" + std::to_string(GetScienceYield()) + " C+" + std::to_string(GetCultureYield()));
}