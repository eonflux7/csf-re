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

// Constructs one 44-byte element at `destination` (cdecl, 5 stack args, caller cleans 0x14).
CSF_FUNC(int, __cdecl, S_00773060_ConstructElement44, 0x007728B0,
         (int a1, int a2, void* a3, int a4, int a5));

// Grows the 44-byte-element array and inserts one element (__thiscall, ret 12).
CSF_FUNC(uint32_t*, __fastcall, S_00773060_InsertElement44, 0x00773060,
         (void* self, void* edx, uint32_t* a2, void* a3, void* a4));

// Appends one 44-byte element to the array held at self+0x04/0x08/0x0C.
// Uses the spare capacity when available, otherwise delegates to S_00773060_InsertElement44.
static uint32_t* __fastcall S_00773060_AppendElement44(
    S_00773060* self, void* /*edx*/, int element)
{
    // Vtable reference from the original's call site; no store is performed here.
    (void)CSF_VTBL(0x00773060);

    const std::uint32_t begin = self->field_0004;
    std::uint32_t index = 0;

    if (begin != 0)
    {
        index = static_cast<std::uint32_t>(
            static_cast<std::int32_t>(self->field_0008 - begin) / 44);
    }

    if (begin == 0 ||
        index >= static_cast<std::uint32_t>(
                     static_cast<std::int32_t>(self->field_000c - begin) / 44))
    {
        return S_00773060_InsertElement44(
            self, nullptr,
            reinterpret_cast<std::uint32_t*>(&element),
            reinterpret_cast<void*>(static_cast<std::uintptr_t>(self->field_0008)),
            reinterpret_cast<void*>(static_cast<std::intptr_t>(element)));
    }

    const std::uint32_t end = self->field_0008;
    const int constructed = S_00773060_ConstructElement44(
        static_cast<int>(end), 1,
        reinterpret_cast<void*>(static_cast<std::intptr_t>(element)),
        static_cast<int>(reinterpret_cast<std::intptr_t>(self)),
        element);

    self->field_0008 = end + 44u;
    return reinterpret_cast<uint32_t*>(static_cast<std::uintptr_t>(static_cast<std::uint32_t>(constructed)));
}

HOOK(0x007730D0, S_00773060_AppendElement44, "auto");
