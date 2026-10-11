#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

static int __fastcall GfxScene_DestroyStringStorage(
    void* self, void* /*edx*/, char flags)
{
    auto* storage = static_cast<uint8_t*>(self);
    void* allocation = self;

    if ((flags & 2) != 0)
    {
        allocation = storage - 4;
        uint32_t count = CSF_AT(uint32_t, storage, -4);

        if (static_cast<int32_t>(count - 1) >= 0)
        {
            auto* element = reinterpret_cast<uint8_t*>(
                reinterpret_cast<uintptr_t>(storage) + count * 28);

            do
            {
                element -= 28;
                if (CSF_AT(uint32_t, element, 24) >= 0x10)
                {
                    void* block = CSF_AT(void*, element, 4);
                    if (block)
                        Mem_FreeDebug(block);
                }

                --count;
                CSF_AT(uint32_t, element, 24) = 15;
                CSF_AT(uint32_t, element, 20) = 0;
                CSF_AT(uint8_t, element, 4) = 0;
            } while (count != 0);
        }
    }
    else
    {
        if (CSF_AT(uint32_t, storage, 24) >= 0x10)
        {
            void* block = CSF_AT(void*, storage, 4);
            if (block)
                Mem_FreeDebug(block);
        }

        CSF_AT(uint32_t, storage, 24) = 15;
        CSF_AT(uint32_t, storage, 20) = 0;
        CSF_AT(uint8_t, storage, 4) = 0;
    }

    if ((flags & 1) != 0)
        Mem_Free(allocation);

    return static_cast<int>(reinterpret_cast<uintptr_t>(allocation));
}

HOOK(0x006CD210, GfxScene_DestroyStringStorage, "auto");
