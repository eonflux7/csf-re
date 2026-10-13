#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(uint32_t*, __cdecl, CopyRecordRange_40AE10, 0x0040AE10,
         (uint32_t* srcBegin, uint32_t* srcEnd, uint32_t* dstBegin));
CSF_FUNC(int, __cdecl, EntryTable_FreePointerRange, 0x0040B480,
         (int first, int end, int , int));

static uint32_t* __fastcall SetTrackedValueSlot(
    S_0040b8e0* self,
    void* /*edx*/,
    uint32_t* outputSlot,
    uint32_t* currentValue,
    uint32_t* previousValue)
{
    if (currentValue != previousValue)
    {
        uint32_t* replacement = CopyRecordRange_40AE10(
            previousValue,
            reinterpret_cast<uint32_t*>(static_cast<std::uintptr_t>(self->field_0008)),
            currentValue);

        EntryTable_FreePointerRange(
            static_cast<int>(reinterpret_cast<std::uintptr_t>(replacement)),
            static_cast<int>(self->field_0008),
            static_cast<int>(reinterpret_cast<std::uintptr_t>(self)),
            static_cast<int>(reinterpret_cast<std::uintptr_t>(outputSlot)));

        self->field_0008 = static_cast<uint32_t>(reinterpret_cast<std::uintptr_t>(replacement));
    }

    *outputSlot = static_cast<uint32_t>(reinterpret_cast<std::uintptr_t>(currentValue));
    return outputSlot;
}

HOOK(0x0040B8E0, SetTrackedValueSlot, "auto");
