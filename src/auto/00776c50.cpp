#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Constructs one 52-byte element at `destination` (cdecl, 5 stack args, caller cleans 0x14).
CSF_FUNC(int, __cdecl, S_00776380_ConstructElement52, 0x00775250,
         (int a1, int a2, int a3, int a4, int a5));

// Grows the 52-byte-element array and inserts one element (__thiscall, ret 12).
CSF_FUNC(std::uint32_t*, __fastcall, S_00776380_InsertElement52, 0x00776380,
         (void* self, void* edx, std::uint32_t* a2, int a3, int a4));

// Appends one 52-byte element to the array held at self+0x04/0x08/0x0C.
// Uses the spare capacity when available, otherwise delegates to S_00776380_InsertElement52.
static std::uint32_t* __fastcall S_00776380_AppendElement52(
    S_00776380* self, void* /*edx*/, int element)
{
    // Vtable reference from the original's call site; no store is performed here.
    (void)CSF_VTBL(0x00776380);

    const std::uint32_t begin = self->field_0004;
    std::uint32_t index = 0;

    if (begin != 0)
    {
        index = static_cast<std::uint32_t>(
            static_cast<std::int32_t>(self->field_0008 - begin) / 52);
    }

    if (begin == 0 ||
        index >= static_cast<std::uint32_t>(
                     static_cast<std::int32_t>(self->field_000c - begin) / 52))
    {
        return S_00776380_InsertElement52(
            self, nullptr,
            reinterpret_cast<std::uint32_t*>(&element),
            static_cast<int>(self->field_0008),
            element);
    }

    const std::uint32_t end = self->field_0008;
    const int constructed = S_00776380_ConstructElement52(
        static_cast<int>(end), 1,
        element,
        static_cast<int>(reinterpret_cast<std::intptr_t>(self)),
        element);

    self->field_0008 = end + 52u;
    return reinterpret_cast<std::uint32_t*>(static_cast<std::uintptr_t>(static_cast<std::uint32_t>(constructed)));
}

HOOK(0x00776C50, S_00776380_AppendElement52, "auto");
