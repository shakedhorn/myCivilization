#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"

#include "GameDefs.h"
#include "Map.h"
#include "Unit.h"
#include "City.h"
#include "UI.h"
#include "Empire.h"
#include <ctime>
#include <algorithm> 
#include <limits> // For logic
using namespace std;

// --- Static Initialization ---
std::map<UnitType, UnitStats> GameData::Units;
std::map<BuildingType, BuildingStats> GameData::Buildings;
std::map<TechType, TechStats> GameData::Techs;
std::map<CivicType, CivicStats> GameData::Civics;
std::map<GovernmentType, GovernmentStats> GameData::Governments;

void GameData::Init() {
    // --- Units ---
    Units[UnitType::Settler] = { "Settler", 2, 0, 0, 80, TechType::None, false };
    Units[UnitType::Builder] = { "Builder", 2, 0, 0, 35, TechType::None, false };
    Units[UnitType::Warrior] = { "Warrior", 2, 10, 1, 25, TechType::None, false };
    Units[UnitType::BarbarianWarrior] = { "Barbarian", 2, 6, 1, 0, TechType::None, false };

    // Units linked to new Techs
    Units[UnitType::Archer] = { "Archer", 2, 15, 2, 40, TechType::Archery, false };
    Units[UnitType::Horseman] = { "Horseman", 4, 25, 1, 70, TechType::HorsebackRiding, false };
    Units[UnitType::Galley] = { "Galley", 3, 15, 1, 50, TechType::Sailing, true };
    Units[UnitType::Ship] = { "Battleship", 6, 70, 3, 450, TechType::Navigation, true }; // Navigation moved to late game
    Units[UnitType::Tank] = { "Tank", 5, 85, 1, 350, TechType::Combustion, false };

    // --- Buildings ---
    Buildings[BuildingType::Monument] = { "Monument", 30, TechType::None, 0, 2, 0, false, BuildingType::None };
    Buildings[BuildingType::Granary] = { "Granary", 40, TechType::Pottery, 0, 0, 1, false, BuildingType::None };
    Buildings[BuildingType::Walls] = { "Walls", 60, TechType::Masonry, 0, 0, 0, false, BuildingType::None };
    Buildings[BuildingType::Campus] = { "Campus", 100, TechType::Writing, 2, 0, 0, true, BuildingType::None };
    Buildings[BuildingType::Library] = { "Library", 80, TechType::Writing, 2, 0, 0, false, BuildingType::Campus };
    Buildings[BuildingType::TheaterSquare] = { "Theater Sq.", 100, TechType::Drama, 0, 2, 0, true, BuildingType::None };
    Buildings[BuildingType::Amphitheater] = { "Amphitheater", 80, TechType::Drama, 0, 2, 0, false, BuildingType::TheaterSquare };
    Buildings[BuildingType::Workshop] = { "Workshop", 100, TechType::Engineering, 0, 0, 3, true, BuildingType::None };

    // --- Governments ---
    Governments[GovernmentType::Despotism] = { "Despotism", 0, 0, 0, UnitType::Warrior, CivicType::None };
    Governments[GovernmentType::Monarchy] = { "Monarchy", 0, 1, 2, UnitType::Horseman, CivicType::MilitaryTradition };
    Governments[GovernmentType::Republic] = { "Republic", 1, 2, 0, UnitType::Builder, CivicType::CodeOfLaws };
    Governments[GovernmentType::Democracy] = { "Democracy", 3, 3, 0, UnitType::Settler, CivicType::Suffrage };

    // --- TECH TREE EXPANDED ---
    // Tier 1: Ancient
    Techs[TechType::Pottery] = { "Pottery", 25, TechType::None };
    Techs[TechType::Mining] = { "Mining", 25, TechType::None };
    Techs[TechType::AnimalHusbandry] = { "Animal Husb.", 25, TechType::None };
    // Tier 2: Classical Start
    Techs[TechType::Sailing] = { "Sailing", 40, TechType::Pottery };
    Techs[TechType::Writing] = { "Writing", 40, TechType::Pottery };
    Techs[TechType::Masonry] = { "Masonry", 45, TechType::Mining };
    Techs[TechType::BronzeWorking] = { "Bronze Work", 45, TechType::Mining };
    Techs[TechType::Archery] = { "Archery", 40, TechType::AnimalHusbandry };
    // Tier 3: Classical High
    Techs[TechType::Wheel] = { "The Wheel", 60, TechType::Masonry };
    Techs[TechType::Drama] = { "Drama", 70, TechType::Writing };
    Techs[TechType::Mathematics] = { "Mathematics", 80, TechType::Writing };
    // Tier 4: Medieval
    Techs[TechType::HorsebackRiding] = { "Horseback", 100, TechType::Archery };
    Techs[TechType::IronWorking] = { "Iron Working", 110, TechType::BronzeWorking };
    Techs[TechType::Construction] = { "Construction", 120, TechType::Masonry };
    Techs[TechType::Currency] = { "Currency", 120, TechType::Mathematics };
    Techs[TechType::Engineering] = { "Engineering", 150, TechType::Wheel };
    // Tier 5: Renaissance
    Techs[TechType::Printing] = { "Printing", 200, TechType::Drama }; // Drama -> Printing
    Techs[TechType::Astronomy] = { "Astronomy", 250, TechType::Sailing };
    Techs[TechType::Banking] = { "Banking", 280, TechType::Currency };
    Techs[TechType::Gunpowder] = { "Gunpowder", 300, TechType::IronWorking };
    Techs[TechType::Navigation] = { "Navigation", 350, TechType::Astronomy };
    // Tier 6: Industrial
    Techs[TechType::Industrialization] = { "Industry", 450, TechType::Engineering };
    Techs[TechType::SteamPower] = { "Steam Power", 500, TechType::Industrialization };
    Techs[TechType::Sanitation] = { "Sanitation", 400, TechType::Construction };
    Techs[TechType::Combustion] = { "Combustion", 600, TechType::SteamPower };
    // Tier 7: Modern
    Techs[TechType::Flight] = { "Flight", 750, TechType::Combustion };
    Techs[TechType::Radio] = { "Radio", 800, TechType::Flight };
    Techs[TechType::Plastics] = { "Plastics", 850, TechType::Combustion };
    Techs[TechType::Computers] = { "Computers", 1000, TechType::Radio };
    // Tier 8: Future
    Techs[TechType::Robotics] = { "Robotics", 1200, TechType::Computers };
    Techs[TechType::ArtificialIntelligence] = { "AI", 1500, TechType::Robotics };
    Techs[TechType::FutureTech] = { "Future Tech", 2000, TechType::ArtificialIntelligence };


    // --- CIVICS TREE EXPANDED ---
    // Tier 1
    Civics[CivicType::CodeOfLaws] = { "Code of Laws", 20, CivicType::None };
    Civics[CivicType::Mysticism] = { "Mysticism", 30, CivicType::None };
    // Tier 2
    Civics[CivicType::Craftsmanship] = { "Craftsmanship", 40, CivicType::CodeOfLaws };
    Civics[CivicType::ForeignTrade] = { "Foreign Trade", 40, CivicType::CodeOfLaws };
    // Tier 3
    Civics[CivicType::MilitaryTradition] = { "Mil. Tradition", 60, CivicType::Craftsmanship };
    Civics[CivicType::EarlyEmpire] = { "Early Empire", 70, CivicType::ForeignTrade };
    Civics[CivicType::StateWorkforce] = { "State Workforce", 80, CivicType::Craftsmanship };
    // Tier 4
    Civics[CivicType::Theology] = { "Theology", 100, CivicType::Mysticism };
    Civics[CivicType::Feudalism] = { "Feudalism", 120, CivicType::StateWorkforce };
    Civics[CivicType::CivilService] = { "Civil Service", 150, CivicType::Feudalism };
    Civics[CivicType::Guilds] = { "Guilds", 160, CivicType::Feudalism };
    // Tier 5
    Civics[CivicType::Humanism] = { "Humanism", 250, CivicType::Theology };
    Civics[CivicType::DiplomaticService] = { "Diplomatic Svc", 280, CivicType::Guilds };
    Civics[CivicType::Nationalism] = { "Nationalism", 350, CivicType::DiplomaticService };
    Civics[CivicType::Mercantilism] = { "Mercantilism", 350, CivicType::Humanism };
    // Tier 6
    Civics[CivicType::Urbanization] = { "Urbanization", 450, CivicType::CivilService };
    Civics[CivicType::Ideology] = { "Ideology", 600, CivicType::Nationalism };
    Civics[CivicType::Suffrage] = { "Suffrage", 700, CivicType::Ideology };
    // Tier 7
    Civics[CivicType::MassMedia] = { "Mass Media", 800, CivicType::Urbanization };
    Civics[CivicType::Globalization] = { "Globalization", 1000, CivicType::MassMedia };
    Civics[CivicType::SocialMedia] = { "Social Media", 1200, CivicType::Globalization };
}

