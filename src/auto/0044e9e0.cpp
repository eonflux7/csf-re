#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_FUNC(void*, __cdecl, _memcpy, 0x007AEE60, (void* destination, const void* source, std::size_t byteCount));

// Removes the dword at `index` from a dword array (data pointer at +0, element count at +8),
// shifting the following elements down by one. Returns the new count.
static uint8_t* __fastcall DwordArrayRemoveAtIndexShift(void* self, void* /*edx*/, int32_t index)
{
    int32_t newCount = CSF_AT(int32_t, self, 0x08) - 1;
    CSF_AT(int32_t, self, 0x08) = newCount;
    uint8_t* result = reinterpret_cast<uint8_t*>(static_cast<intptr_t>(newCount));

    if (index != newCount)
    {
        uint8_t* data = CSF_AT(uint8_t*, self, 0x00);
        std::size_t shiftBytes = static_cast<std::size_t>(newCount - index) * 4;
        _memcpy(data + static_cast<intptr_t>(index) * 4,
                data + static_cast<intptr_t>(index) * 4 + 4,
                shiftBytes);
    }
    return result;
}

HOOK(0x0044E9E0, DwordArrayRemoveAtIndexShift, "auto");
