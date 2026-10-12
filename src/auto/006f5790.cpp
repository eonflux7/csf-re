#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"

// Callee: C_00840f48::IsActive (__thiscall, no stack args, returns bool in al).
CSF_FUNC(char, __fastcall, C_00840f48_IsActive, 0x0051C0B0, (void* self, void* edx));

// Scans the pointer array stored at +0x04 (count at +0x0C) and returns the first
// entry whose IsActive() is false. Returns 0 if the count is 0 or every entry is active.
static int __fastcall CameraProjection_FindFirstInactiveEntry(void* self, void* /*edx*/)
{
    uint32_t index = 0;

    if (CSF_AT(uint32_t, self, 0x0C) == 0)
        return 0;

    while (true)
    {
        // The original reloads the array pointer on every iteration.
        uint32_t* entries = reinterpret_cast<uint32_t*>(CSF_AT(uint32_t, self, 0x04));
        void* entry = reinterpret_cast<void*>(entries[index]);

        if (!C_00840f48_IsActive(entry, nullptr))
            return static_cast<int>(entries[index]);

        // The original reloads the count after the call.
        ++index;
        if (index >= CSF_AT(uint32_t, self, 0x0C))
            return 0;
    }
}

HOOK(0x006F5790, CameraProjection_FindFirstInactiveEntry, "auto");
