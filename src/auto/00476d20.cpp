#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_GLOBAL(float, g_fZero, 0x0082D4CC);

static void __fastcall C_008316d8_ResetFireTimer(C_008316d8* self, void* /*edx*/)
{
    const float timer = CSF_AT(float, reinterpret_cast<void*>(self->field_0040), 0x128);
    self->field_0010 = timer;
    self->field_0014 = !(timer > g_fZero);
}

HOOK(0x00476D20, C_008316d8_ResetFireTimer, "auto");
