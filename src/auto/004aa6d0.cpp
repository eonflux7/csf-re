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
CSF_GLOBAL(float, g_AxisY, 0x008A7D78);
CSF_GLOBAL(float, g_AxisY_Y, 0x008A7D7C);
CSF_GLOBAL(float, g_AxisY_Z, 0x008A7D80);
CSF_GLOBAL(float, g_RayPlaneNoHitValue, 0x00835444);

CSF_FUNC(uint32_t, __fastcall, CGuiControl_GetField012C, 0x0044F630, (CGuiControl* self, void*));
CSF_FUNC(int, __fastcall, SceneFindSubObject, 0x004DE7A0, (void* self, void*, unsigned int a2));
CSF_FUNC(double, __fastcall, S_004e0c20_GetRayPlaneHitT, 0x004E0D70, (void* self, void* edx, uint32_t* a2, int a3));

// Moves the object a1 vertically (field at +4) so it sits on the scene surface
// hit by a ray cast along the negated Y axis from the sub-object found in the current scene.
static void __stdcall AiCore_AdjustPositionToSceneSurface(S_004e0c20* a1)
{
    uint32_t sceneObjectRaw = CGuiControl_GetField012C(reinterpret_cast<CGuiControl*>(g_pCurrentScene), nullptr);

    int subObjectRaw = SceneFindSubObject(
        reinterpret_cast<void*>(static_cast<uintptr_t>(sceneObjectRaw)),
        nullptr,
        static_cast<unsigned int>(reinterpret_cast<uintptr_t>(a1)));

    if (subObjectRaw == 0)
        return;

    S_004e0c20* subObject = reinterpret_cast<S_004e0c20*>(static_cast<intptr_t>(subObjectRaw));

    float rayDirection[3];
    rayDirection[0] = -g_AxisY;
    rayDirection[1] = -g_AxisY_Y;
    rayDirection[2] = -g_AxisY_Z;

    double rayPlaneHitT = S_004e0c20_GetRayPlaneHitT(
        subObject,
        nullptr,
        reinterpret_cast<uint32_t*>(a1),
        static_cast<int>(reinterpret_cast<uintptr_t>(rayDirection)));

    if (rayPlaneHitT != static_cast<double>(g_RayPlaneNoHitValue))
        CSF_AT(float, a1, 4) = CSF_AT(float, a1, 4) - static_cast<float>(rayPlaneHitT);
}

HOOK(0x4AA6D0, AiCore_AdjustPositionToSceneSurface, "auto");
