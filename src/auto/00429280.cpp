#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Copies 32 five-dword entries (20 bytes each) from this+0x74C into the caller's
// buffer, then returns the pointer just past the last copied entry.
// Called with ecx = this, stack args: destination (a3), unused (a4); callee pops 8.
static uint32_t* __fastcall C_008300c0_CopyCheatEntries(C_008300c0* self, void* /*edx*/,
                                                       void* destination, int /*unused*/)
{
    if (destination == nullptr)
    {
        // The original returns the incoming eax unchanged here; that value is not
        // recoverable from C++, so nullptr is returned instead (see notes).
        return nullptr;
    }

    uint8_t* source = reinterpret_cast<uint8_t*>(self) + 0x74C;
    uint32_t* dst = static_cast<uint32_t*>(destination);

    for (int entryIndex = 0; entryIndex < 32; ++entryIndex)
    {
        uint32_t* src = reinterpret_cast<uint32_t*>(source);
        dst[0] = src[0];
        dst[1] = src[1];
        dst[2] = src[2];
        dst[3] = src[3];
        dst[4] = src[4];
        source += 20;
        dst += 5;
    }

    return reinterpret_cast<uint32_t*>(source);
}

HOOK(0x429280, C_008300c0_CopyCheatEntries, "auto");
