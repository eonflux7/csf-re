#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(S_00692b80*, __fastcall, S_00692b80_FindProperty, 0x006932B0, (S_00692b80* self, void* , char* name));
CSF_FUNC(void, __fastcall, CoreZlibLog_ReadVector4, 0x00693140, (std::uint8_t* self, void* , float* output));

// Looks up the named property on this object. If found, reads its vector4 value
// into the output block; otherwise zeroes all four output dwords.
static void __fastcall Hud_ReadColor(S_00692b80* self, void* /*edx*/, char* String1, std::uint8_t* outputColor)
{
    S_00692b80* foundProperty = S_00692b80_FindProperty(self, nullptr, String1);
    if (foundProperty != nullptr)
    {
        CoreZlibLog_ReadVector4(reinterpret_cast<std::uint8_t*>(foundProperty), nullptr,
                                reinterpret_cast<float*>(outputColor));
    }
    else
    {
        CSF_AT(int, outputColor, 0x0C) = 0;
        CSF_AT(int, outputColor, 0x08) = 0;
        CSF_AT(int, outputColor, 0x04) = 0;
        CSF_AT(int, outputColor, 0x00) = 0;
    }
}

HOOK(0x00693A90, Hud_ReadColor, "auto");
