#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(int, __stdcall, UpdateTimedEntry, 0x00446910, (int a1, float arg4));

CSF_GLOBAL(float, g_HalfDurationScale, 0x0082D444);
CSF_GLOBAL(float, g_fZero, 0x0082D4CC);

static int __fastcall UpdateTimedEntries(void* self, void* /*edx*/, float elapsed)
{
    uint32_t remaining = CSF_AT(uint32_t, self, 0x34);
    if (!remaining)
        return 0;

    uint32_t offset = 0;
    do
    {
        auto* entry = reinterpret_cast<uint8_t*>(
            CSF_AT(uint32_t, self, 0x2C)) + offset;

        if (CSF_AT(int32_t, entry, 0) != -1)
        {
            if (CSF_AT(uint32_t, entry, 0x14) != 0x7F7FFFFF)
                CSF_AT(float, entry, 0x14) -= elapsed;

            if (CSF_AT(float, entry, 0x14) < g_fZero)
            {
                CSF_AT(int32_t, entry, 0) = -1;
            }
            else
            {
                float accumulated = elapsed + CSF_AT(float, entry, 0x1C);
                CSF_AT(float, entry, 0x1C) = accumulated;
                if (accumulated > g_HalfDurationScale)
                {
                    UpdateTimedEntry(reinterpret_cast<int>(entry), accumulated);
                    CSF_AT(uint32_t, entry, 0x1C) = 0;
                }
            }
        }

        offset += 32;
        --remaining;
    } while (remaining);

    return 0;
}

HOOK(0x004471D0, UpdateTimedEntries, "auto");
