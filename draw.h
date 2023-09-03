//
// Created by jerrymk on 09/03/2023.
//

#ifndef KLIPPER_12X4_DISPLAY_DESIGN_TOOL_DRAW_H
#define KLIPPER_12X4_DISPLAY_DESIGN_TOOL_DRAW_H

#include <SFML/Graphics.hpp>
#include "klipper_font.h"
#include <string>

#define PIXEL_SIZE 16
#define DISPLAY_WIDTH 128
#define DISPLAY_HEIGHT 64
#define CHARACTER_WIDTH 8
#define CHARACTER_HEIGHT 16
#define DISPLAY_OFFSET 0

void drawRect(sf::VertexArray& va, int x, int y, int w, int h, uint32_t color, int scale = PIXEL_SIZE, int border = 1) {
    va.append({sf::Vector2f(x * scale, y * scale), sf::Color(color)});
    va.append({sf::Vector2f((x + w) * scale - border, (y) * scale), sf::Color(color)});
    va.append({sf::Vector2f((x + w) * scale - border, (y + h) * scale - border), sf::Color(color)});
    va.append({sf::Vector2f((x) * scale, (y + h) * scale - border), sf::Color(color)});
}

void drawChar(sf::VertexArray& va, char ch, int x, int y, uint32_t color, int scale = PIXEL_SIZE, int border = 1) {
    for (auto r: font[ch]) {
        for (int i = 0; i < 8; i++) {
            if (r & 0b10000000)
                drawRect(va, x + i, y, 1, 1, color, scale, border);
            r <<= 1;
        }
        y++;
    }
}

void drawString(sf::VertexArray& va, const std::string& str, int x, int y, uint32_t color, int scale = PIXEL_SIZE, int border = 1) {
    for (auto c: str) {
        drawChar(va, c, x, y, color, scale, border);
        x += CHARACTER_WIDTH;
    }
}

#endif //KLIPPER_12X4_DISPLAY_DESIGN_TOOL_DRAW_H
