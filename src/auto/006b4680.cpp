#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, C_00860288_ApplyColor, 0x006B3780,
         (void* self, void* edx, float* a2));
CSF_FUNC(uint32_t*, __fastcall, C_00860288_BeginColorTransition, 0x006B45D0,
         (void* self, void* edx, int a2, float a3));

CSF_GLOBAL(float, g_ColorBlack, 0x008A7DA0);

static uint32_t* __fastcall C_00860288_SetColor(
    C_00860288* self, void* /*edx*/, int colorAddress, char transition)
{
    void* color = reinterpret_cast<void*>(colorAddress);

    self->field_0088 = transition ? 1 : 5;
    self->field_008c = CSF_AT(uint32_t, color, 0x00);
    CSF_AT(uint32_t, self, 0x90) = CSF_AT(uint32_t, color, 0x04);
    CSF_AT(uint32_t, self, 0x94) = CSF_AT(uint32_t, color, 0x08);
    CSF_AT(uint32_t, self, 0x98) = CSF_AT(uint32_t, color, 0x0C);

    if (transition)
    {
        if (static_cast<uint8_t>(self->vIsField0088Set()) == 0)
            C_00860288_ApplyColor(self, nullptr, &g_ColorBlack);

        return C_00860288_BeginColorTransition(
            self, nullptr, colorAddress, 0.2f);
    }

    return reinterpret_cast<uint32_t*>(
        C_00860288_ApplyColor(self, nullptr, reinterpret_cast<float*>(color)));
}

HOOK(0x006B4680, C_00860288_SetColor, "auto");
