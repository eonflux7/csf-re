#include <cstdint>
#include "hook.h"
#include "types/auto_classes.h"

static int __fastcall S_004fb820_Initialize(S_004fb820* self, void* /*edx*/)
{
    self->field_0014 = 0;
    self->field_0018 = 15;
    self->field_0004 = 0;
    self->field_0034 = 15;
    self->field_0030 = 0;
    self->field_0020 = 0;
    self->field_0038 = 0;
    self->field_0039 = 0;
    return reinterpret_cast<int>(self);
}

HOOK(0x004FB820, S_004fb820_Initialize, "auto");
