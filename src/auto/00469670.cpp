#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, MissionScript_CreateSlotEntry, 0x00468470, (int a1, int a2, int a3));
CSF_FUNC(int, __cdecl, MissionScript_UnlinkSlotEntry, 0x00468AF0, (int* a1, int* a2, int a3, int a4));

static int* __fastcall MissionScript_SetValueSlot(
    S_004427c0* self,
    void* /*edx*/,
    int* outputSlot,
    int currentValue,
    int previousValue)
{
    if (currentValue != previousValue)
    {
        int* newEntry = reinterpret_cast<int*>(static_cast<std::uintptr_t>(
            static_cast<uint32_t>(MissionScript_CreateSlotEntry(
                previousValue,
                static_cast<int>(self->field_0008),
                currentValue))));

        MissionScript_UnlinkSlotEntry(
            newEntry,
            reinterpret_cast<int*>(static_cast<std::uintptr_t>(self->field_0008)),
            static_cast<int>(reinterpret_cast<std::uintptr_t>(self)),
            static_cast<int>(reinterpret_cast<std::uintptr_t>(outputSlot)));

        self->field_0008 = static_cast<uint32_t>(reinterpret_cast<std::uintptr_t>(newEntry));
    }

    *outputSlot = currentValue;
    return outputSlot;
}

HOOK(0x00469670, MissionScript_SetValueSlot, "auto");
