#include <cstdint>
#include <cstddef>
#include <cmath>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(uint32_t*, __fastcall, sub_6B8020, 0x006B8020, (void* self, void* edx));
CSF_FUNC(void, __fastcall, sub_6B97A0, 0x006B97A0, (void* self, void* edx));
CSF_GLOBAL(float, g_NearZeroEpsilon, 0x008603A0);

// Sets the float property at offset +0x20 of the attached property block.
// Non-zero values go through the refresh path 0x6B8020 first; near-zero values
// only write when a property block is attached, then call 0x6B97A0.
static void __fastcall GfxObjectProps_SetFloatPropertyAt20(S_006b8020* self, void* /*edx*/, float value)
{
    if (std::fabs(value) > g_NearZeroEpsilon) {
        sub_6B8020(self, nullptr);
        auto* props = reinterpret_cast<uint8_t*>(self->field_0010);
        CSF_AT(float, props, 0x20) = value;
    } else {
        uint32_t propsAddr = self->field_0010;
        if (propsAddr != 0) {
            CSF_AT(float, reinterpret_cast<uint8_t*>(propsAddr), 0x20) = value;
            sub_6B97A0(self, nullptr);
        }
    }
}

HOOK(0x006B9890, GfxObjectProps_SetFloatPropertyAt20, "auto");
