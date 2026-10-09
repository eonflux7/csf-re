#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, MissionScript_ResetEntryRange, 0x00469710, (int begin, int end));
CSF_FUNC(int, __cdecl, sub_76A0B0, 0x0076A0B0, (int a1, int a2, int a3));

static int* __fastcall InputBindings_SetActionIndex(
    S_0076a4f0* self,
    void* /*edx*/,
    int* outputSlot,
    int newIndex,
    int currentIndex)
{
    if (newIndex != currentIndex)
    {
        uint32_t rangeBegin = static_cast<uint32_t>(
            sub_76A0B0(currentIndex, static_cast<int>(self->field_0008), newIndex));

        MissionScript_ResetEntryRange(
            static_cast<int>(rangeBegin),
            static_cast<int>(self->field_0008));

        self->field_0008 = rangeBegin;
    }

    *outputSlot = newIndex;
    return outputSlot;
}

HOOK(0x0076A4F0, InputBindings_SetActionIndex, "auto");