enum class GameState { Map, TechTree, CultureTree, BuildingPlacement, Government, RangedTargeting };

class CivGame : public olc::PixelGameEngine {
public:
    Map map;
    GameState state = GameState::Map;
    std::vector<Empire> empires;
    int currentPlayer = 0;
    int turnCount = 1;
    const int MAX_TURNS = 500;

    int selectedX = -1;
    int selectedY = -1;
    Entity* selectedEntity = nullptr;
    BuildingType pendingBuilding = BuildingType::None;

    std::map<TechType, olc::vi2d> techPositions;
    std::map<CivicType, olc::vi2d> civicPositions;

    float treeScrollX = 0.0f;

    std::vector<std::string> cityNamesPool;

    std::string GetNextCityName() {
        if (cityNamesPool.empty()) return "New City " + std::to_string(rand() % 1000);
        int index = rand() % cityNamesPool.size();
        std::string name = cityNamesPool[index];
        cityNamesPool.erase(cityNamesPool.begin() + index);
        return name;
    }


    vector<string> GetTaskList() {
        vector<string> tasks;
        if (currentPlayer != 0) return tasks;
        if (empires[0].currentResearch == TechType::None && empires[0].HasAvailableTechs()) {
            tasks.push_back("- Choose Research");
        }
        if (empires[0].currentCivic == CivicType::None && empires[0].HasAvailableCivics()) {
            tasks.push_back("- Choose Civic");
        }
        for (int i = 0; i < map.tiles.size(); i++) {
            Tile& t = map.tiles[i];
            if (t.city && t.city->ownerID == 0) {
                if (t.city->isProducingUnit == false && t.city->buildingQueue == BuildingType::None) {
                    tasks.push_back("- City Idle: " + t.city->name);
                }
            }
        }
        if (state == GameState::BuildingPlacement) tasks.push_back("- Place District (Click Tile)");
        else if (state == GameState::RangedTargeting) tasks.push_back("- Select Target (Click Enemy)");
        else {
            for (int i = 0; i < map.tiles.size(); i++) {
                Tile& t = map.tiles[i];
                if (t.unit && t.unit->ownerID == 0 && t.unit->currentMovement > 0 && !t.unit->isAlert) {
                    tasks.push_back("- Move " + GameData::Units[t.unit->type].name);
                }
            }
        }
        return tasks;
    }

    CivGame() : map(26, 18) { sAppName = "myCivilization"; }

