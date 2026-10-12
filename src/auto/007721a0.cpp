#include <cstdint>
#include <cstddef>
#include "hook.h"
#include "types/auto_classes.h"

// Copies the (key, value) pairs from moveFrom up to the container end (self->field_0008)
// down onto eraseFirst, advances the container end to the new last position, and writes
// eraseFirst to *outIterator. Returns outIterator.
static uint32_t* __fastcall S_007721a0_EraseElementRange(S_007721a0* self, void* /*edx*/,
                                                         uint32_t* outIterator,
                                                         uint32_t* eraseFirst,
                                                         uint32_t* moveFrom)
{
    if (eraseFirst != moveFrom) {
        uint32_t* containerEnd = reinterpret_cast<uint32_t*>(self->field_0008);
        uint32_t* dest = eraseFirst;
        uint32_t* src = moveFrom;
        if (moveFrom != containerEnd) {
            do {
                dest[0] = src[0];
                dest[1] = src[1];
                src += 2;
                dest += 2;
            } while (src != containerEnd);
        }
        self->field_0008 = static_cast<uint32_t>(reinterpret_cast<uintptr_t>(dest));
    }
    *outIterator = static_cast<uint32_t>(reinterpret_cast<uintptr_t>(eraseFirst));
    return outIterator;
}

HOOK(0x007721A0, S_007721a0_EraseElementRange, "auto");
