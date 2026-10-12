#include <cstdint>
#include "globals.h"
#include "hook.h"

// Container layout (from the original): +0x04 = pointer to an array of 32-bit entries,
// +0x10 = number of entries. Returns entry[index] when 0 <= index < count, otherwise 0.
static int __fastcall MpGameModes_GetSessionEntryByIndex(void* self, void* /*edx*/, int index)
{
    int result = 0;
    if (index >= 0 && index < CSF_AT(int, self, 0x10)) {
        uint32_t* entries = reinterpret_cast<uint32_t*>(CSF_AT(uint32_t, self, 0x4));
        result = static_cast<int>(entries[index]);
    }
    return result;
}

HOOK(0x0054E610, MpGameModes_GetSessionEntryByIndex, "auto");