    bool OnUserCreate() override {
        GameData::Init();
        srand((unsigned int)time(nullptr));
        map.GenerateRandom();

        cityNamesPool = {
            "Jerusalem", "Abu-Dabi", "Tehran", "New-Delhi", "Budapest", "Warsaw",
            "Moscow", "Otawa", "Toronto", "Washington", "London", "Paris", "Rome",
            "Berlin", "Madrid", "Athens", "Cairo", "Tokyo", "Beijing", "New York"
        };

        // 0=Player, 1=AI, 3=Barbarians
        empires.push_back(Empire(0, olc::BLUE));
        empires.push_back(Empire(1, olc::RED));
        empires.push_back(Empire(3, olc::DARK_GREY));

        // Start Locations
        ForceLandAt(3, 3); ForceLandAt(map.GetWidth() - 4, map.GetHeight() - 4);
        map.GetTile(3, 3).unit = new Unit(olc::BLUE, UnitType::Settler, 0);
        map.GetTile(map.GetWidth() - 4, map.GetHeight() - 4).unit = new Unit(olc::RED, UnitType::Settler, 1);

        SpawnBarbarianCamp(); SpawnBarbarianCamp(); SpawnBarbarianCamp(); SpawnBarbarianCamp();

        // --- Init Tree Positions ---
        // Ancient (X=50)
        techPositions[TechType::Pottery] = { 50, 80 };
        techPositions[TechType::Mining] = { 50, 250 };
        techPositions[TechType::AnimalHusbandry] = { 50, 420 };
        // Classical (X=250)
        techPositions[TechType::Sailing] = { 250, 80 };  // From Pottery
        techPositions[TechType::Writing] = { 250, 160 }; // From Pottery
        techPositions[TechType::Masonry] = { 250, 250 }; // From Mining
        techPositions[TechType::BronzeWorking] = { 250, 330 }; // From Mining
        techPositions[TechType::Archery] = { 250, 420 }; // From Animals
        // Classical High (X=500)
        techPositions[TechType::Drama] = { 500, 160 }; // From Writing
        techPositions[TechType::Mathematics] = { 500, 220 }; // From Writing
        techPositions[TechType::Wheel] = { 500, 280 }; // From Masonry
        techPositions[TechType::Astronomy] = { 500, 80 }; // From Sailing
        // Medieval (X=750)
        techPositions[TechType::Currency] = { 750, 160 }; // From Math
        techPositions[TechType::Construction] = { 750, 240 }; // From Masonry
        techPositions[TechType::IronWorking] = { 750, 330 }; // From Bronze
        techPositions[TechType::Engineering] = { 750, 400 }; // From Wheel
        techPositions[TechType::HorsebackRiding] = { 750, 500 }; // From Archery
        // Renaissance (X=1000)
        techPositions[TechType::Printing] = { 1000, 160 };// From Drama
        techPositions[TechType::Banking] = { 1000, 240 };// From Currency
        techPositions[TechType::Gunpowder] = { 1000, 350 };// From Iron
        techPositions[TechType::Navigation] = { 1000, 80 }; // Correction: Shifted slightly
        // Industrial (X=1250)
        techPositions[TechType::Sanitation] = { 1250, 240 };
        techPositions[TechType::Industrialization] = { 1250, 400 }; // From Eng
        techPositions[TechType::SteamPower] = { 1450, 400 }; // From Ind
        // Modern (X=1700)
        techPositions[TechType::Combustion] = { 1700, 400 }; // From Steam
        techPositions[TechType::Flight] = { 1900, 300 };
        techPositions[TechType::Plastics] = { 1900, 400 };
        techPositions[TechType::Radio] = { 2100, 300 };
        techPositions[TechType::Computers] = { 2300, 350 };
        // Future (X=2500+)
        techPositions[TechType::Robotics] = { 2600, 350 };
        techPositions[TechType::ArtificialIntelligence] = { 2800, 350 };
        techPositions[TechType::FutureTech] = { 3000, 350 };

        // --- Civics Positions ---
        civicPositions[CivicType::CodeOfLaws] = { 50, 300 };
        civicPositions[CivicType::Mysticism] = { 50, 450 };
        civicPositions[CivicType::Craftsmanship] = { 250, 200 };
        civicPositions[CivicType::ForeignTrade] = { 250, 350 };
        civicPositions[CivicType::Theology] = { 250, 500 };
        civicPositions[CivicType::MilitaryTradition] = { 450, 200 };
        civicPositions[CivicType::StateWorkforce] = { 450, 280 };
        civicPositions[CivicType::EarlyEmpire] = { 450, 380 };
        civicPositions[CivicType::Feudalism] = { 650, 300 };
        civicPositions[CivicType::CivilService] = { 850, 300 };
        civicPositions[CivicType::Guilds] = { 850, 400 };
        civicPositions[CivicType::Humanism] = { 650, 500 };
        civicPositions[CivicType::Mercantilism] = { 850, 500 };
        civicPositions[CivicType::DiplomaticService] = { 1050, 400 };
        civicPositions[CivicType::Nationalism] = { 1250, 400 };
        civicPositions[CivicType::Urbanization] = { 1100, 300 };
        civicPositions[CivicType::Ideology] = { 1450, 400 };
        civicPositions[CivicType::Suffrage] = { 1650, 400 };
        civicPositions[CivicType::MassMedia] = { 1500, 300 };
        civicPositions[CivicType::Globalization] = { 1800, 350 };
        civicPositions[CivicType::SocialMedia] = { 2000, 350 };

        return true;
    }

    void ForceLandAt(int x, int y) {
        if (x >= 0 && x < map.GetWidth() && y >= 0 && y < map.GetHeight())
            map.GetTile(x, y).terrain = TerrainType::Plains;
    }

    void SpawnBarbarianCamp() {
        int x, y;
        int attempts = 0;
        do {
            x = rand() % map.GetWidth(); y = rand() % map.GetHeight();
            attempts++;
            if (abs(x - 3) + abs(y - 3) < 6) continue;
            Tile& t = map.GetTile(x, y);
            if (t.terrain != TerrainType::Water && t.improvement == ImprovementType::None && t.city == nullptr) {
                t.improvement = ImprovementType::BarbarianCamp;
                break;
            }
        } while (attempts < 50);
    }

    bool IsCoastal(int cx, int cy) {
        int dx[] = { 0, 0, 1, -1 }; int dy[] = { 1, -1, 0, 0 };
        for (int i = 0; i < 4; i++) {
            int nx = cx + dx[i]; int ny = cy + dy[i];
            if (nx >= 0 && nx < map.GetWidth() && ny >= 0 && ny < map.GetHeight()) {
                if (map.GetTile(nx, ny).terrain == TerrainType::Water) return true;
            }
        }
        return false;
    }

    // Helper: Counts Barbarians around a specific tile
    int CountNearbyBarbarians(int cx, int cy, int radius) {
        int count = 0;
        for (int y = cy - radius; y <= cy + radius; y++) {
            for (int x = cx - radius; x <= cx + radius; x++) {
                if (x >= 0 && x < map.GetWidth() && y >= 0 && y < map.GetHeight()) {
                    Unit* u = map.GetTile(x, y).unit;
                    if (u && u->ownerID == 3) count++;
                }
            }
        }
        return count;
    }

    // --- GAME LOGIC HELPERS ---

    int GetTileBonusProduction(int cx, int cy) {
        int bonus = 0;
        int dx[] = { 0, 0, 0, 1, -1 }; int dy[] = { 0, 1, -1, 0, 0 };
        for (int i = 0; i < 5; i++) {
            int nx = cx + dx[i]; int ny = cy + dy[i];
            if (nx >= 0 && nx < map.GetWidth() && ny >= 0 && ny < map.GetHeight()) {
                ImprovementType imp = map.GetTile(nx, ny).improvement;
                if (imp == ImprovementType::Mine) bonus += 2;
                bonus += GameData::Governments[empires[currentPlayer].currentGov].prodBonus;
            }
        }
        return bonus;
    }

