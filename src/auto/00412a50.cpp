#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Game heap free (__cdecl, 0x691110).
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

// Destructor body for C_0082d448 with delete flags:
//   bit 1 (0x02): array form. Element count is stored 4 bytes before the object;
//                 each 12-byte element gets its vtable reset, highest index first.
//   bit 0 (0x01): release the heap block (array header included in array form).
// Returns the block start (count header for array form, object otherwise).
static uint32_t* __fastcall C_0082d448_Destroy(C_0082d448* self, void* /*edx*/, char flags) {
    if ((flags & 2) != 0) {
        int elementCount = CSF_AT(int, self, -4);
        uint8_t* elementEnd = reinterpret_cast<uint8_t*>(self) + 12 * elementCount;
        for (int remaining = elementCount; remaining > 0; --remaining) {
            elementEnd -= 12;
            *reinterpret_cast<void***>(elementEnd) = CSF_VTBL(0x0082D410);
        }
        if ((flags & 1) != 0) {
            Mem_Free(reinterpret_cast<uint8_t*>(self) - 4);
        }
        return reinterpret_cast<uint32_t*>(reinterpret_cast<uint8_t*>(self) - 4);
    }

    *reinterpret_cast<void***>(self) = CSF_VTBL(0x0082D410);
    if ((flags & 1) != 0) {
        Mem_Free(self);
    }
    return reinterpret_cast<uint32_t*>(self);
}

HOOK(0x00412A50, C_0082d448_Destroy, "auto");
