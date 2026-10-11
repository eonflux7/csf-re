#include <cstdint>
#include "globals.h"
#include "hook.h"

CSF_GLOBAL(float, g_fZero, 0x0082D4CC);

static void UpdateEntryTimer(void* self, uint32_t offset, float elapsed)
{
    uint8_t* entry = CSF_AT(uint8_t*, self, 0x1C) + offset;
    if (CSF_AT(int32_t, entry, 0x00) != -1)
    {
        float remaining = CSF_AT(float, entry, 0x20) - elapsed;
        CSF_AT(float, entry, 0x20) = remaining;
        if (remaining < g_fZero)
            CSF_AT(int32_t, entry, 0x00) = -1;
    }
}

static void __fastcall UpdateTimers(void* self, void* /*edx*/, float elapsed)
{
    const uint32_t count = CSF_AT(uint32_t, self, 0x24);
    uint32_t processed = 0;

    if (static_cast<int32_t>(count) >= 4)
    {
        uint32_t groupsRemaining = ((count - 4) >> 2) + 1;
        processed = 4 * groupsRemaining;
        uint32_t offset = 0;

        do
        {
            UpdateEntryTimer(self, offset, elapsed);
            UpdateEntryTimer(self, offset + 40, elapsed);
            UpdateEntryTimer(self, offset + 120 - 40, elapsed);
            UpdateEntryTimer(self, offset + 120, elapsed);
            offset += 160;
        }
        while (--groupsRemaining);
    }

    if (processed < count)
    {
        uint32_t offset = 40 * processed;
        uint32_t remaining = count - processed;
        do
        {
            UpdateEntryTimer(self, offset, elapsed);
            offset += 40;
        }
        while (--remaining);
    }
}

HOOK_KEEP_ECX(0x00445D70, UpdateTimers, 1, "auto");
