#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Note: the disassembly pushes four arguments before the first call (the output slot is the
// fourth), and the caller pops both calls' arguments with a single `add esp, 0x20`.
CSF_FUNC(void*, __cdecl, ResolveReplacementObject, 0x0040AE10,
         (void* a1, void* a2, void* a3, void** a4));
CSF_FUNC(int, __cdecl, RelinkObjectPair, 0x0040B480,
         (int a1, int a2, int a3, int a4));

static void** __fastcall SetTrackedValueSlot(
    S_0040b8e0* self,
    void* /*edx*/,
    void** outputSlot,
    void* currentValue,
    void* previousValue)
{
    if (currentValue != previousValue)
    {
        void* replacement = ResolveReplacementObject(
            currentValue,
            reinterpret_cast<void*>(static_cast<std::uintptr_t>(self->field_0008)),
            previousValue,
            outputSlot);

        RelinkObjectPair(
            static_cast<int>(reinterpret_cast<std::uintptr_t>(replacement)),
            static_cast<int>(self->field_0008),
            static_cast<int>(reinterpret_cast<std::uintptr_t>(self)),
            static_cast<int>(reinterpret_cast<std::uintptr_t>(outputSlot)));

        self->field_0008 = static_cast<uint32_t>(reinterpret_cast<std::uintptr_t>(replacement));
    }

    *outputSlot = currentValue;
    return outputSlot;
}

HOOK(0x0040B8E0, SetTrackedValueSlot, "auto");
