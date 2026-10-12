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

CSF_FUNC(void*, __fastcall, Buffer_PadToMinimumLength, 0x00425CE0, (void* self, void* edx, int a2));

static uint32_t* __fastcall C_0082fe88_InitDefaults(C_0082fe88* self, void* /*edx*/)
{
    self->vtbl = CSF_VTBL(0x0082FE88);

    self->field_0004 = 0;
    self->field_0008 = 0;
    CSF_AT(uint32_t, self, 0x0C) = 0;
    CSF_AT(uint32_t, self, 0x10) = 10;

    self->field_0014 = 0;
    self->field_0018 = 0;
    self->field_001c = 0;
    self->field_0020 = 10;
    self->field_0024 = 0xFFFFFFFF;
    self->field_0028 = 0;

    uint32_t currentLength = self->field_0008;
    if (currentLength < 4) {
        Buffer_PadToMinimumLength(reinterpret_cast<void*>(&self->field_0004), nullptr,
                                  static_cast<int>(4 - currentLength));
    }

    return reinterpret_cast<uint32_t*>(self);
}

HOOK(0x004262C0, C_0082fe88_InitDefaults, "auto");
