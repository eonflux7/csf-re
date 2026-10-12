#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

CSF_FUNC(uint32_t, __fastcall, CGuiControl_GetField012C, 0x0044F630, (CGuiControl* self, void*));
CSF_FUNC(int, __fastcall, SceneFindSubObject, 0x004DE7A0, (void* self, void* edx, unsigned int a2));
CSF_FUNC(int, __cdecl, AiTaskStates_IsDistanceWithin200, 0x0041E350, (S_004e0c20* a1, S_004e0c20* a2));
CSF_FUNC(char, __fastcall, IsGuardStateValidForTarget, 0x004AA310, (void* self, void* edx, int a2));

static int __fastcall AiCore_IsTargetPositionValid(S_004aa310* self, void* /*edx*/, int a2)
{
    (void)CSF_VTBL(0x004AA310);

    uint32_t sceneObject = CGuiControl_GetField012C(reinterpret_cast<CGuiControl*>(g_pCurrentScene), nullptr);
    int found = SceneFindSubObject(reinterpret_cast<void*>(static_cast<uintptr_t>(sceneObject)),
                                   nullptr,
                                   static_cast<unsigned int>(a2));
    S_004e0c20* subObject = reinterpret_cast<S_004e0c20*>(static_cast<uintptr_t>(static_cast<uint32_t>(found)));

    if (subObject == nullptr)
        return 0;
    if ((CSF_AT(uint8_t, subObject, 4) & 1) != 0)
        return 0;
    S_004e0c20* targetObject = reinterpret_cast<S_004e0c20*>(static_cast<uintptr_t>(static_cast<uint32_t>(a2)));
    if (AiTaskStates_IsDistanceWithin200(subObject, targetObject) == 0)
        return 0;
    if (IsGuardStateValidForTarget(self, nullptr, a2) == 0)
        return 0;
    return 1;
}

HOOK(0x004AEDD0, AiCore_IsTargetPositionValid, "auto");
