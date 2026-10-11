#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

static CSF_FUNC(unsigned int, __fastcall, DestroySceneObjectOptionsStorage, 0x004018E0, (void* self, void* edx));
static CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));

static unsigned int __fastcall WeaponFirearms_FinalizeSceneObjectOptions(
    void* self, void* /*edx*/)
{
    if (CSF_AT(uint32_t, self, 0x90) >= 0x10u &&
        CSF_AT(void*, self, 0x7C))
    {
        Mem_FreeDebug(CSF_AT(void*, self, 0x7C));
    }

    CSF_AT(uint32_t, self, 0x90) = 15;
    CSF_AT(uint32_t, self, 0x8C) = 0;
    CSF_AT(uint8_t, self, 0x7C) = 0;

    return DestroySceneObjectOptionsStorage(
        static_cast<uint8_t*>(self) + 4, nullptr);
}

HOOK(0x004772B0, WeaponFirearms_FinalizeSceneObjectOptions, "auto");
