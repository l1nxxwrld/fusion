#pragma once

#ifndef CONFIG_H
#define CONFIG_H

#include "../../ext/json/json.hpp"
#include "../../../util/math/geometry.h"

#include <filesystem>
#include <vector>
#include <string>

using json = nlohmann::json;


struct config
{
    static void RenderMenu();
};

struct esp {
    inline static bool Enabled = true;

    struct Data {
        std::vector<Vector3> boxVerticies;
        std::string name;
        std::string distText;
        float dist;
        float opacityFadeFactor;
        float health;
        float maxHealth;
    };

    inline static std::vector<Data> renderData;

    inline static bool Box = true;
    inline static float BoxColor[4]{ 0, 0, 0, 1 };

    inline static bool FilledBox = true;
    inline static bool SecondFilledBox = true;
    inline static float FilledBoxColor[4]{ 0, 0, 0, 0.15f }; // Include alpha value
    inline static float SecondFilledBoxColor[4]{ 0, 0, 0, 0.35f }; // Include alpha value


    inline static bool Outline = true;
    inline static float OutlineColor[4]{ 0, 0, 0, 0.25 };

    inline static bool Text = true;
    inline static float TextSize = 18;
    inline static float TextColor[4]{ 1.0f, 1.0f, 1.0f, 1.0f };
    inline static bool TextOutline = true;
    inline static float TextOutlineColor[4]{ 0, 0, 0, 1.0f };
    inline static float TextUnrenderDistance = 14.0f;

    inline static bool nickname = true;
    inline static bool distance = true;

    inline static float FadeDistance = 3.0f;

    inline static bool HealthBar = true;
};

// Aimbot Configuration
struct aimassist {
    inline static Vector3 renderData;

    inline static bool Enabled = false;

    inline static bool visibilityCheck = true;
    inline static bool aimAssistFeedback = true;
    inline static bool fovCircle = true;

    inline static Vector3 data;
    inline static bool pitchInfluenced = false;
    inline static bool aimKey = true;

    inline static bool adaptive = true;
    inline static float adaptiveOffset = 3;

    inline static float fov = 35.0f;
    inline static float smooth = 15.f;
    inline static float aimDistance = 4.f;
    inline static float randomYaw = 2;
    inline static float randomPitch = .075f;

    inline static int targetPriority = 2;
    inline static const char* targetPriorityList[3]{ "Distance", "Health", "Closest to Crosshair" };
};

// Reach Configuration
struct reach {
    inline static bool Enabled = false;
    inline static float ReachDistance = 3.0f;
};

// Clicker Configuration
struct clicker {
    inline static bool leftclicker = false;
    inline static float leftMaxCps = 14;
    inline static float leftMinCps = 8;
    inline static bool ignoreBlocks = true;

    inline static bool rightclicker = false;
    inline static float rightMaxCps = 20;
    inline static float rightMinCps = 15;
    inline static bool blocksOnly = true;
};

// Combined Configuration
struct Config {
    esp esp;
    aimassist aimbot;
    reach reach;
    clicker clicker;
};
extern Config currentConfig; // Declaration of the global variable

#endif // CONFIG_H