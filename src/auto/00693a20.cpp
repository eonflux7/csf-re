#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(S_00692b80*, __fastcall, S_00692b80_FindProperty, 0x006932B0, (S_00692b80* self, void* , char* name));
CSF_FUNC(void, __fastcall, CoreZlibLog_ReadVector2, 0x00692F60, (std::uint8_t* self, void* , float* output));

// Looks up a property by name; if found, reads a two-float vector into the
// output buffer, otherwise zeroes both output dwords.
static void __fastcall HudReadFloatPair(S_00692b80* self, void* /*edx*/, char* String1, int32_t* outputWords)
{
    S_00692b80* property = S_00692b80_FindProperty(self, nullptr, String1);
    if (property != nullptr)
    {
        CoreZlibLog_ReadVector2(reinterpret_cast<std::uint8_t*>(property), nullptr,
                                reinterpret_cast<float*>(outputWords));
    }
    else
    {
        outputWords[0] = 0;
        outputWords[1] = 0;
    }
}

HOOK(0x693A20, HudReadFloatPair, "auto");
