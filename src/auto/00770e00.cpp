#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Constructs one 12-byte element at `destination` (cdecl, 5 stack args, caller cleans 0x14).
CSF_FUNC(std::uint32_t*, __cdecl, S_00770ce0_ConstructElement12, 0x00770500,
         (std::uint32_t* a1, int a2, std::uint32_t* a3, int a4, int a5));

// Grows the 12-byte-element array and inserts one element (__thiscall, ret 12).
CSF_FUNC(std::uint32_t*, __fastcall, S_00770ce0_InsertElement12, 0x00770CE0,
         (void* self, void* edx, std::uint32_t* a2, char* a3, std::uint32_t* a4));

// Appends one 12-byte element to the array held at self+0x04/0x08/0x0C.
// Uses the spare capacity when available, otherwise delegates to S_00770ce0_InsertElement12.
static std::uint32_t* __fastcall S_00770ce0_AppendElement12(
    S_00770ce0* self, void* /*edx*/, std::uint32_t* a3)
{
    // Vtable reference from the original's call site; no store is performed here.
    (void)CSF_VTBL(0x00770CE0);

    const std::uint32_t begin = self->field_0004;
    std::uint32_t index = 0;

    if (begin != 0)
    {
        index = static_cast<std::uint32_t>(
            static_cast<std::int32_t>(self->field_0008 - begin) / 12);
    }

    if (begin == 0 ||
        index >= static_cast<std::uint32_t>(
                     static_cast<std::int32_t>(self->field_000c - begin) / 12))
    {
        return S_00770ce0_InsertElement12(
            self, nullptr,
            reinterpret_cast<std::uint32_t*>(&a3),
            reinterpret_cast<char*>(static_cast<std::uintptr_t>(self->field_0008)),
            a3);
    }

    const std::uint32_t end = self->field_0008;
    std::uint32_t* result = S_00770ce0_ConstructElement12(
        reinterpret_cast<std::uint32_t*>(static_cast<std::uintptr_t>(end)), 1,
        a3,
        static_cast<int>(reinterpret_cast<std::intptr_t>(self)),
        static_cast<int>(reinterpret_cast<std::intptr_t>(a3)));

    self->field_0008 = end + 12u;
    return result;
}

HOOK(0x00770E00, S_00770ce0_AppendElement12, "auto");
