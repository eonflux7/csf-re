#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Constructs one 36-byte element at `destination` (cdecl, 5 stack args, caller cleans 0x14).
CSF_FUNC(int, __cdecl, S_006c9710_ConstructElement36, 0x006C91B0,
         (int a1, int a2, int a3, int a4, int a5));

// Grows the 36-byte-element array and inserts one element (__thiscall, ret 12).
CSF_FUNC(std::uint32_t*, __fastcall, S_006c9710_InsertElement36, 0x006C9710,
         (void* self, void* edx, std::uint32_t* a2, int a3, int a4));

// Appends one 36-byte element to the array held at self+0x04/0x08/0x0C.
// Uses the spare capacity when available, otherwise delegates to S_006c9710_InsertElement36.
static std::uint32_t* __fastcall S_006c9710_AppendElement36(
    S_006c9710* self, void* /*edx*/, int element)
{
    // Vtable reference from the original's call site; no store is performed here.
    (void)CSF_VTBL(0x006C9710);

    const std::uint32_t begin = self->field_0004;
    std::uint32_t index = 0;

    if (begin != 0)
    {
        index = static_cast<std::uint32_t>(
            static_cast<std::int32_t>(self->field_0008 - begin) / 36);
    }

    if (begin == 0 ||
        index >= static_cast<std::uint32_t>(
                     static_cast<std::int32_t>(self->field_000c - begin) / 36))
    {
        return S_006c9710_InsertElement36(
            self, nullptr,
            reinterpret_cast<std::uint32_t*>(&element),
            static_cast<int>(self->field_0008),
            element);
    }

    const std::uint32_t end = self->field_0008;
    const int constructed = S_006c9710_ConstructElement36(
        static_cast<int>(end), 1,
        element,
        static_cast<int>(reinterpret_cast<std::intptr_t>(self)),
        element);
    std::uint32_t* result = reinterpret_cast<std::uint32_t*>(
        static_cast<std::uintptr_t>(constructed));

    self->field_0008 = end + 36u;
    return result;
}

HOOK(0x006C99E0, S_006c9710_AppendElement36, "auto");
