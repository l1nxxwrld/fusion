#include "../misc/config.h"
#include "../../../../../ext/imgui/imgui.h"
#include "hud.h"
#include "../../../menu/menu.h"
#include <cmath> // For sin and cos functions
#include <vector>
#include <algorithm>

// Helper function to get a rainbow color based on vertical position and time
ImColor GetRainbowWaveColor(float verticalPosition, float time) {
    float waveFrequency = 6.0f; // Increased frequency for a faster color transition
    float colorOffset = 3.3f; // Offset to ensure only two waves are visible
    float r = 0.5f * (sin(waveFrequency * verticalPosition + time) + 1.0f); // Red component
    float g = 0.5f * (sin(waveFrequency * verticalPosition + time + colorOffset) + 1.0f); // Green component
    float b = 0.5f * (sin(waveFrequency * verticalPosition + time + 2.0f * colorOffset) + 1.0f); // Blue component
    return ImColor(r, g, b);
}

// Helper function to render text with shadow and rainbow wave effect
void RenderTextWithShadow(ImDrawList* drawList, ImFont* font, const char* mainText, const char* subText, float posX, float& yPos, float fontSize, float shadowOffset, float subTextGap, ImColor shadowColor, ImColor subTextColor) {
    ImVec2 mainTextSize = font->CalcTextSizeA(fontSize, FLT_MAX, 0.0f, mainText);
    ImVec2 subTextSize = font->CalcTextSizeA(fontSize, FLT_MAX, 0.0f, subText);

    // Calculate positions
    float mainTextPosX = posX - mainTextSize.x - subTextSize.x - subTextGap;
    float shadowPosX = mainTextPosX + shadowOffset;
    float shadowPosY = yPos + shadowOffset;

    // Render shadows
    drawList->AddText(font, fontSize, ImVec2(shadowPosX, shadowPosY), shadowColor, mainText);
    drawList->AddText(font, fontSize, ImVec2(shadowPosX + mainTextSize.x + subTextGap, shadowPosY), shadowColor, subText);

    // Render main text with vertical wave effect
    ImVec2 currentPos = ImVec2(mainTextPosX, yPos);
    for (const char* c = mainText; *c; ++c) {
        ImVec2 charSize = font->CalcTextSizeA(fontSize, FLT_MAX, 0.0f, c, c + 1);
        // Apply a phase offset based on the character's position
        float phaseOffset = fmod(currentPos.x, 2.0f * fontSize) / fontSize; // Creates two close waves
        ImColor rainbowColor = GetRainbowWaveColor(currentPos.y, ImGui::GetTime() * 10.0f); // Increase time factor for faster transition
        drawList->AddText(font, fontSize, currentPos, rainbowColor, c);
        currentPos.x += charSize.x; // Move to the next character position
    }

    // Render subtext
    drawList->AddText(font, fontSize, ImVec2(mainTextPosX + mainTextSize.x + subTextGap, yPos), subTextColor, subText);

    // Update y position for next line
    yPos += fontSize - 6; // Update yPos for next module
}

void Hud::RenderUpdate() {
    if (arraylist::enable)
    {
        // Texts and associated subtexts
        struct TextItem {
            const char* mainText;
            const char* subText;
            float length;
        };

        std::vector<TextItem> texts = {
            { "aimbot", "", 0.0f },
            { "clicker", "18", 0.0f },
            { "player esp", "2d", 0.0f },
            { "arraylist", "", 0.0f },
            { "delay remover", "", 0.0f },
            { "bridge assist", "matrix", 0.0f }
        };

        // Configuration
        float rightMargin = 2.0f; // Margin from the right edge
        float topMargin = 2.0f; // Initial margin from the top edge
        float font_size = 24.0f; // Font size
        float lineSpacing = 0.0f; // Spacing between lines set to 0
        float shadowOffset = 1.0f; // Offset for the shadow
        float subTextGap = 4.0f; // Gap between main text and subtext

        // Colors
        ImColor subTextColor(192, 192, 192); // Grayish
        ImColor shadowColor(0, 0, 0); // Static dark shadow color

        // Get screen size
        ImVec2 screenSize = ImGui::GetIO().DisplaySize;

        // Calculate lengths for sorting
        for (auto& textItem : texts) {
            textItem.length = Menu::Font->CalcTextSizeA(font_size, FLT_MAX, 0.0f, textItem.mainText).x + Menu::Font->CalcTextSizeA(font_size, FLT_MAX, 0.0f, textItem.subText).x + subTextGap;
        }

        // Sort texts by length (longest to shortest)
        std::sort(texts.begin(), texts.end(), [](const TextItem& a, const TextItem& b) {
            return a.length > b.length;
            });

        // Initial position
        float posX = screenSize.x - rightMargin; // X position from the right edge
        float posY = topMargin; // Initial Y position from the top edge

        // Render all elements
        for (const auto& textItem : texts) {
            RenderTextWithShadow(ImGui::GetWindowDrawList(), Menu::Font, textItem.mainText, textItem.subText, posX, posY, font_size, shadowOffset, subTextGap, shadowColor, subTextColor);
        }
    }
}
