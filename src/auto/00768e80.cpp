#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

// Grows a dynamic array of 12-byte elements by `extra` entries.
// Layout: field_0000 = data, field_0004 = capacity, field_0008 = used count.
static char S_00768e80_Grow(S_00768e80* self, int extra);

static char __fastcall S_00768e80_GrowImpl(S_00768e80* self, void* /*edx*/, int extra)
{
    uint32_t newCount = extra + self->field_0004;
    uint8_t* newData = static_cast<uint8_t*>(Mem_Alloc(12 * newCount));
    char result = 0;
    uint8_t* dest;
    uint8_t* savedDest;

    if (newData) {
        int last = static_cast<int>(newCount) - 1;
        if (last >= 0) {
            uint8_t* p = newData;
            int n = last + 1;
            do {
                CSF_AT(uint32_t, p, 0) = 0;
                CSF_AT(uint32_t, p, 4) = (uint32_t)(uintptr_t)CSF_ADDR_DATA(0x86F164);
                CSF_AT(uint8_t, p, 8) = 1;
                p += 12;
                --n;
            } while (n != 0);
        }
        dest = newData;
        savedDest = newData;
    } else {
        savedDest = nullptr;
        dest = nullptr;
    }

    uint32_t used = self->field_0008;
    if (used != 0) {
        std::memcpy(dest, reinterpret_cast<const void*>(self->field_0000), 12 * used);
        result = 0;
    }

    self->field_0004 = self->field_0004 + extra;
    if (self->field_0000 != 0) {
        result = static_cast<char>(Mem_Free(reinterpret_cast<void*>(self->field_0000)));
        dest = savedDest;
        self->field_0000 = 0;
    }
    self->field_0000 = reinterpret_cast<uint32_t>(dest);
    return result;
}

HOOK(0x768E80, S_00768e80_GrowImpl, "auto");
