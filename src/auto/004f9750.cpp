#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, C_00830438_Construct, 0x00712CC0, (void* self, void* edx));

static uint32_t* __fastcall C_0083f1f8_Construct(C_0083f1f8* self, void* /*edx*/)
{
    C_00830438_Construct(self, nullptr);
    self->field_00f0 = 0;
    self->field_00f4 = 0;
    self->field_0010 = 0;
    self->field_000c = 0;
    self->field_0012 = 0;
    self->field_00dc = 0;
    self->field_00e0 = 0;
    self->field_00e4 = 0;
    self->field_00e8 = 0;
    self->field_00ec = 0;
    self->field_0108 = 0;
    self->field_0109 = 0;
    *reinterpret_cast<void***>(self) = CSF_VTBL(0x0083F1F8);
    self->field_00fc = 1056964608;
    self->field_0104 = static_cast<uint32_t>(-1);
    self->field_00f8 = 1;
    return reinterpret_cast<uint32_t*>(self);
}

HOOK(0x004F9750, C_0083f1f8_Construct, "auto");
