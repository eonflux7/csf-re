#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// Callees (bound by address).
CSF_FUNC(uint32_t, __fastcall, GetSceneMenuContext, 0x0044FC60, (void* self, void* edx));
CSF_FUNC(double, __cdecl, MenuMpCharacterSelect_NormalizeAngle, 0x00406950, (float angleRadians));
CSF_FUNC(double, __cdecl, Minimap_StepAngleTowards, 0x0043AD70, (float currentDeg, float targetDeg, float* outDelta));
CSF_FUNC(char, __fastcall, HudClearSceneMenuPending, 0x0043A180, (void* self, void* edx));
CSF_FUNC(char, __fastcall, HudRefreshSceneMenuPanel, 0x0043A030, (void* self, void* edx));

// Data (bound by address).
CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);
CSF_GLOBAL(float, g_DegToRadFactor, 0x00830CA8);
CSF_GLOBAL(float, g_RadToDegFactor, 0x00830CA4);
CSF_GLOBAL(float, g_AngleDeltaEpsilon, 0x00830CAC);

static char __fastcall HudApplySceneMenuContext(S_00439ba0* self, void* /*edx*/)
{
    char result = static_cast<char>(self->field_004d);
    if (result)
    {
        // Read the current scene's menu context and its angle (slot 56 / 4 = 14).
        uint32_t sceneMenuContext = GetSceneMenuContext(g_pCurrentScene, nullptr);
        float angleOutBuf[2];
        float* angleSource = ((float* (__fastcall*)(void*, void*, float*))
            CSF_AT(void**, reinterpret_cast<void*>(sceneMenuContext), 0)[14])(
            reinterpret_cast<void*>(sceneMenuContext), nullptr, &angleOutBuf[0]);

        float radians = *angleSource * g_DegToRadFactor;
        MenuMpCharacterSelect_NormalizeAngle(radians);
        // x87 result of the normalize call is the value scaled to degrees.
        float targetDeg = static_cast<float>(
            MenuMpCharacterSelect_NormalizeAngle(radians) * g_RadToDegFactor);

        float currentDeg = self->field_0048;
        float angleDelta = 0.0f;
        self->field_0048 = static_cast<float>(
            Minimap_StepAngleTowards(currentDeg, targetDeg, &angleDelta));

        if (std::fabs(angleDelta) > g_AngleDeltaEpsilon || (result = static_cast<char>(self->field_004c)) != 0)
        {
            self->field_004c = 0;
            HudClearSceneMenuPending(self, nullptr);
            return HudRefreshSceneMenuPanel(self, nullptr);
        }
    }
    return result;
}

HOOK(0x0043A3E0, HudApplySceneMenuContext, "auto");
