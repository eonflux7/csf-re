#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Callee returns an int that the caller adds to its total (mov eax from the call result is used),
// so the return is declared int here instead of void.
CSF_FUNC(int, __fastcall, sub_6C2030, 0x006C2030, (void* self, void* edx));

// Number of 36-byte records between elementsBegin and elementsEnd (signed division, as in the original).
static int C_00860954_ElementCount(C_00860954* self) {
    return static_cast<int>(self->field_000c - self->field_0008) / 36;
}

// Writes {record count, sum of child results} into out[0] and out[1]; returns out (ret 4).
static int* __fastcall C_00860954_SumChildTotals(C_00860954* self, void* /*edx*/, int* out) {
    int recordCount = 0;
    if (self->field_0008 != 0) {
        recordCount = C_00860954_ElementCount(self);
    }

    unsigned int index = 0;
    int childTotal = 0;
    for (uint32_t byteOffset = 0;; byteOffset += 36) {
        if (self->field_0008 == 0 ||
            index >= static_cast<unsigned int>(C_00860954_ElementCount(self))) {
            break;
        }
        uint8_t* record = reinterpret_cast<uint8_t*>(self->field_0008) + byteOffset;
        void* child = CSF_AT(void*, record, 0x1C);
        if (child) {
            childTotal += sub_6C2030(child, nullptr);
        }
        ++index;
    }

    out[0] = recordCount;
    out[1] = childTotal;
    return out;
}

HOOK(0x006C8E50, C_00860954_SumChildTotals, "auto");
