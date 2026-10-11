#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

static int __fastcall AppMain_DestroyStringRecord(void* self, void* /*edx*/, char flags)
{
    auto* bytes = static_cast<uint8_t*>(self);

    if ((flags & 2) != 0)
    {
        void* allocation = bytes - 4;
        const uint32_t count = CSF_AT(uint32_t, self, -4);

        if (static_cast<int32_t>(count - 1) >= 0)
        {
            auto* element = bytes + 56 * count;
            uint32_t remaining = count;
            do
            {
                element -= 56;
                if (CSF_AT(uint32_t, element, 0x34) >= 8)
                {
                    void* buffer = CSF_AT(void*, element, 0x20);
                    if (buffer)
                        Mem_FreeDebug(buffer);
                }

                --remaining;
                CSF_AT(uint32_t, element, 0x34) = 7;
                CSF_AT(uint32_t, element, 0x30) = 0;
                CSF_AT(uint16_t, element, 0x20) = 0;
            }
            while (remaining != 0);
        }

        if ((flags & 1) != 0)
            Mem_Free(allocation);

        return static_cast<int>(reinterpret_cast<uintptr_t>(allocation));
    }

    if (CSF_AT(uint32_t, self, 0x34) >= 8)
    {
        void* buffer = CSF_AT(void*, self, 0x20);
        if (buffer)
            Mem_FreeDebug(buffer);
    }

    CSF_AT(uint32_t, self, 0x34) = 7;
    CSF_AT(uint32_t, self, 0x30) = 0;
    CSF_AT(uint16_t, self, 0x20) = 0;

    if ((flags & 1) != 0)
        Mem_Free(self);

    return static_cast<int>(reinterpret_cast<uintptr_t>(self));
}

HOOK(0x00508180, AppMain_DestroyStringRecord, "auto");
