#include "base.h"
#include "sdk/sdk.h"

#include "moduleManager/modules/visual/esp.h"
#include "moduleManager/modules/combat/aimAssist.h"

#include "../../ext/imgui/imgui.h"
#include "menu/menu.h"
#include "moduleManager/modules/visual/hud.h"

void Base::RenderLoop() // Runs every frame
{
	if (!Base::Running) return;
	Esp::RenderUpdate();
	AimAssist::RenderUpdate();
	Hud::RenderUpdate();
}