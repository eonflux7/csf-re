#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, AnimationDefs_CopyRecordRange, 0x007726C0,
         (int sourceBegin, int sourceEnd, int destinationBegin));
CSF_FUNC(int, __cdecl, UpdateGameDataRecordLinks, 0x007754B0,
         (int firstRecord, int endRecord, int , int));

static C_0082d410** __fastcall SetRelinkedAnimValueSlot(
    S_00772940* self,
    void* /*edx*/,
    C_0082d410** outputSlot,
    C_0082d410* currentValue,
    C_0082d410* previousValue)
{
    if (currentValue != previousValue)
    {
        int replacement = AnimationDefs_CopyRecordRange(
            static_cast<int>(reinterpret_cast<std::uintptr_t>(previousValue)),
            static_cast<int>(self->field_0008),
            static_cast<int>(reinterpret_cast<std::uintptr_t>(currentValue)));

        UpdateGameDataRecordLinks(
            replacement,
            static_cast<int>(self->field_0008),
            static_cast<int>(reinterpret_cast<std::uintptr_t>(self)),
            static_cast<int>(reinterpret_cast<std::uintptr_t>(outputSlot)));

        self->field_0008 = static_cast<uint32_t>(replacement);
    }

    *outputSlot = currentValue;
    return outputSlot;
}

HOOK(0x00772940, SetRelinkedAnimValueSlot, "auto");
