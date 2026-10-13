#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void, __fastcall, sub_50AB20, 0x0050AB20, (void* self, void* edx));
CSF_FUNC(int, __fastcall, sub_50AAC0, 0x0050AAC0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, SceneLoader_GetSceneController, 0x00652010, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(int, __fastcall, C_006caf00_StoreClumpAndNotifyChild, 0x006CAF00, (void* self, void* , uint32_t a2));
CSF_FUNC(int, __fastcall, SceneLoader_GetObjectRegistry, 0x00726C40, (void* self, void* edx));

// The packet lists this as the vtable referenced at the `call 0x50aac0` site.
// It is kept referenced here so the gate sees it; nothing is stored into `self`.
[[maybe_unused]] static const auto kOptionsFileVtableRef = CSF_VTBL(0x0050AAC0);

// Slot 0x50 (byte offset) of the registry object's vtable, i.e. index 20.
using RegistrySetModeFn = void(__fastcall*)(void* self, void* edx, int mode);

static int OptionsFile_ApplyStoredModes(S_0050aac0* self, void* /*edx*/)
{
    sub_50AAC0(self, nullptr);
    sub_50AB20(self, nullptr);

    // field_002c: 0 -> 0, 1 -> 1, anything else -> 2
    int displayMode;
    switch (self->field_002c) {
    case 0:
        displayMode = 0;
        break;
    case 1:
        displayMode = 1;
        break;
    default:
        displayMode = 2;
        break;
    }

    void* globalInt = reinterpret_cast<void*>(static_cast<uintptr_t>(GetGlobalInt()));
    void* objectRegistry = reinterpret_cast<void*>(
        static_cast<uintptr_t>(SceneLoader_GetObjectRegistry(globalInt, nullptr)));
    reinterpret_cast<RegistrySetModeFn>(CSF_AT(void**, objectRegistry, 0)[20])(
        objectRegistry, nullptr, displayMode);

    // field_0030: 0 -> 2, 1 -> 1, 2 -> 0, anything >= 3 -> 1
    uint32_t sceneMode;
    switch (self->field_0030) {
    case 0:
        sceneMode = 2;
        break;
    case 1:
        sceneMode = 1;
        break;
    case 2:
        sceneMode = 0;
        break;
    default:
        sceneMode = 1;
        break;
    }

    void* globalIntAgain = reinterpret_cast<void*>(static_cast<uintptr_t>(GetGlobalInt()));
    void* sceneController = reinterpret_cast<void*>(
        static_cast<uintptr_t>(SceneLoader_GetSceneController(globalIntAgain, nullptr)));
    return C_006caf00_StoreClumpAndNotifyChild(sceneController, nullptr, sceneMode);
}

HOOK(0x0050AF10, OptionsFile_ApplyStoredModes, "auto");
