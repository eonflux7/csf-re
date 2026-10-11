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

CSF_FUNC(uint32_t, __fastcall, CGuiControl_GetField012C, 0x0044F630, (CGuiControl* self, void*));
CSF_FUNC(int, __fastcall, SceneFindSubObject, 0x004DE7A0, (void* self, void* edx, unsigned int a2));
CSF_FUNC(double, __fastcall, SubObjectDistanceToPoint, 0x004E0F60, (void* self, void* edx, float* a2));

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);
CSF_GLOBAL(float, g_Const200_0f, 0x0082DDEC);

static bool __cdecl IsCurrentSceneObjectInRange(S_004e0c20* a1)
{
    // Scene must exist and pass the GUI control check first.
    if (!CGuiControl_GetField012C(reinterpret_cast<CGuiControl*>(g_pCurrentScene), nullptr))
        return false;

    S_004e0c20* subObject = reinterpret_cast<S_004e0c20*>(
        SceneFindSubObject(g_pCurrentScene, nullptr, static_cast<uint32_t>(reinterpret_cast<uintptr_t>(a1))));

    if (!subObject)
        return false;

    double distance = SubObjectDistanceToPoint(
        subObject, nullptr, reinterpret_cast<float*>(a1->pad_0000));

    return std::fabs(distance) <= g_Const200_0f;
}

HOOK(0x41E380, IsCurrentSceneObjectInRange, "auto");