    int GetTileBonusFood(int cx, int cy) {
        int bonus = 0;
        int dx[] = { 0, 0, 0, 1, -1 }; int dy[] = { 0, 1, -1, 0, 0 };
        for (int i = 0; i < 5; i++) {
            int nx = cx + dx[i]; int ny = cy + dy[i];
            if (nx >= 0 && nx < map.GetWidth() && ny >= 0 && ny < map.GetHeight()) {
                ImprovementType imp = map.GetTile(nx, ny).improvement;
                if (imp == ImprovementType::Farm) bonus += 2;
            }
        }
        return bonus;
    }

    void ResolveCombat(Unit* attacker, Tile* targetTile, bool isRanged) {
        auto& attStats = GameData::Units[attacker->type];
        if (targetTile->unit) {
            Unit* defender = targetTile->unit;
            auto& defStats = GameData::Units[defender->type];
            if (isRanged) {
                int dmg = std::max(10, attStats.combatStrength * 3 - defStats.combatStrength);
                defender->hp -= dmg;
            }
            else {
                int attDmg = std::max(5, attStats.combatStrength * 4 - defStats.combatStrength * 2);
                int defDmg = std::max(5, defStats.combatStrength * 4 - attStats.combatStrength * 2);
                defender->hp -= attDmg;
                attacker->hp -= defDmg;
            }
        }
        else if (targetTile->city && targetTile->city->ownerID != attacker->ownerID) {
            City* targetCity = targetTile->city;
            int cityDef = targetCity->GetDefenseStrength();
            if (isRanged) {
                int dmg = std::max(5, attStats.combatStrength * 2 - cityDef);
                targetCity->hp -= dmg;
            }
            else {
                int dmg = std::max(5, attStats.combatStrength * 3 - cityDef);
                int recoil = std::max(10, cityDef - attStats.combatStrength);
                targetCity->hp -= dmg;
                attacker->hp -= recoil;
                if (targetCity->hp <= 0) {
                    targetCity->hp = 50;
                    targetCity->ownerID = attacker->ownerID;
                    targetCity->color = attacker->color;
                    targetTile->unit = attacker;
                    map.GetTile(selectedX, selectedY).unit = nullptr;
                }
            }
        }
        attacker->currentMovement = 0;
    }

    void RemoveDeadUnits() {
        for (auto& tile : map.tiles) {
            if (tile.unit && tile.unit->hp <= 0) {
                if (selectedEntity == tile.unit) { selectedEntity = nullptr; selectedX = -1; selectedY = -1; }
                delete tile.unit;
                tile.unit = nullptr;
            }
        }
    }

    UnitType GetCurrentStrongestUnit() {
        if (empires[0].HasTech(TechType::Combustion)) return UnitType::Tank;
        if (empires[0].HasTech(TechType::Gunpowder)) return UnitType::Warrior;
        if (empires[0].HasTech(TechType::BronzeWorking)) return UnitType::Warrior;
        return UnitType::Warrior;
    }

    // --- BARBARIAN LOGIC (IMPROVED) ---
    void ProcessBarbarians() {
        // 1. Spawning (With Limit of 2 per village area)
        if (turnCount % 12 == 0) {
            for (int y = 0; y < map.GetHeight(); y++) {
                for (int x = 0; x < map.GetWidth(); x++) {
                    Tile& t = map.GetTile(x, y);
                    if (t.improvement == ImprovementType::BarbarianCamp) {
                        if (t.unit == nullptr) {
                            // CHECK LIMIT: Don't spawn if crowded
                            if (CountNearbyBarbarians(x, y, 4) < 2) {
                                UnitType type = GetCurrentStrongestUnit();
                                // Weaker barbarians (override type if needed, but we nerfed stats in Init)
                                t.unit = new Unit(olc::DARK_GREY, UnitType::BarbarianWarrior, 3);
                            }
                        }
                    }
                }
            }
        }

        // 2. Smart Movement
        for (int y = 0; y < map.GetHeight(); y++) {
            for (int x = 0; x < map.GetWidth(); x++) {
                Tile& t = map.GetTile(x, y);
                if (t.unit && t.unit->ownerID == 3 && !t.unit->hasMoved) {
                    Unit* u = t.unit;
                    u->hasMoved = true;

                    // Find nearest target (Player Unit or City)
                    int targetX = -1, targetY = -1;
                    int minDist = 999;
                    int scanRadius = 6; // Barbarian vision

                    for (int dy = -scanRadius; dy <= scanRadius; dy++) {
                        for (int dx = -scanRadius; dx <= scanRadius; dx++) {
                            int nx = x + dx; int ny = y + dy;
                            if (nx >= 0 && nx < map.GetWidth() && ny >= 0 && ny < map.GetHeight()) {
                                Tile& scanT = map.GetTile(nx, ny);
                                bool isEnemy = false;
                                if (scanT.unit && scanT.unit->ownerID != 3) isEnemy = true;
                                if (scanT.city && scanT.city->ownerID != 3) isEnemy = true;

                                if (isEnemy) {
                                    int dist = abs(x - nx) + abs(y - ny);
                                    if (dist < minDist) { minDist = dist; targetX = nx; targetY = ny; }
                                }
                            }
                        }
                    }

                    // Decide Move
                    int bestDx = 0, bestDy = 0;
                    if (targetX != -1) {
                        // Move towards target
                        if (targetX > x) bestDx = 1; else if (targetX < x) bestDx = -1;
                        if (targetY > y) bestDy = 1; else if (targetY < y) bestDy = -1;
                    }
                    else {
                        // Random Patrol if no target
                        bestDx = (rand() % 3) - 1;
                        bestDy = (rand() % 3) - 1;
                    }

                    // Try move/attack
                    int nx = x + bestDx; int ny = y + bestDy;

                    // Prioritize orthogonal movement if diagonal is blocked or for better pathing
                    if (bestDx != 0 && bestDy != 0 && (rand() % 2 == 0)) bestDy = 0;

                    if (nx >= 0 && nx < map.GetWidth() && ny >= 0 && ny < map.GetHeight()) {
                        Tile& target = map.GetTile(nx, ny);
                        if ((target.unit && target.unit->ownerID != 3) || (target.city && target.city->ownerID != 3)) {
                            ResolveCombat(u, &target, false);
                        }
                        else if (!target.unit && target.terrain != TerrainType::Mountain && target.terrain != TerrainType::Water) {
                            target.unit = u; t.unit = nullptr;
                            u->currentMovement = 0;
                        }
                    }
                }
            }
        }
        for (auto& tile : map.tiles) if (tile.unit && tile.unit->ownerID == 3) tile.unit->hasMoved = false;
    }

