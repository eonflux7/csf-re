#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

static uint32_t* __fastcall C_008310d8_Initialize(C_008310d8* self, void* /*edx*/)
{
    self->vtbl = CSF_VTBL(0x008310D8);
    self->field_0054 = 0;
    self->field_0058 = 0;
    self->field_005c = 0;
    self->field_0060 = 10;
    reinterpret_cast<uint8_t*>(&self->field_0004)[0] = 0;
    return reinterpret_cast<uint32_t*>(self);
}

HOOK(0x00444FB0, C_008310d8_Initialize, "auto");
