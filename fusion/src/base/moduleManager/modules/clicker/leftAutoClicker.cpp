#include "leftAutoClicker.h"
#include "../../../sdk/sdk.h"
#include "../../../menu/menu.h"
#include "../../../util/logger.h"
#include "../../../menu/menu.h"
#include "../misc/config.h"


#include <chrono>
#include <random>

long lastClickTime = 0;
int nextCps = 10;

void LeftAutoClicker::Update()
{
    if (!clicker::leftclicker) return;
    if (Menu::Open) return;
    if (SDK::Minecraft->IsInGuiState()) return;
    if (clicker::ignoreBlocks && SDK::Minecraft->GetMouseOver().IsTypeOfBlock()) return;

    long milli = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    if (lastClickTime == 0) lastClickTime = milli;
    if ((milli - lastClickTime) < (1000 / nextCps)) return;

    if (GetAsyncKeyState(VK_LBUTTON) && 1) {
        POINT pos_cursor;
        GetCursorPos(&pos_cursor);
        SendMessage(Menu::HandleWindow, WM_LBUTTONDOWN, MK_LBUTTON, MAKELPARAM(pos_cursor.x, pos_cursor.y));
        SendMessage(Menu::HandleWindow, WM_LBUTTONUP, 0, MAKELPARAM(pos_cursor.x, pos_cursor.y));

        // Block hit logic
        if (clicker::blockHit) {
            std::random_device rd;
            std::mt19937 gen(rd());
            std::uniform_real_distribution<> distrib(0.0, 100.0);
            if (distrib(gen) <= clicker::blockHitChance) {
                // Perform block hit action
                SendMessage(Menu::HandleWindow, WM_RBUTTONDOWN, MK_RBUTTON, MAKELPARAM(pos_cursor.x, pos_cursor.y));
                SendMessage(Menu::HandleWindow, WM_RBUTTONUP, 0, MAKELPARAM(pos_cursor.x, pos_cursor.y));
            }
        }

        lastClickTime = milli;

        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> distrib(clicker::leftMinCps, clicker::leftMaxCps);
        nextCps = distrib(gen);
    }
}

void LeftAutoClicker::RenderMenu()
{
    ImGui::BeginGroup();
    ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 20);
    ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.12f, 0.12f, 0.12f, 0.5));
    ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 10);
    if (ImGui::BeginChild("autoclicker", ImVec2(425, 150))) { // Adjust height as needed
        ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 3);
        Menu::DoToggleButtonStuff(857834, "Toggle Left Auto Clicker", &clicker::leftclicker);
        ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 5);
        ImGui::Separator();
        Menu::DoSliderStuff(3280, "Min CPS", &clicker::leftMinCps, 1, clicker::leftMaxCps);
        Menu::DoSliderStuff(675, "Max CPS", &clicker::leftMaxCps, clicker::leftMinCps, 20);
        Menu::DoToggleButtonStuff(2136, "Ignore Blocks", &clicker::ignoreBlocks);

        ImGui::Separator();
        Menu::DoToggleButtonStuff(2145, "Enable Block Hit", &clicker::blockHit); // New toggle for block hit
        Menu::DoSliderStuff(2146, "Block Hit Chance (%)", &clicker::blockHitChance, 0, 100); // Slider for block hit chance

        ImGui::EndChild();
    }
    ImGui::PopStyleVar();
    ImGui::PopStyleColor();
    ImGui::EndGroup();
}
