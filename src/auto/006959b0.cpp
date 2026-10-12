#include <cstdint>
#include "hook.h"
#include "types/auto_classes.h"

// Copies the range [src, self->field_0008) into dest (when dest != src), advances
// self->field_0008 to the end of the written data, then stores dest into *outSlot.
// Returns outSlot.
static uint32_t* __fastcall VariantValues_CopyRangeAndSetEnd(
    S_006959b0* self, void* /*edx*/, uint32_t* outSlot, uint32_t* dest, uint32_t* src) {
    if (dest != src) {
        uint32_t* end = reinterpret_cast<uint32_t*>(self->field_0008);
        uint32_t* out = dest;
        if (src != end) {
            do {
                *out++ = *src++;
            } while (src != end);
        }
        self->field_0008 = reinterpret_cast<uint32_t>(out);
    }
    *outSlot = reinterpret_cast<uint32_t>(dest);
    return outSlot;
}

HOOK(0x6959B0, VariantValues_CopyRangeAndSetEnd, "auto");
