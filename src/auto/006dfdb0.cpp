#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

static void __fastcall C_00861988_ReleaseEntry(
    C_00861988* self, void* /*edx*/, int entry)
{
    const uint32_t count = self->field_002c;
    if (count == 0)
        return;

    // The original scans the slot array one dword per compare (a garbage count
    // makes that tens of thousands of rounds); under the hook build's /Od the
    // same linear scan must stay inside the emulator's instruction budget, so
    // the scan is chunked. The compared addresses, the match index and every
    // store are the original's; only the loop is grouped.
    const uint32_t key = static_cast<uint32_t>(entry);
    const uint32_t* start = reinterpret_cast<const uint32_t*>(self->field_0024);
    const uint32_t* end = start + count;
    const uint32_t* p = start;
    const uint32_t* hit = nullptr;

    while (p + 4 <= end)
    {
        if (p[0] == key) { hit = p; break; }
        if (p[1] == key) { hit = p + 1; break; }
        if (p[2] == key) { hit = p + 2; break; }
        if (p[3] == key) { hit = p + 3; break; }
        p += 4;
    }
    if (hit == nullptr)
    {
        while (p != end)
        {
            if (*p == key) { hit = p; break; }
            ++p;
        }
    }

    if (hit != nullptr)
    {
        const int index = static_cast<int>(hit - start);
        if (index != -1)
        {
            if (entry != 0)
            {
                void* object = reinterpret_cast<void*>(entry);
                ((void(__fastcall*)(void*, void*, int))
                    CSF_AT(void**, object, 0)[1])(object, nullptr, 1);
            }
            reinterpret_cast<uint32_t*>(self->field_0024)[index] = 0;
        }
    }
}

HOOK(0x006DFDB0, C_00861988_ReleaseEntry, "auto");
