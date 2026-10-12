#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

static uint32_t* __fastcall C_008443c8_Construct(
    C_008443c8* self, void* /*edx*/, int value)
{
    self->vtbl = CSF_VTBL(0x008443C8);
    self->field_0004 = static_cast<uint32_t>(value);
    return reinterpret_cast<uint32_t*>(self);
}

HOOK(0x0054E500, C_008443c8_Construct, "auto");