    void CheckWakeUp(Unit* u, int x, int y) {
        if (!u->isAlert) return;
        for (int dy = -1; dy <= 1; dy++) {
            for (int dx = -1; dx <= 1; dx++) {
                int nx = x + dx; int ny = y + dy;
                if (nx >= 0 && nx < map.GetWidth() && ny >= 0 && ny < map.GetHeight()) {
                    Tile& t = map.GetTile(nx, ny);
                    if (t.unit && t.unit->ownerID != 0) {
                        u->isAlert = false;
                        return;
                    }
                }
            }
        }
    }

    bool HasUnfinishedTasks() {
        if (currentPlayer != 0) return false;
        if (state == GameState::BuildingPlacement || state == GameState::RangedTargeting) return true;
        for (int i = 0; i < map.tiles.size(); i++) {
            Tile& t = map.tiles[i];
            if (t.unit && t.unit->ownerID == 0 && t.unit->currentMovement > 0 && !t.unit->isAlert) return true;
            if (t.city && t.city->ownerID == 0) {
                if (t.city->isProducingUnit == false && t.city->buildingQueue == BuildingType::None) return true;
            }
        }
        if (empires[0].currentResearch == TechType::None && empires[0].HasAvailableTechs()) return true;
        if (empires[0].currentCivic == CivicType::None && empires[0].HasAvailableCivics()) return true;
        return false;
    }

    void EndTurn() {
        selectedEntity = nullptr;
        selectedX = -1;
        selectedY = -1;
        state = GameState::Map;
        empires[currentPlayer].ProcessTurn();
        for (int i = 0; i < map.tiles.size(); i++) {
            Tile& tile = map.tiles[i];
            if (tile.city && tile.city->ownerID == currentPlayer) {
                std::vector<Unit*> createdUnits;
                int tx = i % map.GetWidth();
                int ty = i / map.GetWidth();
                int extraProd = GetTileBonusProduction(tx, ty);
                int extraFood = GetTileBonusFood(tx, ty);
                BuildingType queuedBuilding = tile.city->buildingQueue;
                bool isDistrictPlaced = tile.city->ProcessTurn(createdUnits, extraProd, extraFood);
                bool productionFinished = (queuedBuilding != BuildingType::None) &&
                    (tile.city->buildingQueue == BuildingType::None);
                if (isDistrictPlaced) {
                    if (tile.city->targetBuildX != -1) {
                        Tile& t = map.GetTile(tile.city->targetBuildX, tile.city->targetBuildY);
                        if (!tile.city->builtBuildings.empty()) {
                            t.externalBuilding = *tile.city->builtBuildings.rbegin();
                        }
                        t.ownerID = tile.city->ownerID;
                        t.pendingBuilding = BuildingType::None;
                        t.improvement = ImprovementType::None;
                        tile.city->targetBuildX = -1;
                    }
                }
                else if (productionFinished) {
                    BuildingType parent = GameData::Buildings[queuedBuilding].requiredParent;
                    if (parent != BuildingType::None) {
                        map.AddInternalBuilding(tile.city->ownerID, parent, queuedBuilding);
                    }
                }
                if (!createdUnits.empty()) {
                    if (tile.unit == nullptr) {
                        tile.unit = createdUnits[0];
                    }
                    else {
                        delete createdUnits[0];
                    }
                }
            }
            if (tile.unit && tile.unit->ownerID == currentPlayer) {
                tile.unit->ResetTurn();
            }
        }
        currentPlayer++;
        if (currentPlayer >= (int)empires.size()) {
            currentPlayer = 0;
            turnCount++;
            ProcessBarbarians();
            for (int y = 0; y < map.GetHeight(); y++) {
                for (int x = 0; x < map.GetWidth(); x++) {
                    Tile& t = map.GetTile(x, y);
                    if (t.unit && t.unit->ownerID == 0) CheckWakeUp(t.unit, x, y);
                }
            }
        }
        if (currentPlayer == 1) DoAITurn();
        else if (currentPlayer == 2) EndTurn();
    }

    void DoAITurn() {
        Empire& ai = empires[1];
        if (ai.currentResearch == TechType::None) ai.currentResearch = TechType::Pottery;
        if (ai.currentCivic == CivicType::None) ai.currentCivic = CivicType::CodeOfLaws;
        for (int y = 0; y < map.GetHeight(); y++) {
            for (int x = 0; x < map.GetWidth(); x++) {
                Tile& t = map.GetTile(x, y);
                if (t.unit && t.unit->ownerID == 1 && !t.unit->hasMoved) {
                    Unit* u = t.unit; u->hasMoved = true;
                    if (u->type == UnitType::Settler && t.city == nullptr) {
                        City* newCity = new City(GetNextCityName(), olc::RED, 1);
                        t.city = newCity; empires[1].cities.push_back(newCity);
                        delete u; t.unit = nullptr; continue;
                    }
                    if (u->currentMovement > 0) {
                        int nx = x + (rand() % 3) - 1; int ny = y + (rand() % 3) - 1;
                        if (nx >= 0 && nx < map.GetWidth() && ny >= 0 && ny < map.GetHeight()) {
                            Tile& target = map.GetTile(nx, ny);
                            if (target.unit && target.unit->ownerID == 0) ResolveCombat(u, &target, false);
                            else if (!target.unit && target.terrain != TerrainType::Mountain) { target.unit = u; t.unit = nullptr; u->currentMovement--; }
                        }
                    }
                }
                if (t.city && t.city->ownerID == 1) {
                    if (!t.city->isProducingUnit && t.city->buildingQueue == BuildingType::None) {
                        t.city->isProducingUnit = true; t.city->unitQueue = UnitType::Warrior;
                    }
                }
            }
        }
        RemoveDeadUnits();
        EndTurn();
    }

    std::string GetTechUnlocks(TechType t) {
        std::string s = "";
        for (auto const& [type, stats] : GameData::Units) if (stats.requiredTech == t) s += stats.name + ", ";
        for (auto const& [type, stats] : GameData::Buildings) if (stats.requiredTech == t) s += stats.name + ", ";
        if (!s.empty()) s.pop_back(); if (!s.empty()) s.pop_back();
        if (s.empty()) return "Nothing";
        return s;
    }

