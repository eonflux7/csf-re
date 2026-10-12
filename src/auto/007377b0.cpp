#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Hud_ColorsMatchRgba(float* self, void* edx, float* other): ecx = this+0x68 (float*), one stack arg.
CSF_FUNC(int, __fastcall, Hud_ColorsMatchRgba, 0x0042D680, (float* self, void* /*edx*/, float* other));

// Copies a 4-dword RGBA block into this+0x68 when it differs from the current color.
// Sets the dirty flag at this+0x20 when a copy happens.
static int __fastcall C_00867260_CopyRgbaIfChanged(C_00867260* self, void* /*edx*/, int newRgbaPtr)
{
    float* colorDest = reinterpret_cast<float*>(reinterpret_cast<uint8_t*>(self) + 0x68);
    int result = Hud_ColorsMatchRgba(colorDest, nullptr, reinterpret_cast<float*>(newRgbaPtr));
    if (static_cast<uint8_t>(result) == 0)
    {
        CSF_AT(uint8_t, self, 0x20) = 1;
        CSF_AT(uint32_t, colorDest, 0) = CSF_AT(uint32_t, reinterpret_cast<void*>(newRgbaPtr), 0);
        CSF_AT(uint32_t, colorDest, 4) = CSF_AT(uint32_t, reinterpret_cast<void*>(newRgbaPtr), 4);
        CSF_AT(uint32_t, colorDest, 8) = CSF_AT(uint32_t, reinterpret_cast<void*>(newRgbaPtr), 8);
        result = static_cast<int>(CSF_AT(uint32_t, reinterpret_cast<void*>(newRgbaPtr), 12));
        CSF_AT(uint32_t, colorDest, 12) = static_cast<uint32_t>(result);
    }
    return result;
}

HOOK(0x007377B0, C_00867260_CopyRgbaIfChanged, "auto");
