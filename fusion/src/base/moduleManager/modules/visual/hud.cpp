#define NOMINMAX
#include <windows.h>
#undef NOMINMAX

#include "../misc/config.h"
#include "../../../../../ext/imgui/imgui.h"
#include "hud.h"
#include "../../../menu/menu.h"
#include <cmath> // For sin and cos functions
#include <vector>
#include <iomanip>
#include <sstream>
#include <algorithm>

std::string RoundToString(float value, int decimalPlaces) {
    std::stringstream stream;
    stream << std::fixed << std::setprecision(decimalPlaces) << value;
    return stream.str();
}

// Helper function to get a rainbow color based on vertical position and time
ImColor waveColor(float verticalPosition, float time) {
    float waveFrequency = 6.0f; // Frequency for color transition
    float colorOffset = 2.7f; // Offset for hue shift
    float hueShift = sinf(waveFrequency * verticalPosition + time) * 0.2f; // Adjust the hue shift

    // Base pink color
    float baseR = 1.0f; // Red component
    float baseG = 0.5f; // Green component
    float baseB = 0.8f; // Blue component

    // Apply hue shift
    float r = baseR + hueShift;
    float g = baseG + hueShift;
    float b = baseB + hueShift;

    // Clamp values to [0, 1]
    r = std::max(0.0f, std::min(1.0f, r));
    g = std::max(0.0f, std::min(1.0f, g));
    b = std::max(0.0f, std::min(1.0f, b));

    return ImColor(r, g, b);
}

// Helper function to render text with shadow and rainbow wave effect
void renderTextShadow(ImDrawList* drawList, ImFont* font, const char* mainText, const char* subText, float posX, float& yPos, float fontSize, float shadowOffset, float subTextGap, ImColor shadowColor, ImColor subTextColor) {
    // Calculate text sizes
    ImVec2 mainTextSize = font->CalcTextSizeA(fontSize, FLT_MAX, 0.0f, mainText);
    ImVec2 subTextSize = font->CalcTextSizeA(fontSize, FLT_MAX, 0.0f, subText);

    // Calculate positions
    float mainTextPosX = posX - mainTextSize.x - subTextSize.x - subTextGap;
    float subTextPosX = mainTextPosX + mainTextSize.x + subTextGap;

    // Render shadow and main text for the main text
    drawList->AddText(font, fontSize, ImVec2(mainTextPosX + shadowOffset, yPos + shadowOffset), shadowColor, mainText);
    drawList->AddText(font, fontSize, ImVec2(mainTextPosX, yPos), waveColor(yPos, ImGui::GetTime() * 10.0f), mainText);

    // Render shadow and subtext if it exists
    if (subText[0] != '\0') {
        drawList->AddText(font, fontSize, ImVec2(subTextPosX + shadowOffset, yPos + shadowOffset), shadowColor, subText);
        drawList->AddText(font, fontSize, ImVec2(subTextPosX, yPos), subTextColor, subText);
    }

    // Update y position for the next line
    yPos += fontSize - 6; // Adjust this value as needed
}

void Hud::RenderUpdate() {
    if (arraylist::enable) {
        // Texts and associated subtexts
        struct TextItem {
            const char* mainText;
            const char* subText;
            float length;
        };

        // Create a vector to store text items with real values
        std::vector<TextItem> texts;

        // Helper lambda to add a text item with float value
        auto addTextItem = [&](const char* mainText, float value, std::vector<TextItem>& texts) {
            // Convert float value to string
            std::string valueStr = std::to_string(value);
            // Remove trailing zeroes and decimal point if not needed
            valueStr.erase(valueStr.find_last_not_of('0') + 1, std::string::npos);
            if (valueStr.back() == '.') valueStr.pop_back(); // Remove trailing decimal point if necessary
            // Add text item
            texts.push_back({ mainText, valueStr.c_str(), 0.0f });
            };

        // Add text items based on current configuration
        if (aimassist::Enabled) {
            texts.push_back({ "aimbot", "", 0.0f });
        }
        if (clicker::leftclicker) {
            addTextItem("left clicker", clicker::leftMaxCps, texts);
        }
        if (clicker::rightclicker) {
            addTextItem("right clicker", clicker::rightMaxCps, texts);
        }
        if (reach::Enabled) {
            addTextItem("reach", reach::ReachDistance, texts);
        }
        if (esp::Enabled) {
            texts.push_back({ "esp", "2d", 0.0f });
        }
        texts.push_back({ "arraylist", "", 0.0f }); // Always show arraylist

        // Configuration
        float rightMargin = 2.0f;
        float topMargin = 2.0f;
        float font_size = 24.0f;
        float shadowOffset = 1.0f;
        float subTextGap = 4.0f;

        // Colors
        ImColor subTextColor(192, 192, 192);
        ImColor shadowColor(0, 0, 0, 128);

        // Get screen size
        ImVec2 screenSize = ImGui::GetIO().DisplaySize;

        // Calculate lengths for sorting
        for (auto& textItem : texts) {
            textItem.length = Menu::FontBold->CalcTextSizeA(font_size, FLT_MAX, 0.0f, textItem.mainText).x + Menu::FontBold->CalcTextSizeA(font_size, FLT_MAX, 0.0f, textItem.subText).x + subTextGap;
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
            renderTextShadow(ImGui::GetWindowDrawList(), Menu::FontBold, textItem.mainText, textItem.subText, posX, posY, font_size, shadowOffset, subTextGap, shadowColor, subTextColor);
        }
    }
}