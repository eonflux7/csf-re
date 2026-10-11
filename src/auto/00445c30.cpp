#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_GLOBAL(float, g_fZero, 0x0082D4CC);

static void __fastcall S_00445b40_UpdateEntryTimer(
    S_00445b40* self, void* /*edx*/, uint32_t offset, float elapsed)
{
    uint8_t* entry = CSF_AT(uint8_t*, self, 0x0C) + offset;
    if (CSF_AT(int32_t, entry, 0x00) != -1)
    {
        float remaining = CSF_AT(float, entry, 0x20) - elapsed;
        CSF_AT(float, entry, 0x20) = remaining;
        if (remaining < g_fZero)
            CSF_AT(int32_t, entry, 0x00) = -1;
    }
}

static void __fastcall S_00445b40_UpdateTimers(
    S_00445b40* self, void* /*edx*/, float elapsed)
{
    const uint32_t count = self->field_0014;
    uint32_t processed = 0;

    if (static_cast<int32_t>(count) >= 4)
    {
        uint32_t groupsRemaining = ((count - 4) >> 2) + 1;
        processed = 4 * groupsRemaining;
        uint32_t offset = 0;

        do
        {
            S_00445b40_UpdateEntryTimer(self, nullptr, offset, elapsed);
            S_00445b40_UpdateEntryTimer(self, nullptr, offset + 36, elapsed);
            S_00445b40_UpdateEntryTimer(self, nullptr, offset + 108 - 36, elapsed);
            S_00445b40_UpdateEntryTimer(self, nullptr, offset + 108, elapsed);
            offset += 144;
        }
        while (--groupsRemaining);
    }

    if (processed < count)
    {
        uint32_t offset = 36 * processed;
        uint32_t remaining = count - processed;
        do
        {
            S_00445b40_UpdateEntryTimer(self, nullptr, offset, elapsed);
            offset += 36;
        }
        while (--remaining);
    }
}

HOOK_KEEP_ECX(0x00445C30, S_00445b40_UpdateTimers, 1, "auto");
