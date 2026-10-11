#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

static CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

static int __fastcall FxParticleManager_ResetFirstManager(void* self, void* /*edx*/)
{
    for (std::uint32_t i = 0; i < CSF_AT(std::uint32_t, self, 0x24); ++i)
    {
        void* object = CSF_AT(void**, self, 0x1C)[i];
        if (object)
        {
            reinterpret_cast<void (__fastcall*)(void*, void*, int)>(
                CSF_AT(void**, object, 0)[2])(object, nullptr, 1);
            CSF_AT(void**, self, 0x1C)[i] = nullptr;
        }
    }

    void* storage = CSF_AT(void*, self, 0x1C);
    CSF_AT(std::uint32_t, self, 0x20) = 0;
    CSF_AT(std::uint32_t, self, 0x24) = 0;

    int result = 0;
    if (storage)
    {
        result = Mem_Free(storage);
        CSF_AT(void*, self, 0x1C) = nullptr;
    }
    return result;
}

HOOK(0x0051F7B0, FxParticleManager_ResetFirstManager, "auto");
