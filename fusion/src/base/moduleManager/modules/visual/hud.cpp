#include "../misc/config.h"
#include "../../../../../ext/imgui/imgui.h"
#include "hud.h"
#include "../../../menu/menu.h"

void Hud::RenderUpdate() {
    if (arraylist::enable)
    {
        // Texts and associated subtexts
        const char* watermark = "aimbot";
        const char* clicker = "clicker";
        const char* clickertps = "18";
        const char* esp = "player esp";
        const char* espmode = "2d";
        const char* arraylist = "arraylist";

        // Configuration
        float rightMargin = 2.0f; // Margin from the right edge
        float topMargin = 2.0f; // Initial margin from the top edge
        float font_size = 24.0f; // Font size
        float lineSpacing = 0.0f; // Spacing between lines set to 0
        float shadowOffset = 1.0f; // Offset for the shadow
        float subTextGap = 4.0f; // Gap between main text and subtext

        // Colors
        ImColor mainTextColor(204, 102, 255); // Lighter purple
        ImColor subTextColor(192, 192, 192); // Grayish
        ImColor shadowColor(0, 0, 0, 128); // Shadow color with some transparency

        // Get screen size
        ImVec2 screenSize = ImGui::GetIO().DisplaySize;

        // Initial position
        float posX = screenSize.x - rightMargin; // X position from the right edge
        float posY = topMargin; // Initial Y position from the top edge

        // Helper function to render text with a given main and subtext, and shadow
        auto renderTextWithShadow = [&](const char* mainText, const char* subText, float& yPos) {
            ImVec2 mainTextSize = Menu::Font->CalcTextSizeA(font_size, FLT_MAX, 0.0f, mainText);
            ImVec2 subTextSize = Menu::Font->CalcTextSizeA(font_size, FLT_MAX, 0.0f, subText);

            // Calculate positions
            float mainTextPosX = posX - mainTextSize.x - subTextSize.x - subTextGap - lineSpacing;
            float shadowPosX = mainTextPosX + shadowOffset;
            float shadowPosY = yPos + shadowOffset;

            // Render shadows
            ImGui::GetWindowDrawList()->AddText(Menu::Font, font_size, ImVec2(shadowPosX, shadowPosY), shadowColor, mainText);
            ImGui::GetWindowDrawList()->AddText(Menu::Font, font_size, ImVec2(shadowPosX + mainTextSize.x + subTextGap, shadowPosY), shadowColor, subText);

            // Render main text and subtext
            ImGui::GetWindowDrawList()->AddText(Menu::Font, font_size, ImVec2(mainTextPosX, yPos), mainTextColor, mainText);
            ImGui::GetWindowDrawList()->AddText(Menu::Font, font_size, ImVec2(mainTextPosX + mainTextSize.x + subTextGap, yPos), subTextColor, subText);

            // Update y position for next line
            yPos += font_size - 6; // Update yPos for next module
            };

        // Render "aimbot" with shadow
        ImVec2 watermarkSize = Menu::Font->CalcTextSizeA(font_size, FLT_MAX, 0.0f, watermark);
        ImGui::GetWindowDrawList()->AddText(Menu::Font, font_size, ImVec2(posX - watermarkSize.x + shadowOffset, topMargin + shadowOffset), shadowColor, watermark);
        ImGui::GetWindowDrawList()->AddText(Menu::Font, font_size, ImVec2(posX - watermarkSize.x, topMargin), mainTextColor, watermark);

        // Update y position for next line
        posY += font_size - 6;

        // Render "clicker" with "18" and shadow
        renderTextWithShadow(clicker, clickertps, posY);

        // Render "player esp" with "2d" and shadow
        renderTextWithShadow(esp, espmode, posY);

        // Render "arraylist" with shadow
        ImVec2 arrayListSize = Menu::Font->CalcTextSizeA(font_size, FLT_MAX, 0.0f, arraylist);
        ImGui::GetWindowDrawList()->AddText(Menu::Font, font_size, ImVec2(posX - arrayListSize.x + shadowOffset, posY + shadowOffset), shadowColor, arraylist);
        ImGui::GetWindowDrawList()->AddText(Menu::Font, font_size, ImVec2(posX - arrayListSize.x, posY), mainTextColor, arraylist);
    }
}
