#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(uint32_t*, __fastcall, C_00840dac_ConstructBase, 0x0078C830, (void* self, void* edx));

static uint32_t* __fastcall C_00840dac_ConstructMenuState(C_00840dac* self, void* /*edx*/)
{
    C_00840dac_ConstructBase(self, nullptr);
    self->vtbl = CSF_VTBL(0x00840DAC);
    return reinterpret_cast<uint32_t*>(self);
}

HOOK(0x0051B4C0, C_00840dac_ConstructMenuState, "auto");
