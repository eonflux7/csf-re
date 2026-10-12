#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Bind lines copied from the packet.
CSF_FUNC(int, __cdecl, sub_7726C0, 0x007726C0, (void* a1, void* a2, int a3));
CSF_FUNC(int, __cdecl, UpdateGameDataRecordLinks, 0x007754B0, (int a1, int a2, int a3, int a4));

static void** __fastcall SetRelinkedAnimValueSlot(
    S_00772940* self,
    void* /*edx*/,
    void** outputSlot,
    void* currentValue,
    void* previousValue)
{
    if (currentValue != previousValue)
    {
        int replacement = sub_7726C0(
            currentValue,
            reinterpret_cast<void*>(static_cast<std::uintptr_t>(self->field_0008)),
            static_cast<int>(reinterpret_cast<std::uintptr_t>(previousValue)));

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
