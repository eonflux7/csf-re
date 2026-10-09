#include <cstdint>
#include "globals.h"
#include <cstddef>
#include "hook.h"
#include "types/auto_classes.h"

// Copies the state fields of a source record (pointed to by a2) into this object.
// Field at source +0x04 selects whether the source int at +0x6C is converted into field_0058.
static int __fastcall CopyStateFromRecord(S_0045bda0* self, void* /*edx*/, int a2) {
    const int result = a2;
    uint8_t* src = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(static_cast<uint32_t>(a2)));

    self->field_0028 = CSF_AT(uint32_t, src, 0x04);
    self->field_0030 = CSF_AT(uint32_t, src, 0x0C);
    self->field_0034 = CSF_AT(uint32_t, src, 0x10);
    self->field_003c = CSF_AT(uint32_t, src, 0x18);
    self->field_0040 = CSF_AT(uint32_t, src, 0x1C);
    self->field_0044 = CSF_AT(uint32_t, src, 0x20);
    self->field_0048 = CSF_AT(uint32_t, src, 0x24);
    self->field_004c = CSF_AT(uint32_t, src, 0x28);
    self->field_0050 = CSF_AT(uint32_t, src, 0x2C);
    self->field_0054 = CSF_AT(uint8_t, src, 0x30);

    if (CSF_AT(uint32_t, src, 0x04) == 4) {
        self->field_0058 = static_cast<float>(CSF_AT(int32_t, src, 0x6C));
    }
    return result;
}

HOOK(0x463FA0, CopyStateFromRecord, "auto");

