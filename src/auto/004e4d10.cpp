#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void, __fastcall, sub_4E4210, 0x004E4210, (void* self, void* edx, float a2));
CSF_FUNC(char, __fastcall, sub_4E4350, 0x004E4350, (void* self, void* edx, int a2));
CSF_FUNC(void, __fastcall, sub_4E4750, 0x004E4750, (void* self, void* edx, float a2));

CSF_GLOBAL(float, g_fZero, 0x0082D4CC);

// Dispatches on the object's mode (field_004c): mode 1 goes to sub_4E4350,
// mode 2 goes to sub_4E4750 (both receive the raw int argument reinterpreted
// as a float for the latter). Afterwards, if the object's field_0044 is
// non-negative, sub_4E4210 is called with the same argument.
static void __fastcall C_0083e7d8_DispatchByModeAndSign(C_0083e7d8* self, void* /*edx*/, int a2)
{
    // Read once: the original keeps the value in eax across both compares.
    const uint32_t mode = self->field_004c;

    if (mode == 1)
    {
        sub_4E4350(self, nullptr, a2);
    }
    else if (mode == 2)
    {
        sub_4E4750(self, nullptr, *reinterpret_cast<float*>(&a2));
    }

    // field_0044 is reloaded here, after any callee may have changed it.
    if (self->field_0044 >= g_fZero)
    {
        sub_4E4210(self, nullptr, *reinterpret_cast<float*>(&a2));
    }
}

HOOK(0x004E4D10, C_0083e7d8_DispatchByModeAndSign, "auto");
