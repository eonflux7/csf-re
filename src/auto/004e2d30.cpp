#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_GLOBAL(float, g_fIntroVideoDuration, 0x0082E118);

static bool __fastcall C_0083e248_CheckStateSevenOrFiveOverDuration(C_0083e248* self, void* /*edx*/)
{
    uint32_t state = self->field_0018;
    if (state == 7)
        return true;
    if (state == 5 && self->field_0024 > g_fIntroVideoDuration)
        return true;
    return false;
}

HOOK(0x004E2D30, C_0083e248_CheckStateSevenOrFiveOverDuration, "auto");
