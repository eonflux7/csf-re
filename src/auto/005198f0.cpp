#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_GLOBAL(float, g_fZero, 0x0082D4CC);

CSF_FUNC(void*, __fastcall, C_00840d64_ReleaseEntryTarget, 0x005197A0, (void* self, void* edx, int a2));

// Ages every timed entry by deltaTime (entries are {float timer; uint32 target;} 8-byte records).
// Entries whose timer has run down to zero or below release their target object and clear it.
static unsigned int __fastcall C_00840d64_TickTimedEntries(C_00840d64* self, void* /*edx*/, float deltaTime)
{
    unsigned int result = self->field_0020;
    for (unsigned int i = 0; i < result; ++i)
    {
        uint8_t* entry = reinterpret_cast<uint8_t*>(self->field_0018) + 8 * i;
        float& timer = *reinterpret_cast<float*>(entry);
        if (timer > g_fZero)
        {
            float remaining = timer - deltaTime;
            timer = remaining;
            if (remaining <= g_fZero)
            {
                int target = static_cast<int>(*reinterpret_cast<uint32_t*>(entry + 4));
                C_00840d64_ReleaseEntryTarget(self, nullptr, target);
                *reinterpret_cast<uint32_t*>(entry + 4) = 0;
            }
        }
        result = self->field_0020;
    }
    return result;
}

HOOK(0x005198F0, C_00840d64_TickTimedEntries, "auto");
