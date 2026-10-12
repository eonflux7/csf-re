#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, sub_6B3780, 0x006B3780, (void* self, void* edx, float* a2));
CSF_FUNC(uint32_t*, __fastcall, sub_6B45D0, 0x006B45D0, (void* self, void* edx, int a2, float a3));

CSF_GLOBAL(float, g_ColorBlack, 0x008A7DA0);
CSF_GLOBAL(float, g_ColorBlack_G, 0x008A7DA4);
CSF_GLOBAL(float, g_ColorBlack_B, 0x008A7DA8);
CSF_GLOBAL(float, g_ColorBlack_A, 0x008A7DAC);

// Toggles the black fade phase of the object. Phase 5 resets the phase to 0 and
// loads the black color into the 16-byte color slot at +0x8C, then runs the
// apply step. Any other phase sets the phase to 2 and runs the fade step with
// black and a 0.2 duration.
static uint32_t* __fastcall C_00860288_UpdateBlackFadePhase(C_00860288* self, void* /*edx*/)
{
    if (self->field_0088 == 5)
    {
        self->field_0088 = 0;
        *reinterpret_cast<float*>(&self->field_008c) = g_ColorBlack;
        CSF_AT(float, self, 0x90) = g_ColorBlack_G;
        CSF_AT(float, self, 0x94) = g_ColorBlack_B;
        CSF_AT(float, self, 0x98) = g_ColorBlack_A;
        return (uint32_t*)(sub_6B3780(self, nullptr, reinterpret_cast<float*>(&self->field_008c)));
    }
    else
    {
        self->field_0088 = 2;
        return sub_6B45D0(self, nullptr, (int)CSF_ADDR_DATA(0x008A7DA0), 0.2f);
    }
}

HOOK(0x006B4CA0, C_00860288_UpdateBlackFadePhase, "auto");
