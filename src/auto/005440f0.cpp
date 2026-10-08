#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// Thiscall callee at 0x53B540 (one stack arg: this, ret 0).
CSF_FUNC(int, __fastcall, CMenuGameOptions_PrepareSliderBinding, 0x53B540,
         (CMenuGameOptionsPC* self, void* edx));

// Refreshes the attached option sub-object (at +0x24) with the slider value (+0x5C)
// and the mouse-flag byte (+0x60), in the same order and with the same vtable slots
// as the original. Returns the result of the final virtual call, or the raw sub-object
// pointer (0) when there is no sub-object.
static int __fastcall CMenuGameOptionsPC_PushSliderValueToOptions(CMenuGameOptionsPC* self, void* /*edx*/)
{
    int result = static_cast<int>(self->field_0024);
    if (result != 0)
    {
        CMenuGameOptions_PrepareSliderBinding(self, nullptr);

        // Slider value is passed as its raw 32-bit pattern (float bits on the stack).
        uint32_t sliderBits = *reinterpret_cast<uint32_t*>(&self->field_005c);

        void* subObject = reinterpret_cast<void*>(self->field_0024);

        // slot 0x30 / 4 = 12
        ((int(__fastcall*)(void*, void*, uint32_t))CSF_AT(void**, subObject, 0)[12])(
            subObject, nullptr, sliderBits);

        // slot 0x44 / 4 = 17
        ((int(__fastcall*)(void*, void*, uint32_t))CSF_AT(void**, subObject, 0)[17])(
            subObject, nullptr, sliderBits);

        // slot 0x64 / 4 = 25
        uint32_t mouseFlag = static_cast<uint32_t>(CSF_AT(uint8_t, self, 0x60));
        result = ((int(__fastcall*)(void*, void*, uint32_t))CSF_AT(void**, subObject, 0)[25])(
            subObject, nullptr, mouseFlag);
    }
    return result;
}

HOOK(0x005440F0, CMenuGameOptionsPC_PushSliderValueToOptions, "auto");
