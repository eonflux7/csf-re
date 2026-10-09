#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(int, __fastcall, Scene_FindEntryById, 0x00468750, (void* self, void* edx, int a2));
CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);
[[maybe_unused]] static void* const s_SceneEntryVtable = CSF_VTBL(0x00468750);

static int __stdcall ResetFoundSceneEntry([[maybe_unused]] int a1, [[maybe_unused]] int a2, int a3)
{
    // Scene slot at +0xBC (index 47) of the current scene object.
    void* sceneSlot = CSF_AT(void*, g_pCurrentScene, 0xBC);

    // Walk the argument chain: *a3 -> [+4] -> [+4] gives the lookup key.
    void* requestHandle = reinterpret_cast<void*>(static_cast<uintptr_t>(a3));
    void* innerPtr = CSF_AT(void*, requestHandle, 0);
    uint32_t midValue = CSF_AT(uint32_t, innerPtr, 4);
    uint32_t lookupKey = CSF_AT(uint32_t, reinterpret_cast<void*>(static_cast<uintptr_t>(midValue)), 4);

    int result = Scene_FindEntryById(sceneSlot, nullptr, static_cast<int>(lookupKey));
    if (result == 0) {
        return result;
    }

    void* entry = reinterpret_cast<void*>(static_cast<uintptr_t>(result));
    CSF_AT(uint32_t, entry, 104) = 0;
    CSF_AT(uint32_t, entry, 88) = 0;
    CSF_AT(uint32_t, entry, 24) = 0;
    CSF_AT(uint32_t, entry, 28) = 0;
    CSF_AT(uint32_t, entry, 20) = 0;
    CSF_AT(uint8_t, entry, 129) = 0;
    CSF_AT(uint8_t, entry, 131) = 0;
    CSF_AT(uint32_t, entry, 96) = 0xFFFFFFFFu;
    CSF_AT(uint32_t, entry, 100) = 0xFFFFFFFFu;
    CSF_AT(uint8_t, entry, 130) = 0;
    CSF_AT(uint32_t, entry, 108) = 0xFFFFFFFFu;
    CSF_AT(uint32_t, entry, 112) = 0;
    CSF_AT(uint32_t, entry, 116) = 0xFFFFFFFFu;
    CSF_AT(uint32_t, entry, 120) = 0;
    CSF_AT(uint32_t, entry, 124) = 0;
    CSF_AT(uint8_t, entry, 128) = 0;

    return result;
}

HOOK(0x00461C60, ResetFoundSceneEntry, "auto");
