#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

static void* __fastcall SceneLights_DestroyStringStorage(
    void* self, void* /*edx*/, char flags)
{
    if ((flags & 2) != 0)
    {
        void* allocation = reinterpret_cast<void*>(
            reinterpret_cast<std::uintptr_t>(self) - 4);
        std::uint32_t remaining = CSF_AT(std::uint32_t, allocation, 0);

        if (static_cast<std::int32_t>(remaining - 1) >= 0)
        {
            std::uintptr_t elementAddress =
                reinterpret_cast<std::uintptr_t>(self) + remaining * 0x28u;

            do
            {
                elementAddress -= 0x28;
                void* element = reinterpret_cast<void*>(elementAddress);

                if (CSF_AT(std::uint32_t, element, 0x18) >= 0x10 &&
                    CSF_AT(void*, element, 4) != nullptr)
                {
                    Mem_FreeDebug(CSF_AT(void*, element, 4));
                }

                --remaining;
                CSF_AT(std::uint32_t, element, 0x18) = 15;
                CSF_AT(std::uint32_t, element, 0x14) = 0;
                CSF_AT(std::uint8_t, element, 4) = 0;
            }
            while (remaining != 0);
        }

        if ((flags & 1) != 0)
            Mem_Free(allocation);

        return allocation;
    }

    if (CSF_AT(std::uint32_t, self, 0x18) >= 0x10 &&
        CSF_AT(void*, self, 4) != nullptr)
    {
        Mem_FreeDebug(CSF_AT(void*, self, 4));
    }

    CSF_AT(std::uint32_t, self, 0x18) = 15;
    CSF_AT(std::uint32_t, self, 0x14) = 0;
    CSF_AT(std::uint8_t, self, 4) = 0;

    if ((flags & 1) != 0)
        Mem_Free(self);

    return self;
}

HOOK(0x004F0810, SceneLights_DestroyStringStorage, "auto");