    bool OnUserUpdate(float fElapsedTime) override {
        Clear(olc::BLACK);
        if (turnCount > MAX_TURNS) {
            DrawString(ScreenWidth() / 2 - 80, ScreenHeight() / 2, "GAME OVER", olc::WHITE, 2); return true;
        }
        if (state == GameState::TechTree || state == GameState::CultureTree) {
            if (GetKey(olc::Key::RIGHT).bHeld) treeScrollX += 300.0f * fElapsedTime;
            if (GetKey(olc::Key::LEFT).bHeld) treeScrollX -= 300.0f * fElapsedTime;
            if (treeScrollX < 0) treeScrollX = 0;
            DrawString(10, ScreenHeight() - 20, "Use LEFT/RIGHT Arrow Keys to Scroll Tree", olc::WHITE);
        }
        else {
            treeScrollX = 0;
        }
        if (state == GameState::Map || state == GameState::BuildingPlacement || state == GameState::RangedTargeting) {
            map.Draw(this);
            if (state == GameState::RangedTargeting && selectedEntity) {
                Unit* u = (Unit*)selectedEntity; int range = GameData::Units[u->type].range;
                for (int dy = -range; dy <= range; dy++) for (int dx = -range; dx <= range; dx++)
                    if (abs(dx) + abs(dy) <= range && selectedX + dx >= 0 && selectedX + dx < map.GetWidth() && selectedY + dy >= 0 && selectedY + dy < map.GetHeight())
                        DrawRect((selectedX + dx) * 64, (selectedY + dy) * 64, 64, 64, olc::RED);
                DrawString(10, 40, "Select target (Left Click)...", olc::RED, 2);
            }
            if (state == GameState::BuildingPlacement && selectedEntity) {
                for (int dy = -2; dy <= 2; dy++) for (int dx = -2; dx <= 2; dx++)
                    if (selectedX + dx >= 0 && selectedX + dx < map.GetWidth() && selectedY + dy >= 0 && selectedY + dy < map.GetHeight())
                        DrawRect((selectedX + dx) * 64, (selectedY + dy) * 64, 64, 64, olc::CYAN);
                DrawString(10, 40, "Select tile for District...", olc::CYAN, 2);
            }
            int mx = GetMouseX() / 64;
            int my = GetMouseY() / 64;
            Tile* t = map.GetTileAtMouse(this);
            if (currentPlayer == 0 && GetMouse(1).bPressed && t) {
                if (state == GameState::Map && selectedEntity && dynamic_cast<Unit*>(selectedEntity)) {
                    Unit* u = (Unit*)selectedEntity;
                    int dist = abs(mx - selectedX) + abs(my - selectedY);
                    bool isWater = (t->terrain == TerrainType::Water);
                    bool isNaval = GameData::Units[u->type].isNaval;
                    bool canEnter = (isNaval && isWater) || (!isNaval && !isWater) || (t->city != nullptr);
                    if (dist == 1 && u->currentMovement > 0 && t->terrain != TerrainType::Mountain && canEnter) {
                        if (t->unit && t->unit->ownerID != 0) {
                            ResolveCombat(u, t, false); RemoveDeadUnits(); u->isAlert = false;
                        }
                        else if (t->city && t->city->ownerID != 0) {
                            ResolveCombat(u, t, false); RemoveDeadUnits(); u->isAlert = false;
                        }
                        else if (!t->unit) {
                            t->unit = u;
                            map.GetTile(selectedX, selectedY).unit = nullptr;
                            u->currentMovement--;
                            u->isAlert = false;
                            selectedX = mx; selectedY = my;
                            if (t->improvement == ImprovementType::BarbarianCamp) {
                                t->improvement = ImprovementType::None;
                                empires[0].gold += 50;
                            }
                        }
                    }
                }
                else if (state == GameState::BuildingPlacement || state == GameState::RangedTargeting) {
                    state = GameState::Map;
                    pendingBuilding = BuildingType::None;
                }
            }
            if (currentPlayer == 0 && GetMouse(0).bPressed) {
                if (GetMouseY() < ScreenHeight() - 140) {
                    if (state == GameState::RangedTargeting && t) {
                        Unit* u = (Unit*)selectedEntity;
                        int range = GameData::Units[u->type].range;
                        if (abs(mx - selectedX) + abs(my - selectedY) <= range && ((t->unit && t->unit->ownerID != 0) || (t->city && t->city->ownerID != 0))) {
                            ResolveCombat(u, t, true); RemoveDeadUnits(); u->isAlert = false; state = GameState::Map;
                        }
                        else state = GameState::Map;
                    }
                    else if (state == GameState::BuildingPlacement && t) {
                        if (abs(mx - selectedX) <= 2 && abs(my - selectedY) <= 2 && !t->city && t->externalBuilding == BuildingType::None && t->terrain != TerrainType::Water && t->terrain != TerrainType::Mountain) {
                            City* c = (City*)selectedEntity;
                            if (c->targetBuildX != -1) map.GetTile(c->targetBuildX, c->targetBuildY).pendingBuilding = BuildingType::None;
                            if (c->buildingQueue != pendingBuilding) c->productionStored = 0;
                            c->isProducingUnit = false; c->buildingQueue = pendingBuilding; c->targetBuildX = mx; c->targetBuildY = my;
                            t->pendingBuilding = pendingBuilding;
                            state = GameState::Map; pendingBuilding = BuildingType::None;
                        }
                        else { state = GameState::Map; pendingBuilding = BuildingType::None; }
                    }
                    else if (state == GameState::Map && t) {
                        selectedEntity = t->unit ? (Entity*)t->unit : (t->city ? (Entity*)t->city : nullptr);
                        selectedX = mx; selectedY = my;
                    }
                }
            }
            if (selectedX != -1) DrawRect(selectedX * 64, selectedY * 64, 64, 64, olc::WHITE);
            UI::DrawBottomPanel(this, selectedEntity);
            if (state == GameState::Map && selectedEntity) {
                if (Unit* u = dynamic_cast<Unit*>(selectedEntity)) {
                    if (u->ownerID == 0) {
                        if (u->isAlert) { if (UI::DrawButton(this, 180, ScreenHeight() - 60, "Wake Up (F)") || GetKey(olc::Key::F).bPressed) u->isAlert = false; }
                        else if (u->currentMovement > 0) { if (UI::DrawButton(this, 180, ScreenHeight() - 60, "Alert (F)") || GetKey(olc::Key::F).bPressed) { u->isAlert = true; selectedEntity = nullptr; selectedX = -1; } }
                        if (!u->isAlert && u->currentMovement > 0 && GameData::Units[u->type].range > 1) if (UI::DrawButton(this, 300, ScreenHeight() - 60, "Range Attack (R)") || GetKey(olc::Key::R).bPressed) state = GameState::RangedTargeting;
                        if (UI::DrawButton(this, 420, ScreenHeight() - 60, "Delete Unit (X)") || GetKey(olc::Key::X).bPressed) {
                            Tile& t = map.GetTile(selectedX, selectedY);
                            delete t.unit;
                            t.unit = nullptr;
                            selectedEntity = nullptr;
                            selectedX = -1;
                        }
                        if (u->type == UnitType::Settler) {
                            if (UI::DrawButton(this, 180, ScreenHeight() - 100, "Found City")) {
                                Tile& t = map.GetTile(selectedX, selectedY);
                                if (!t.city) {
                                    t.city = new City(GetNextCityName(), olc::BLUE, 0);
                                    empires[0].cities.push_back(t.city);
                                    delete t.unit; t.unit = nullptr;
                                    selectedEntity = nullptr; selectedX = -1;
                                }
                            }
                        }
                        if (u->type == UnitType::Builder) {
                            Tile& t = map.GetTile(selectedX, selectedY);
                            if (t.externalBuilding == BuildingType::None) {
                                if (t.terrain == TerrainType::Plains && t.improvement == ImprovementType::None) {
                                    if (UI::DrawButton(this, 180, ScreenHeight() - 100, "Build Farm")) { t.improvement = ImprovementType::Farm; delete t.unit; t.unit = nullptr; selectedEntity = nullptr; selectedX = -1; }
                                }
                                if ((t.terrain == TerrainType::Mountain || t.terrain == TerrainType::Forest) && t.improvement == ImprovementType::None) {
                                    if (UI::DrawButton(this, 180, ScreenHeight() - 100, "Build Mine")) { t.improvement = ImprovementType::Mine; delete t.unit; t.unit = nullptr; selectedEntity = nullptr; selectedX = -1; }
                                }
                            }
                            else {
                                DrawString(180, ScreenHeight() - 100, "Cannot build here (District)", olc::RED);
                            }
                        }
                    }
                }
                if (City* c = dynamic_cast<City*>(selectedEntity)) {
                    if (c->ownerID == 0) {
                        int startX = 280;
                        int startY = ScreenHeight() - 130;
                        int btnX = startX;
                        int btnY = startY;
                        int rowCount = 0;
                        DrawString(btnX, btnY - 10, "Produce:", olc::YELLOW);
                        for (auto const& [type, stats] : GameData::Units) {
                            if (type == UnitType::BarbarianWarrior) continue;
                            if (type == UnitType::Settler && c->population <= 2) continue;
                            bool locked = !empires[0].HasTech(stats.requiredTech);
                            if (stats.isNaval && !IsCoastal(selectedX, selectedY)) locked = true;
                            if (locked) continue;
                            if (UI::DrawButton(this, btnX, btnY, stats.name)) {
                                if (c->targetBuildX != -1) {
                                    map.GetTile(c->targetBuildX, c->targetBuildY).pendingBuilding = BuildingType::None;
                                    c->targetBuildX = -1;
                                }
                                c->isProducingUnit = true; c->unitQueue = type; c->productionStored = 0;
                            }
                            btnY += 30; rowCount++;
                            if (rowCount >= 4) { rowCount = 0; btnY = startY; btnX += 160; }
                        }
                        if (rowCount != 0) { rowCount = 0; btnY = startY; btnX += 160; }
                        DrawString(btnX, btnY - 10, "Buildings:", olc::YELLOW);
                        for (auto const& [type, stats] : GameData::Buildings) {
                            if (type == BuildingType::None) continue;
                            if (!empires[0].HasTech(stats.requiredTech) || !c->HasRequiredDistrict(type)) continue;
                            bool built = c->builtBuildings.count(type);
                            std::string name = stats.name + (built ? "(X)" : "");
                            if (UI::DrawButton(this, btnX, btnY, name)) {
                                if (!built) {
                                    if (stats.requiresTile) { state = GameState::BuildingPlacement; pendingBuilding = type; }
                                    else {
                                        if (c->targetBuildX != -1) {
                                            map.GetTile(c->targetBuildX, c->targetBuildY).pendingBuilding = BuildingType::None;
                                            c->targetBuildX = -1;
                                        }
                                        c->isProducingUnit = false; c->buildingQueue = type; c->productionStored = 0;
                                    }
                                }
                            }
                            if (built) DrawLine(btnX, btnY + 14, btnX + 150, btnY + 14, olc::GREEN);
                            btnY += 30; rowCount++;
                            if (rowCount >= 4) { rowCount = 0; btnY = startY; btnX += 160; }
                        }
                    }
                }
            }
        }
        if (state == GameState::TechTree) {
            Clear(olc::VERY_DARK_BLUE); DrawString(10, 10, "TECH TREE", olc::CYAN, 3);
            for (auto const& [type, stats] : GameData::Techs) {
                if (type == TechType::None || techPositions.find(type) == techPositions.end()) continue;
                if (stats.parent != TechType::None && techPositions.find(stats.parent) != techPositions.end()) {
                    olc::vi2d p1 = techPositions[stats.parent];
                    olc::vi2d p2 = techPositions[type];
                    DrawLine(p1.x + 150 - (int)treeScrollX, p1.y + 14, p2.x - (int)treeScrollX, p2.y + 14, olc::GREY);
                }
            }
            for (auto const& [type, stats] : GameData::Techs) {
                if (type == TechType::None || techPositions.find(type) == techPositions.end()) continue;
                olc::vi2d pos = techPositions[type];
                int drawX = pos.x - (int)treeScrollX;
                if (drawX < -200 || drawX > ScreenWidth()) continue;
                bool unlocked = empires[0].HasTech(type);
                bool available = (stats.parent == TechType::None) || empires[0].HasTech(stats.parent);
                bool active = (empires[0].currentResearch == type);
                int turns = 0;
                if (!unlocked) {
                    int sci = empires[0].GetTotalScienceYield();
                    int remaining = stats.scienceCost;
                    if (active) remaining -= empires[0].researchProgress;
                    if (sci <= 0) turns = 99;
                    else turns = (remaining + sci - 1) / sci;
                }
                if (available || unlocked) {
                    if (UI::DrawButton(this, drawX, pos.y, stats.name)) { if (!unlocked) empires[0].currentResearch = type; }
                }
                else {
                    DrawRect(drawX, pos.y, 150, 28, olc::GREY); DrawString(drawX + 10, pos.y + 8, stats.name, olc::GREY);
                }
                if (active) DrawRect(drawX - 2, pos.y - 2, 154, 32, olc::CYAN);
                DrawString(drawX, pos.y + 35, "Unlocks: " + GetTechUnlocks(type), olc::VERY_DARK_GREY, 1);
                DrawString(drawX, pos.y + 45, "Turns: " + std::to_string(turns), unlocked ? olc::GREEN : olc::YELLOW);
            }
        }
        if (state == GameState::CultureTree) {
            Clear(olc::Pixel(50, 0, 50)); DrawString(10, 10, "CIVICS TREE", olc::MAGENTA, 3);
            for (auto const& [type, stats] : GameData::Civics) {
                if (type == CivicType::None || civicPositions.find(type) == civicPositions.end()) continue;
                if (stats.parent != CivicType::None && civicPositions.find(stats.parent) != civicPositions.end()) {
                    olc::vi2d p1 = civicPositions[stats.parent];
                    olc::vi2d p2 = civicPositions[type];
                    DrawLine(p1.x + 150 - (int)treeScrollX, p1.y + 14, p2.x - (int)treeScrollX, p2.y + 14, olc::GREY);
                }
            }
            for (auto const& [type, stats] : GameData::Civics) {
                if (type == CivicType::None || civicPositions.find(type) == civicPositions.end()) continue;
                olc::vi2d pos = civicPositions[type];
                int drawX = pos.x - (int)treeScrollX;
                if (drawX < -200 || drawX > ScreenWidth()) continue;
                bool unlocked = empires[0].HasCivic(type);
                bool available = (stats.parent == CivicType::None) || empires[0].HasCivic(stats.parent);
                bool active = (empires[0].currentCivic == type);

                if (available || unlocked) {
                    if (UI::DrawButton(this, drawX, pos.y, stats.name)) { if (!unlocked) empires[0].currentCivic = type; }
                }
                else {
                    DrawRect(drawX, pos.y, 150, 28, olc::GREY); DrawString(drawX + 10, pos.y + 8, stats.name, olc::GREY);
                }
                if (active) DrawRect(drawX - 2, pos.y - 2, 154, 32, olc::MAGENTA);
                int turns = 0;
                if (!unlocked) {
                    int cul = empires[0].GetTotalCultureYield();
                    int remaining = stats.cultureCost;
                    if (active) remaining -= empires[0].cultureProgress;
                    if (cul <= 0) turns = 99;
                    else turns = (remaining + cul - 1) / cul;
                }
                DrawString(drawX, pos.y + 35, "Turns: " + std::to_string(turns), unlocked ? olc::GREEN : olc::YELLOW);
            }
        }
        if (state == GameState::Government) {
            Clear(olc::Pixel(80, 40, 0)); DrawString(10, 10, "GOVERNMENT", olc::WHITE, 3);
            int y = 80;
            for (auto const& [gov, stats] : GameData::Governments) {
                bool unlocked = empires[0].HasCivic(stats.requiredCivic);
                bool active = (empires[0].currentGov == gov);
                std::string label = stats.name + (active ? " (Active)" : (!unlocked ? " (Locked)" : ""));
                if (UI::DrawButton(this, 50, y, label)) {
                    if (unlocked && !active && (turnCount - empires[0].lastGovChangeTurn >= 10)) {
                        empires[0].currentGov = gov; empires[0].lastGovChangeTurn = turnCount;
                        Tile& capital = map.GetTile(3, 3);
                        if (capital.city) {
                            for (int dy = -1; dy <= 1; dy++) for (int dx = -1; dx <= 1; dx++)
                                if (3 + dx < map.GetWidth() && 3 + dy < map.GetHeight() && !map.GetTile(3 + dx, 3 + dy).unit && map.GetTile(3 + dx, 3 + dy).terrain != TerrainType::Water) {
                                    map.GetTile(3 + dx, 3 + dy).unit = new Unit(olc::BLUE, stats.freeUnit, 0); break;
                                }
                        }
                    }
                }
                DrawString(220, y + 5, "Bonus: Prod+" + std::to_string(stats.prodBonus) + " Sci+" + std::to_string(stats.sciBonus));
                DrawString(220, y + 20, "Req: " + GameData::Civics[stats.requiredCivic].name + " | Free: " + GameData::Units[stats.freeUnit].name);
                y += 60;
            }
        }
        UI::DrawButton(this, 10, 10, "Map"); if (GetMouse(0).bPressed && GetMouseX() < 160 && GetMouseY() < 40) state = GameState::Map;
        UI::DrawButton(this, 170, 10, "Science"); if (GetMouse(0).bPressed && GetMouseX() > 170 && GetMouseX() < 320 && GetMouseY() < 40) state = GameState::TechTree;
        UI::DrawButton(this, 330, 10, "Culture"); if (GetMouse(0).bPressed && GetMouseX() > 330 && GetMouseX() < 480 && GetMouseY() < 40) state = GameState::CultureTree;
        UI::DrawButton(this, 490, 10, "Gov"); if (GetMouse(0).bPressed && GetMouseX() > 490 && GetMouseX() < 580 && GetMouseY() < 40) state = GameState::Government;
        int infoX = 600;
        DrawString(infoX, 5, "Gold: " + std::to_string(empires[0].gold), olc::YELLOW);
        DrawString(infoX, 15, "Sci: +" + std::to_string(empires[0].GetTotalScienceYield()) + "  Cul: +" + std::to_string(empires[0].GetTotalCultureYield()), olc::WHITE);
        DrawString(infoX, 25, "Turn: " + std::to_string(turnCount) + "/500", olc::WHITE);
        if (currentPlayer == 0) {
            std::vector<std::string> tasks = GetTaskList();
            if (!tasks.empty()) {
                DrawString(ScreenWidth() - 320, 15, "UNFINISHED TASKS:", olc::RED, 2);
                int taskY = 35;
                for (const auto& task : tasks) {
                    DrawString(ScreenWidth() - 320, taskY, task, olc::Pixel(255, 150, 150));
                    taskY += 12;
                }
            }
            else if (UI::DrawButton(this, ScreenWidth() - 160, 10, "END TURN")) EndTurn();
        }
        else DrawString(ScreenWidth() - 150, 10, "AI THINKING...", olc::RED);
        return true;
    }
};

int main() {
    CivGame game;
    if (game.Construct(1024, 768, 1, 1)) game.Start();
    return 0;
}