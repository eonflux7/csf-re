#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void, __fastcall, RefreshTargetHandle, 0x0048D050, (void* self, void* edx));

static void __fastcall S_0048c9f0_CopyTargetHandle(
    S_0048c9f0* self, void* /*edx*/, const void* handle)
{
    const bool valid = CSF_AT(int32_t, handle, 0x08) >= 0;
    uint8_t flags = self->field_0039;
    if (valid)
        flags |= 1;
    else
        flags &= 0xFE;

    const uint32_t previousTarget = self->field_000c;
    self->field_0039 = flags;

    if (CSF_AT(uint32_t, handle, 0x04) != previousTarget)
        self->field_003c = 0x40000000;

    self->field_0039 &= 0xFD;
    self->field_000c = CSF_AT(uint32_t, handle, 0x04);
    self->field_0010 = CSF_AT(uint32_t, handle, 0x08);
    RefreshTargetHandle(self, nullptr);
}

HOOK(0x0048C9F0, S_0048c9f0_CopyTargetHandle, "auto");
