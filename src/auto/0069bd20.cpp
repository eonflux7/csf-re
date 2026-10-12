#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Disassembly pushes four dwords before the first call (arg8, field_0008, arg4, a4) and the
// caller pops both calls' arguments with one `add esp, 0x20`, so the callee takes four arguments.
CSF_FUNC(int, __cdecl, ResolveReplacementObject, 0x0069B8B0,
         (int a1, int a2, int a3, int a4));
CSF_FUNC(int, __cdecl, RelinkObjectPair, 0x0069B860,
         (int a1, int a2, int a3, int a4));

static int* __fastcall SetRelinkedValueSlot(
    S_0069bd20* self,
    void* /*edx*/,
    int* outputSlot,
    int newValue,
    int oldValue)
{
    if (newValue != oldValue)
    {
        int replacement = ResolveReplacementObject(
            oldValue,
            static_cast<int>(self->field_0008),
            newValue,
            static_cast<int>(reinterpret_cast<std::uintptr_t>(outputSlot)));

        RelinkObjectPair(
            replacement,
            static_cast<int>(self->field_0008),
            static_cast<int>(reinterpret_cast<std::uintptr_t>(self)),
            static_cast<int>(reinterpret_cast<std::uintptr_t>(outputSlot)));

        self->field_0008 = static_cast<uint32_t>(replacement);
    }

    *outputSlot = newValue;
    return outputSlot;
}

HOOK(0x0069BD20, SetRelinkedValueSlot, "auto");
