#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(unsigned int, __fastcall, SceneLoader_ReleaseAndResetBuffers, 0x00500210, (void* self, void*));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));
CSF_FUNC(int, __cdecl, ReleaseSceneEntryResources, 0x00755BE0, (int a1, int a2, int a3, int a4));

static unsigned int __fastcall C_00869cd0_Destroy(C_00869cd0* self, void* /*edx*/)
{
    uint8_t* sceneEntry = CSF_AT(uint8_t*, self, 0x4);
    *(void***)self = CSF_VTBL(0x00869CD0);

    if (sceneEntry)
    {
        if (CSF_AT(uint32_t, sceneEntry, 0x4))
        {
            ReleaseSceneEntryResources(
                (int)CSF_AT(uint32_t, sceneEntry, 0x4),
                (int)CSF_AT(uint32_t, sceneEntry, 0x8),
                (int)(uintptr_t)sceneEntry,
                0);
            if (CSF_AT(uint32_t, sceneEntry, 0x4))
                Mem_FreeDebug((void*)(uintptr_t)CSF_AT(uint32_t, sceneEntry, 0x4));
        }
        CSF_AT(uint32_t, sceneEntry, 0x4) = 0;
        CSF_AT(uint32_t, sceneEntry, 0x8) = 0;
        CSF_AT(uint32_t, sceneEntry, 0xC) = 0;
        Mem_Free(sceneEntry);
        CSF_AT(uint8_t*, self, 0x4) = nullptr;
    }

    return SceneLoader_ReleaseAndResetBuffers(
        reinterpret_cast<void*>(reinterpret_cast<uint8_t*>(self) + 8), nullptr);
}

HOOK(0x007563B0, C_00869cd0_Destroy, "auto");
