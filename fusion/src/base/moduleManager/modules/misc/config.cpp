#include "config.h"

#include "../../../../../ext/imgui/imgui.h"
#include "../../../menu/menu.h"
#include "../../../base.h"
#include <filesystem>
#include <fstream>
#include <iostream>
#include <winhttp.h>
#include "configshare.h"
#pragma comment(lib, "winhttp.lib")

Config currentConfig;

namespace fs = std::filesystem;

std::vector<std::string> ListConfigFiles(const std::string& directory) {
    std::vector<std::string> configFiles;
    for (const auto& entry : std::filesystem::directory_iterator(directory)) {
        if (entry.is_regular_file() && entry.path().extension() == ".json") {
            configFiles.push_back(entry.path().stem().string()); // Save the file name without extension
        }
    }
    return configFiles;
}

void SaveConfig(const Config& config, const std::string& filename) {
    // Define the path to the directory and file
    fs::path directory = "C:/fusion";
    fs::path filePath = directory / (filename + ".json"); // Append ".json" to filename

    // Debugging output
    std::cout << "Saving config to: " << filePath << std::endl;

    // Ensure the directory exists
    if (!fs::exists(directory)) {
        try {
            fs::create_directories(directory);
            std::cout << "Directory created: " << directory << std::endl;
        }
        catch (const fs::filesystem_error& e) {
            std::cerr << "Failed to create directory: " << e.what() << std::endl;
            return;
        }
    }

    // Create the JSON object
    json j;
    j["esp"] = {
        {"Enabled", config.esp.Enabled},
        {"Box", config.esp.Box},
        {"BoxColor", {config.esp.BoxColor[0], config.esp.BoxColor[1], config.esp.BoxColor[2], config.esp.BoxColor[3]}},
        {"FilledBox", config.esp.FilledBox},
        {"FilledBoxColor", {config.esp.FilledBoxColor[0], config.esp.FilledBoxColor[1], config.esp.FilledBoxColor[2], config.esp.FilledBoxColor[3]}},
        {"SecondFilledBox", config.esp.SecondFilledBox},
        {"SecondFilledBoxColor", {config.esp.SecondFilledBoxColor[0], config.esp.SecondFilledBoxColor[1], config.esp.SecondFilledBoxColor[2], config.esp.SecondFilledBoxColor[3]}},
        {"HealthBar", config.esp.HealthBar},
        {"Text", config.esp.Text},
        {"nickname", config.esp.nickname},
        {"distance", config.esp.distance},
        {"TextOutline", config.esp.TextOutline},
        {"FadeDistance", config.esp.FadeDistance},
        {"TextSize", config.esp.TextSize}
    };

    j["aimbot"] = {
        {"Enabled", config.aimbot.Enabled},
        {"FOV", config.aimbot.fov},
        {"LockDistance", config.aimbot.aimDistance},
        {"Smoothness", config.aimbot.smooth},
        {"VisibilityCheck", config.aimbot.visibilityCheck},
        {"LeftButtonToAim", config.aimbot.aimKey},
        {"TargetPriority", config.aimbot.targetPriority},
        {"AdaptToStrafing", config.aimbot.adaptive},
        {"AdaptiveStrafingOffset", config.aimbot.adaptiveOffset},
        {"YawRandomness", config.aimbot.randomYaw},
        {"PitchRandomness", config.aimbot.randomPitch},
        {"FOVCircle", config.aimbot.fovCircle},
        {"FeedbackLine", config.aimbot.aimAssistFeedback}
    };

    j["reach"] = {
        {"Enabled", config.reach.Enabled},
        {"ReachDistance", config.reach.ReachDistance}
    };

    j["clicker"] = {
        {"LeftEnabled", config.clicker.leftclicker},
        {"LeftMinCps", config.clicker.leftMinCps},
        {"LeftMaxCps", config.clicker.leftMaxCps},
        {"LeftIgnoreBlocks", config.clicker.ignoreBlocks},
        {"RightEnabled", config.clicker.rightclicker},
        {"RightMinCps", config.clicker.rightMinCps},
        {"RightMaxCps", config.clicker.rightMaxCps},
        {"RightBlocksOnly", config.clicker.blocksOnly}
    };

    // Save the JSON to the file
    std::ofstream file(filePath);
    if (!file.is_open()) {
        std::cerr << "Failed to open file for writing: " << filePath << std::endl;
        return;
    }

    file << j.dump(4); // 4 is the indent level for readability
    file.close();
    std::cout << "Config saved successfully!" << std::endl;
}


