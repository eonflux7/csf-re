#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));

static int __cdecl NetPerfCounters_DestroyRange(
    int first, int last, int /*unused3*/, int /*unused4*/)
{
    uint32_t cursor = static_cast<uint32_t>(first);
    const uint32_t end = static_cast<uint32_t>(last);

    if (cursor != end)
    {
        uint8_t* fields =
            reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(cursor)) + 60;

        do
        {
            if (CSF_AT(uint32_t, fields, 0x64) >= 0x10u)
            {
                void* block = CSF_AT(void*, fields, 0x50);
                if (block)
                    Mem_FreeDebug(block);
            }
            CSF_AT(uint32_t, fields, 0x64) = 15;
            CSF_AT(uint32_t, fields, 0x60) = 0;
            CSF_AT(uint8_t, fields, 0x50) = 0;

            if (CSF_AT(uint32_t, fields, 0x00) >= 0x10u)
            {
                void* block = CSF_AT(void*, fields, -0x14);
                if (block)
                    Mem_FreeDebug(block);
            }
            CSF_AT(uint32_t, fields, 0x00) = 15;
            CSF_AT(uint32_t, fields, -0x04) = 0;
            CSF_AT(uint8_t, fields, -0x14) = 0;

            if (CSF_AT(uint32_t, fields, -0x1C) >= 0x10u)
            {
                void* block = CSF_AT(void*, fields, -0x30);
                if (block)
                    Mem_FreeDebug(block);
            }
            CSF_AT(uint32_t, fields, -0x1C) = 15;
            CSF_AT(uint32_t, fields, -0x20) = 0;
            CSF_AT(uint8_t, fields, -0x30) = 0;

            cursor += 164;
            fields += 164;
        } while (cursor != end);
    }

    return last;
}

HOOK(0x0078BE10, NetPerfCounters_DestroyRange, "auto");
