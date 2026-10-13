#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Copies 32 five-dword entries (20 bytes each) from this+0x04 into the caller's
// buffer, then returns the pointer just past the last source entry (this+0x284).
// If the destination is null, nothing is copied and this+0x04 is returned.
// Called with ecx = this, one stack arg (destination); callee pops 4.
static uint32_t* __fastcall OptionsFile_CopyEntries(void* self, void* /*edx*/, void* destination)
{
    uint8_t* source = reinterpret_cast<uint8_t*>(self) + 4;

    if (destination == nullptr)
    {
        return reinterpret_cast<uint32_t*>(source);
    }

    uint32_t* dst = static_cast<uint32_t*>(destination);

    for (int entryIndex = 32; entryIndex != 0; --entryIndex)
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

HOOK(0x50A670, OptionsFile_CopyEntries, "auto");
