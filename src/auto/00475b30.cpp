#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

static int __fastcall C_008316d8_ResetStateFromSource(
    C_008316d8* self, void* /*edx*/)
{
    void* source = reinterpret_cast<void*>(self->field_0040);

    self->field_0014 = 1;
    self->field_0010 = 0.0f;
    self->field_001c = CSF_AT(uint32_t, source, 0x104);
    self->field_0020 = CSF_AT(uint32_t, source, 0x108);

    const int result = CSF_AT(int32_t, source, 0x110);
    self->field_0024 = static_cast<uint32_t>(result);
    self->field_0016 = 0;
    self->field_0018 = 0.0f;
    return result;
}

HOOK(0x00475B30, C_008316d8_ResetStateFromSource, "auto");
