#pragma once
#include "olcPixelGameEngine.h"
#include "Entity.h"
#include <string>

class UI {
public:
    static void DrawBottomPanel(olc::PixelGameEngine* pge, Entity* selected);
    static bool DrawButton(olc::PixelGameEngine* pge, int x, int y, const std::string& text);
};

static constexpr int PANEL_HEIGHT = 140;

void UI::DrawBottomPanel(olc::PixelGameEngine* pge, Entity* selected) {
    int screenW = pge->ScreenWidth();
    int screenH = pge->ScreenHeight();
    int panelY = screenH - PANEL_HEIGHT;
    pge->FillRect(0, panelY, screenW, PANEL_HEIGHT, olc::VERY_DARK_GREY);
    pge->DrawRect(0, panelY, screenW, PANEL_HEIGHT, olc::WHITE);
    pge->DrawString(10, panelY + 10, "INFO", olc::YELLOW, 2);
    if (selected) {
        selected->DrawInfo(pge, 10, panelY + 40);
    }
    else {
        pge->DrawString(10, panelY + 40, "Nothing selected");
    }
}

bool UI::DrawButton(olc::PixelGameEngine* pge, int x, int y, const std::string& text) {
    int w = 150;
    int h = 28;
    int mx = pge->GetMouseX();
    int my = pge->GetMouseY();
    bool hovered = mx >= x && mx < x + w && my >= y && my < y + h;
    olc::Pixel bg = hovered ? olc::DARK_BLUE : olc::BLUE;
    pge->FillRect(x, y, w, h, bg);
    pge->DrawRect(x, y, w, h, olc::WHITE);
    pge->DrawString(x + 5, y + 8, text, olc::WHITE);
    if (hovered && pge->GetMouse(0).bPressed) return true;
    return false;
}