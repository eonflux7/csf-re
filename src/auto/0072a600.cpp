#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

static std::uint8_t __fastcall CGuiPanel_ToggleComplementaryProgress(
    CGuiPanel* self, void* /*edx*/)
{
    const int mode =
        reinterpret_cast<int(__fastcall*)(void*, void*)>(
            CSF_AT(void**, self, 0)[0x194 / 4])(self, nullptr);

    if (mode != 1)
        return static_cast<std::uint8_t>(mode);

    const std::uint8_t previousFlag = self->field_017c;
    const double complementaryProgress =
        static_cast<double>(self->field_0178) -
        static_cast<double>(self->field_0180);
    void* const linkedControl =
        reinterpret_cast<void*>(self->field_014c);
    const int toggledFlag = previousFlag == 0;

    self->field_0180 = static_cast<float>(complementaryProgress);
    self->field_017c = static_cast<std::uint8_t>(toggledFlag);

    if (linkedControl)
    {
        const float forwardedProgress = static_cast<float>(
            static_cast<double>(self->field_0178) -
            complementaryProgress);

        return static_cast<std::uint8_t>(
            reinterpret_cast<int(__fastcall*)(void*, void*, float, int)>(
                CSF_AT(void**, linkedControl, 0)[0x198 / 4])(
                    linkedControl, nullptr, forwardedProgress, toggledFlag));
    }

    return static_cast<std::uint8_t>(toggledFlag);
}

HOOK(0x0072A600, CGuiPanel_ToggleComplementaryProgress, "auto");
