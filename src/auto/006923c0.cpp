#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

static uint32_t __fastcall ReleaseObjectReference(C_0085e65c* self, void*) {
    uint32_t result = --self->field_0004;
    if (!result) self->vSlot0(1);
    return result;
}
HOOK(0x006923C0, ReleaseObjectReference, "auto");
