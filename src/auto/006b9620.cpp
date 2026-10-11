#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

static int __fastcall GfxObjectProps_ClearPropertyEntries(int* self, void* /*edx*/)
{
    for (uint32_t index = 0; ; ++index)
    {
        auto* entries = CSF_AT(uint8_t*, self, 0x04);
        if (!entries)
            break;

        const uint32_t byteCount =
            CSF_AT(uint32_t, self, 0x08) -
            reinterpret_cast<uint32_t>(entries);
        const uint32_t entryCount =
            static_cast<uint32_t>(static_cast<int32_t>(byteCount) >> 3);
        if (index >= entryCount)
            break;

        void* property = CSF_AT(void*, entries, 8 * index);
        if (property)
        {
            if (CSF_AT(uint32_t, property, 0x18) >= 0x10u &&
                CSF_AT(void*, property, 0x04))
            {
                Mem_FreeDebug(CSF_AT(void*, property, 0x04));
            }

            CSF_AT(uint32_t, property, 0x18) = 15;
            CSF_AT(uint32_t, property, 0x14) = 0;
            CSF_AT(char, property, 0x04) = 0;
            Mem_Free(property);

            CSF_AT(void*, CSF_AT(uint8_t*, self, 0x04), 8 * index) = nullptr;
        }
    }

    void* entries = CSF_AT(void*, self, 0x04);
    int result = entries ? Mem_FreeDebug(entries) : 0;

    CSF_AT(uint32_t, self, 0x04) = 0;
    CSF_AT(uint32_t, self, 0x08) = 0;
    CSF_AT(uint32_t, self, 0x0C) = 0;
    return result;
}

HOOK(0x006B9620, GfxObjectProps_ClearPropertyEntries, "auto");
