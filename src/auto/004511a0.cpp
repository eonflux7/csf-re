#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, GrowButtonArray, 0x0041E5F0, (void* self, void* edx, int growth));
CSF_FUNC(bool, __stdcall, ButtonSelection_MatchesPosition, 0x0044F3E0, (void* a1, float* a4, float arg8));
CSF_FUNC(char, __stdcall, ButtonSelection_MatchesCriteria, 0x0044FFF0, (void* a1, int* a2));

static const auto g_ReportedArrayVtable = CSF_VTBL(0x0041E5F0);

static int __stdcall S_0041e5f0_CollectEligibleButtons(
    S_0041e5f0* destination,
    float* position,
    int* criteria,
    uint32_t* source,
    float tolerance)
{
    const uint32_t count = source[2];

    for (uint32_t index = 0; index < count; ++index)
    {
        C_00833b88* button =
            reinterpret_cast<C_00833b88**>(source[0])[index];

        if (!button || (button->field_0170 & 1) == 0)
            continue;

        if (!button->vGetField016c())
            continue;

        if (button->vSlot7C())
            continue;

        if (!ButtonSelection_MatchesCriteria(button, criteria))
            continue;

        if (!ButtonSelection_MatchesPosition(button, position, tolerance))
            continue;

        if (destination->field_0008 >= destination->field_0004)
        {
            GrowButtonArray(
                destination, nullptr,
                static_cast<int>(destination->field_000c));
        }

        const uint32_t size = destination->field_0008;
        C_00833b88** slot =
            reinterpret_cast<C_00833b88**>(destination->field_0000) + size;
        destination->field_0008 = size + 1;
        *slot = button;
    }

    return static_cast<int>(count);
}

HOOK(0x004511A0, S_0041e5f0_CollectEligibleButtons, "auto");
