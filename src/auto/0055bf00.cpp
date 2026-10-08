#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// _memcpy (cdecl, returns dst, pops nothing)
CSF_FUNC(void*, __cdecl, Memcpy, 0x007AEE60, (void* dest, const void* src, size_t count));

// Removes the first element equal to `value` from the uint32_t array at field_0008
// (count in field_0010), shifting the tail down, and marks the list dirty (field_0018).
// No-op if the value is not present.
static void __fastcall C_008453a4_RemoveItemByValue(C_008453a4* self, void* /*edx*/, uint32_t value)
{
    uint32_t count = self->field_0010;
    uint32_t index = 0;

    if (count != 0) {
        uint32_t* items = reinterpret_cast<uint32_t*>(self->field_0008);
        for (uint32_t* p = items; *p != value; ++p) {
            if (++index >= count)
                return;
        }

        if (index != 0xFFFFFFFFu) {
            uint32_t newCount = self->field_0010 - 1;
            self->field_0010 = newCount;
            if (index != newCount) {
                uint8_t* base = reinterpret_cast<uint8_t*>(self->field_0008);
                uint8_t* dst = base + index * 4u;
                Memcpy(dst, dst + 4, static_cast<size_t>((newCount - index) * 4u));
            }
            self->field_0018 = 1;
        }
    }
}

HOOK(0x0055BF00, C_008453a4_RemoveItemByValue, "auto");