Config LoadConfig(const std::string& filename) {
    std::ifstream file(filename);
    json j;
    file >> j;

    Config config; // Local variable

    // Load ESP settings
    auto& esp = j["esp"];
    config.esp.Enabled = esp["Enabled"].get<bool>();
    config.esp.Box = esp["Box"].get<bool>();
    auto boxColor = esp["BoxColor"].get<std::vector<float>>();
    std::copy(boxColor.begin(), boxColor.end(), config.esp.BoxColor);
    config.esp.FilledBox = esp["FilledBox"].get<bool>();
    auto filledBoxColor = esp["FilledBoxColor"].get<std::vector<float>>();
    std::copy(filledBoxColor.begin(), filledBoxColor.end(), config.esp.FilledBoxColor);
    config.esp.SecondFilledBox = esp["SecondFilledBox"].get<bool>();
    auto secondFilledBoxColor = esp["SecondFilledBoxColor"].get<std::vector<float>>();
    std::copy(secondFilledBoxColor.begin(), secondFilledBoxColor.end(), config.esp.SecondFilledBoxColor);
    config.esp.HealthBar = esp["HealthBar"].get<bool>();
    config.esp.Text = esp["Text"].get<bool>();
    config.esp.nickname = esp["nickname"].get<bool>();
    config.esp.distance = esp["distance"].get<bool>();
    config.esp.TextOutline = esp["TextOutline"].get<bool>();
    config.esp.FadeDistance = esp["FadeDistance"].get<float>();
    config.esp.TextSize = esp["TextSize"].get<float>();

    // Load Aimbot settings
    auto& aimbot = j["aimbot"];
    config.aimbot.Enabled = aimbot["Enabled"].get<bool>();
    config.aimbot.fov = aimbot["FOV"].get<float>();
    config.aimbot.aimDistance = aimbot["LockDistance"].get<float>();
    config.aimbot.smooth = aimbot["Smoothness"].get<float>();
    config.aimbot.visibilityCheck = aimbot["VisibilityCheck"].get<bool>();
    config.aimbot.aimKey = aimbot["LeftButtonToAim"].get<bool>();
    config.aimbot.targetPriority = aimbot["TargetPriority"].get<int>();
    config.aimbot.adaptive = aimbot["AdaptToStrafing"].get<bool>();
    config.aimbot.adaptiveOffset = aimbot["AdaptiveStrafingOffset"].get<float>();
    config.aimbot.randomYaw = aimbot["YawRandomness"].get<float>();
    config.aimbot.randomPitch = aimbot["PitchRandomness"].get<float>();
    config.aimbot.fovCircle = aimbot["FOVCircle"].get<bool>();
    config.aimbot.aimAssistFeedback = aimbot["FeedbackLine"].get<bool>();

    // Load Reach settings
    auto& reach = j["reach"];
    config.reach.Enabled = reach["Enabled"].get<bool>();
    config.reach.ReachDistance = reach["ReachDistance"].get<float>();

    // Load Clicker settings
    auto& clicker = j["clicker"];
    config.clicker.leftclicker = clicker["LeftEnabled"].get<bool>();
    config.clicker.leftMinCps = clicker["LeftMinCps"].get<float>();
    config.clicker.leftMaxCps = clicker["LeftMaxCps"].get<float>();
    config.clicker.ignoreBlocks = clicker["LeftIgnoreBlocks"].get<bool>();
    config.clicker.rightclicker = clicker["RightEnabled"].get<bool>();
    config.clicker.rightMinCps = clicker["RightMinCps"].get<float>();
    config.clicker.rightMaxCps = clicker["RightMaxCps"].get<float>();
    config.clicker.blocksOnly = clicker["RightBlocksOnly"].get<bool>();
}


void config::RenderMenu() {
    static char configName[256] = "default"; // Input field for config name
    static std::vector<std::string> configFiles = ListConfigFiles("C:/fusion"); // List of configs
    static std::string selectedConfig; // Currently selected config name
    static int selectedIndex = -1; // Track selected index in the list
    static std::string shareURL; // URL for shared config
    static bool shareURLVisible = false; // Flag to control the visibility of the URL input text

    ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 20);
    ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.12f, 0.12f, 0.12f, 0.5));
    ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 10);

    if (ImGui::BeginChild("ConfigContainer", ImVec2(500, 330))) {
        ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 3);

        // Display list of available configurations
        if (ImGui::BeginChild("ConfigList", ImVec2(150, 170), true)) {
            for (int i = 0; i < configFiles.size(); ++i) {
                bool isSelected = (i == selectedIndex);
                if (ImGui::Selectable(configFiles[i].c_str(), isSelected)) {
                    selectedIndex = i;
                    selectedConfig = configFiles[i];
                    strncpy_s(configName, sizeof(configName), configFiles[i].c_str(), _TRUNCATE); // Update the input field
                }
            }
            ImGui::EndChild();
        }

        ImGui::SameLine();

        // Action buttons in the same row as the list box
        ImGui::BeginGroup();
        if (ImGui::Button("Save Config")) {
            SaveConfig(currentConfig, configName); // Just use configName, it will append ".json" in SaveConfig
            configFiles = ListConfigFiles("C:/fusion"); // Refresh the list
            selectedIndex = -1; // Reset selection
        }
        ImGui::SameLine();
        if (ImGui::Button("Load Config")) {
            if (!selectedConfig.empty()) {
                currentConfig = LoadConfig("C:/fusion/" + selectedConfig + ".json");
            }
        }
        ImGui::SameLine();
        if (ImGui::Button("Delete Config")) {
            if (!selectedConfig.empty()) {
                std::filesystem::remove("C:/fusion/" + selectedConfig + ".json");
                configFiles = ListConfigFiles("C:/fusion"); // Refresh the list
                selectedConfig.clear(); // Clear the selected config
                strncpy_s(configName, sizeof(configName), "default", _TRUNCATE); // Reset the input field
            }
        }
        if (ImGui::Button("Share Config")) {
            if (!selectedConfig.empty()) {
                std::cout << "Share button clicked, uploading config: " << selectedConfig << std::endl;
                shareURL = cfgshare::UploadConfig("C:/fusion/" + selectedConfig + ".json");
                std::cout << "Share URL: " << shareURL << std::endl;
                shareURLVisible = true; // Set the flag to true to display the URL
            }
        }
        ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 8);
        if (shareURLVisible) {
            ImGui::InputText("Shared URL", (char*)shareURL.c_str(), shareURL.size() + 1, ImGuiInputTextFlags_ReadOnly);
        }


        ImGui::EndGroup();

        ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10);

        // Input Text for configuration name
        ImGui::InputText("Config Name", configName, IM_ARRAYSIZE(configName));

        // Display the shared URL if available
        // Detach button
        if (ImGui::Button("Detach")) {
            Base::Running = false;
        }

        ImGui::EndChild();
    }

    ImGui::PopStyleVar();
    ImGui::PopStyleColor();
}



