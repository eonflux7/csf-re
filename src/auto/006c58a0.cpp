#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, GetSkinTargetObject, 0x00681FB0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, GetSceneRegionAt, 0x004FC130, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, C_008670f0_SetField0C, 0x006C4620, (C_008670f0* self, void* edx, int value));

// Resolves the skin target from the stack argument, fetches the scene region
// for this->field_0004 (index 12), then forwards it to the target's SetField0C.
static int __fastcall C_008656d0_SetSkinRegionFromTarget(C_008656d0* self, void* /*edx*/, C_008656d0* targetArg)
{
    int targetObject = GetSkinTargetObject(targetArg, nullptr);
    int sceneRegion = GetSceneRegionAt(reinterpret_cast<void*>(self->field_0004), nullptr, 12);

    void* targetPtr = reinterpret_cast<void*>(targetObject);
    auto getHandler = reinterpret_cast<int(__fastcall*)(void*, void*)>(CSF_AT(void**, targetPtr, 0)[28 / 4]);
    C_008670f0* handlerObject = reinterpret_cast<C_008670f0*>(getHandler(targetPtr, nullptr));

    return C_008670f0_SetField0C(handlerObject, nullptr, sceneRegion);
}

HOOK(0x006C58A0, C_008656d0_SetSkinRegionFromTarget, "auto");
