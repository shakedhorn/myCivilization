#pragma once
#include "GameDefs.h"
#include "City.h"
#include <set>
#include <vector>

class Empire {
public:
    int ownerID;
    olc::Pixel color;
    std::vector<City*> cities;
    int gold = 0;
    int science = 0;
    int culture = 0;

    // Tech
    TechType currentResearch = TechType::None;
    int researchProgress = 0;
    std::set<TechType> unlockedTechs;

    // Culture
    CivicType currentCivic = CivicType::None;
    int cultureProgress = 0;
    std::set<CivicType> unlockedCivics;

    // Government
    GovernmentType currentGov = GovernmentType::Despotism;
    int lastGovChangeTurn = -999;

    Empire(int id, olc::Pixel c) : ownerID(id), color(c) {}

    bool HasTech(TechType t) {
        if (t == TechType::None) return true;
        return unlockedTechs.count(t) > 0;
    }

    bool HasCivic(CivicType c) {
        if (c == CivicType::None) return true;
        return unlockedCivics.count(c) > 0;
    }

    // --- FIX: Check if tree is finished ---
    bool HasAvailableTechs() {
        // -1 because None is in the map but cannot be researched
        return unlockedTechs.size() < (GameData::Techs.size() - 1);
    }

    bool HasAvailableCivics() {
        return unlockedCivics.size() < (GameData::Civics.size() - 1);
    }

    int GetTotalScienceYield() {
        int yield = 0;
        int govBonus = GameData::Governments[currentGov].sciBonus;
        for (auto c : cities) {
            yield += c->GetScienceYield() + govBonus;
        }
        return yield;
    }

    int GetTotalCultureYield() {
        int yield = 0;
        int govBonus = GameData::Governments[currentGov].culBonus;
        for (auto c : cities) {
            yield += c->GetCultureYield() + govBonus;
        }
        return yield;
    }

    void ProcessTurn() {
        int sciencePerTurn = GetTotalScienceYield();
        int culturePerTurn = GetTotalCultureYield();

        science += sciencePerTurn;
        culture += culturePerTurn;

        if (currentResearch != TechType::None) {
            researchProgress += sciencePerTurn;
            if (researchProgress >= GameData::Techs[currentResearch].scienceCost) {
                unlockedTechs.insert(currentResearch);
                currentResearch = TechType::None;
                researchProgress = 0;
            }
        }

        if (currentCivic != CivicType::None) {
            cultureProgress += culturePerTurn;
            if (cultureProgress >= GameData::Civics[currentCivic].cultureCost) {
                unlockedCivics.insert(currentCivic);
                currentCivic = CivicType::None;
                cultureProgress = 0;
            }
        }
    }
};