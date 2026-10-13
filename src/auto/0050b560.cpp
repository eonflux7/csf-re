#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

// Binds to the original functions by address.
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(int, __fastcall, SceneLoader_GetSceneController, 0x00652010, (void* self, void* edx));
CSF_FUNC(int, __fastcall, C_006caf00_StoreClumpAndNotifyChild, 0x006CAF00, (void* self, void* , uint32_t a2));

// Maps the stored option index (0, 1, 2, other) to the value passed to StoreClumpAndNotifyChild.
static int OptionsFile_MapModeToClumpValue(int a2) {
    if (a2 == 0) {
        return 2;
    }
    if (a2 == 2) {
        return 0;
    }
    // a2 == 1 and any other value both yield 1.
    return 1;
}

// COptionsFile::SetModeAndStoreClump (thiscall, one stack arg, ret 4).
static int __fastcall OptionsFile_SetModeAndStoreClump(void* self, void* /*edx*/, int a2) {
    CSF_AT(int, self, 0x30) = a2;

    int clumpValue = OptionsFile_MapModeToClumpValue(a2);

    void* globalInt = reinterpret_cast<void*>(GetGlobalInt());
    void* sceneController = reinterpret_cast<void*>(SceneLoader_GetSceneController(globalInt, nullptr));
    return C_006caf00_StoreClumpAndNotifyChild(sceneController, nullptr, static_cast<uint32_t>(clumpValue));
}

HOOK(0x50B560, OptionsFile_SetModeAndStoreClump, "auto");
