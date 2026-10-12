#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Constructs one 196-byte element at `destination` (cdecl, 5 stack args, caller cleans 0x14).
CSF_FUNC(std::uint32_t*, __cdecl, S_00440ee0_ConstructElement196, 0x0043F530,
         (std::uint32_t* a1, int a2, int a3, int a4, int a5));

// Grows the 196-byte-element array and inserts one element (__thiscall, ret 12).
CSF_FUNC(std::uint32_t*, __fastcall, S_00440ee0_InsertElement196, 0x00440EE0,
         (void* self, void* edx, std::uint32_t* a2, char* a3, int a4));

// Appends one 196-byte element to the array held at self+0x04/0x08/0x0C.
// Uses the spare capacity when available, otherwise delegates to S_00440ee0_InsertElement196.
static std::uint32_t* __fastcall S_00440ee0_AppendElement196(
    S_00440ee0* self, void* /*edx*/, int element)
{
    // Vtable reference from the original's call site; no store is performed here.
    (void)CSF_VTBL(0x00440EE0);

    const std::uint32_t begin = self->field_0004;
    std::uint32_t index = 0;

    if (begin != 0)
    {
        index = static_cast<std::uint32_t>(
            static_cast<std::int32_t>(self->field_0008 - begin) / 196);
    }

    if (begin == 0 ||
        index >= static_cast<std::uint32_t>(
                     static_cast<std::int32_t>(self->field_000c - begin) / 196))
    {
        return S_00440ee0_InsertElement196(
            self, nullptr,
            reinterpret_cast<std::uint32_t*>(&element),
            reinterpret_cast<char*>(static_cast<std::uintptr_t>(self->field_0008)),
            element);
    }

    const std::uint32_t end = self->field_0008;
    std::uint32_t* result = S_00440ee0_ConstructElement196(
        reinterpret_cast<std::uint32_t*>(static_cast<std::uintptr_t>(end)), 1,
        element,
        static_cast<int>(reinterpret_cast<std::intptr_t>(self)),
        element);

    self->field_0008 = end + 196u;
    return result;
}

HOOK(0x004413B0, S_00440ee0_AppendElement196, "auto");
