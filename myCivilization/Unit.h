#pragma once
#include "Entity.h"
#include "GameDefs.h"

class Unit : public Entity {
public:
    olc::Pixel color;
    UnitType type;
    int currentMovement;
    int maxMovement;
    int ownerID;
    int hp = 100;
    int maxHP = 100;
    bool hasMoved = false;
    bool isAlert = false;
    Unit(olc::Pixel c, UnitType t, int owner);
    void ResetTurn();
    void Draw(olc::PixelGameEngine* pge, int px, int py) override;
    void DrawInfo(olc::PixelGameEngine* pge, int x, int y) override;
};

inline Unit::Unit(olc::Pixel c, UnitType t, int owner) : color(c), type(t), ownerID(owner) {
    if (GameData::Units.count(t)) {
        maxMovement = GameData::Units[t].movement;
    }
    else {
        maxMovement = 2;
    }
    currentMovement = maxMovement;
    hasMoved = false;
    isAlert = false;
}

inline void Unit::ResetTurn() {
    currentMovement = maxMovement;
    hasMoved = false;
}

inline void Unit::Draw(olc::PixelGameEngine* pge, int px, int py) {
    pge->FillCircle(px + 32, py + 32, 15, color);
    pge->DrawCircle(px + 32, py + 32, 15, olc::BLACK);
    std::string symbol = "?";
    if (type == UnitType::Settler) symbol = "S";
    if (type == UnitType::Builder) symbol = "B";
    if (type == UnitType::Warrior) symbol = "W";
    if (type == UnitType::Archer) symbol = "A";
    if (type == UnitType::Horseman) symbol = "H";
    if (type == UnitType::Tank) symbol = "T";
    if (type == UnitType::Galley || type == UnitType::Ship) symbol = "U";
    if (type == UnitType::BarbarianWarrior) symbol = "!";
    pge->DrawString(px + 28, py + 28, symbol, olc::WHITE);
    if (maxMovement > 0) {
        float ratio = (float)currentMovement / (float)maxMovement;
        pge->FillRect(px + 10, py + 52, (int)(44 * ratio), 4, olc::GREEN);
    }
    if (hp < maxHP) {
        float hpRatio = (float)hp / (float)maxHP;
        pge->FillRect(px + 10, py + 46, 44, 4, olc::BLACK);
        pge->FillRect(px + 10, py + 46, (int)(44 * hpRatio), 4, olc::RED);
    }
    if (isAlert) {
        pge->DrawString(px + 40, py + 10, "Zzz", olc::CYAN);
    }
}

inline void Unit::DrawInfo(olc::PixelGameEngine* pge, int x, int y) {
    auto& stats = GameData::Units[type];
    pge->DrawString(x, y, stats.name, color, 2);
    pge->DrawString(x, y + 25, "HP: " + std::to_string(hp) + "/" + std::to_string(maxHP), olc::RED);
    pge->DrawString(x, y + 35, "Str: " + std::to_string(stats.combatStrength) + " Rng: " + std::to_string(stats.range));
    pge->DrawString(x, y + 45, "Moves: " + std::to_string(currentMovement) + "/" + std::to_string(maxMovement));
    if (isAlert) {
        pge->DrawString(x, y + 60, "STATUS: ALERT (Zzz)", olc::CYAN);
    }
}