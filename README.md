# myCivilization — C++ 2D Turn-Based Strategy Game

A lightweight, turn-based 4X strategy game built in C++ using the **olcPixelGameEngine**. Inspired by the classic *Civilization* series, the game includes map generation, empire management, technology/civic progression systems, tactical combat, and dynamic AI/Barbarian behavior.

## Features

- **Procedural World Grid:** Tile-based map featuring distinct terrain types (Plains, Forests, Mountains, Deserts, and Oceans) that alter resource yields and unit pathing.
- **Empire & City Management:** Found cities with Settlers, construct key improvements (Farms, Mines) using Builders, manage dynamic local population growth, and queue production targets.
- **District-Based Building Tree:** Build Specialized Districts (Campus, Theater Square, Workshop) directly onto physical map tiles, unlocking specialized infrastructure hierarchies (e.g., Library inside a Campus).
- **Dual Tech & Civics Trees:** Advance an empire's structural capabilities by investing accrued Turn Yields into an expansive interactive Science/Technology tree and a Culture/Civics tree.
- **Government Systems:** Transition between state structures (Despotism, Monarchy, Republic, Democracy) to capture unique passive stat multipliers and unlock specialized free units.
- **Tactical Combat Loop:** Control various military unit classes (Warriors, Archers, Horsemen, Tanks, and Naval Vessels) to clear Barbarian outposts, siege enemy city defense walls, and leverage Ranged Target interfaces.
- **Automated Turn Assistants:** Dynamic task ledger indexing engine actions to safeguard structural oversight (e.g., parsing idle production capacities or unit deployment configurations).

## Project Architecture

The repository partitions game components across an object-oriented structural layout:

- `main.cpp`: Orchestrates the main engine pipeline (`OnUserCreate`, `OnUserUpdate`), global data mapping dictionaries, explicit keyboard/mouse interaction states, and core end-turn cycle configurations.
- `GameDefs.h`: Contains critical enumeration sets (`UnitType`, `BuildingType`, `TechType`, `CivicType`, `TerrainType`) alongside data specification blueprints for stats and structural rewards.
- `Entity.h`: Defines the core pure abstract rendering interface base class used to manage map projections and interface visualization feeds.
- `Empire.h`: Tracks faction-wide state statistics including aggregated science/culture loops, research progress gates, unlocked upgrades, and structural yield calculations.
- `City.h` / `City.cpp`: Governs specific settlement metrics, combat tracking, turn processing math (food metrics, infrastructure thresholds), and internal layout parsing.
- `Map.h`: Houses the 2D matrix structure array, world instantiation randomizers, and multi-tier grid building insertion wrappers.
- `Tile.h`: Tracks standalone tile state parameters, active improvements, district configurations, ownership signatures, and asset rendering logic.
- `Unit.h`: Manages mobile pieces, combat statistics, functional turn refreshes, movement gauges, and operational alerts.
- `UI.h`: Standardizes drawing controls for multi-purpose action buttons, overlay configurations, and bottom statistical insight layouts.

## Gameplay Controls

### Map Operations
- **Left-Click (Grid View):** Select a specialized unit, settlement, or individual tile target.
- **Right-Click (Tile Range):** Order a selected unit to move or attack an adjacent hostile target.
- **`F` Key:** Toggle selected unit state between active patrol and "Alert Mode" (remains asleep until hostiles breach immediate visibility radius).
- **`X` Key:** Disband/Delete the selected active unit.
- **`R` Key:** Initialize ranged targeting sequence for capable units (Archers, Battleships).

### Tech & Civics Tree Projections
- **Interactive Buttons (Header Bar):** Seamlessly transition rendering screens between the main tactical map, Technology Grid, Civic Hierarchy, or Government Interface.
- **Left / Right Arrow Keys:** Smoothly horizontal-scroll across expanded timeline progression pathways.
- **Left-Click (Node Matrix):** Designate an unlocked node to focus targeted scientific/cultural generation efforts.

## Compilation Guide

This project depends on the **olcPixelGameEngine** header and proper system-level hardware render APIs.

### Setup Prerequisites
1. Download `olcPixelGameEngine.h` and ensure it sits alongside the project source files.
2. Ensure you have working system graphic device dependencies installed (e.g., standard OpenGL framework runtimes).

### Using MinGW / G++ (Command Line)
Execute the following string inside your terminal environment to link core dependencies:
```bash
g++ -o myCivilization main.cpp City.cpp -luser32 -lgdi32 -lopengl32 -lgdiplus -lShlwapi -std=c++17
