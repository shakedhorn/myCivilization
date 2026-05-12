#pragma once
#include <string>
#include <vector>
#include <map>
#include <set>
#include "olcPixelGameEngine.h"

// --- Enums ---
enum class UnitType { Settler, Builder, Warrior, Horseman, Tank, Galley, Ship, Archer, BarbarianWarrior };
enum class BuildingType { None, Monument, Granary, Walls, Campus, Library, TheaterSquare, Amphitheater, Workshop };
enum class ImprovementType { None, Farm, Mine, BarbarianCamp };

// TechType Expanded - Including Drama and many new ones
enum class TechType {
    None,
    // Ancient
    Pottery, Mining, AnimalHusbandry, Sailing,
    // Classical
    Archery, BronzeWorking, Masonry, Writing, Wheel, Drama,
    // Medieval
    Mathematics, Engineering, IronWorking, HorsebackRiding, Currency, Construction,
    // Renaissance
    Gunpowder, Printing, Astronomy, Banking, Navigation,
    // Industrial
    Combustion, Industrialization, SteamPower, Sanitation,
    // Modern
    Flight, Radio, Plastics, Computers,
    // Future
    Robotics, ArtificialIntelligence, FutureTech
};

// CivicType Expanded
enum class CivicType {
    None,
    // Ancient
    CodeOfLaws, Craftsmanship, ForeignTrade, Mysticism,
    // Classical
    MilitaryTradition, EarlyEmpire, StateWorkforce,
    // Medieval
    Feudalism, CivilService, Guilds, Theology,
    // Renaissance
    Nationalism, Humanism, DiplomaticService,
    // Industrial
    Urbanization, Mercantilism,
    // Modern
    Suffrage, MassMedia, Ideology,
    // Future
    Globalization, SocialMedia
};

enum class TerrainType { Plains, Forest, Mountain, Water, Desert };
enum class GovernmentType { Despotism, Monarchy, Republic, Democracy };

struct UnitStats {
    std::string name = "";
    int movement = 0;
    int combatStrength = 0;
    int range = 0;
    int productionCost = 0;
    TechType requiredTech = TechType::None;
    bool isNaval = false;
};

struct BuildingStats {
    std::string name = "";
    int productionCost = 0;
    TechType requiredTech = TechType::None;
    int scienceBonus = 0;
    int cultureBonus = 0;
    int productionBonus = 0;
    bool requiresTile = false;
    BuildingType requiredParent = BuildingType::None;
};

struct TechStats {
    std::string name = "";
    int scienceCost = 0;
    TechType parent = TechType::None;
};

struct CivicStats {
    std::string name = "";
    int cultureCost = 0;
	CivicType parent = CivicType::None;
};

struct GovernmentStats {
    std::string name = "";
    int sciBonus = 0;
    int culBonus = 0;
    int prodBonus = 0;
	UnitType freeUnit = UnitType::Settler;
	CivicType requiredCivic = CivicType::None;
};

class GameData {
public:
    static std::map<UnitType, UnitStats> Units;
    static std::map<BuildingType, BuildingStats> Buildings;
    static std::map<TechType, TechStats> Techs;
    static std::map<CivicType, CivicStats> Civics;
    static std::map<GovernmentType, GovernmentStats> Governments;
    static void Init();
};