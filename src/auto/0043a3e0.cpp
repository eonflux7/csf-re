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

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);
CSF_GLOBAL(float, g_RadiansToDegreesFactor, 0x00830CA4);
CSF_GLOBAL(float, g_DegreesToRadiansFactor, 0x00830CA8);
CSF_GLOBAL(float, g_NearZeroEpsilon, 0x00830CAC);

CSF_FUNC(uint32_t, __fastcall, GetSceneMenuContext, 0x0044FC60, (void* self, void* edx));
CSF_FUNC(double, __cdecl, MenuMpCharacterSelect_NormalizeAngle, 0x00406950, (float angleRadians));
CSF_FUNC(double, __cdecl, Minimap_StepAngleTowards, 0x0043AD70, (float currentDeg, float targetDeg, float* outDelta));
CSF_FUNC(char, __fastcall, HudClearSceneMenuPending, 0x0043A180, (void* self, void* edx));
CSF_FUNC(char, __fastcall, HudRefreshSceneMenuPanel, 0x0043A030, (void* self, void* edx));

// Vtable slot 0x38 of the scene menu context object: __thiscall(out buffer) -> float*
using SceneMenuContextAngleGetter = float* (__fastcall*)(void* self, void* edx, uint8_t* outBuf);

static char __fastcall HudApplySceneMenuContext(S_00439ba0* self, void* /*edx*/)
{
    char result = static_cast<char>(self->field_004d);
    if (result == 0)
        return result;

    // Scene menu context angle buffer: 8 bytes, holds the context out-value
    // at +0 and the HUD current angle copy at +4.
    alignas(8) uint8_t contextOutBytes[8];
    float* contextAngleOut = reinterpret_cast<float*>(contextOutBytes);
    float* currentAngleDeg = reinterpret_cast<float*>(contextOutBytes + 4);
    float deltaOut = 0.0f;

    uint32_t sceneMenuContext = GetSceneMenuContext(g_pCurrentScene, nullptr);
    void* contextObj = reinterpret_cast<void*>(sceneMenuContext);
    float* contextAngle = reinterpret_cast<SceneMenuContextAngleGetter>(CSF_AT(void**, contextObj, 0)[14])(contextObj, nullptr, contextOutBytes);
    float radiansAngle = *contextAngle * g_DegreesToRadiansFactor;

    *currentAngleDeg = self->field_0048;
    deltaOut = 0.0f;

    double normalizedRadians = MenuMpCharacterSelect_NormalizeAngle(radiansAngle);
    float targetDeg = static_cast<float>(normalizedRadians * g_RadiansToDegreesFactor);

    self->field_0048 = static_cast<float>(Minimap_StepAngleTowards(*currentAngleDeg, targetDeg, &deltaOut));

    if (std::fabs(-deltaOut) > g_NearZeroEpsilon || (result = static_cast<char>(self->field_004c)) != 0)
    {
        self->field_004c = 0;
        HudClearSceneMenuPending(self, nullptr);
        return HudRefreshSceneMenuPanel(self, nullptr);
    }
    return result;
}

HOOK(0x0043A3E0, HudApplySceneMenuContext, "auto");
