#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Constructs one 348-byte element at `destination` (cdecl, 5 stack args, caller cleans 0x14).
CSF_FUNC(int, __cdecl, S_00782f90_ConstructElement348, 0x007829D0,
         (int a1, int a2, int a3, int a4, int a5));

// Grows the 348-byte-element array and inserts one element (__thiscall, ret 12).
CSF_FUNC(std::uint32_t*, __fastcall, S_00782f90_InsertElement348, 0x00782F90,
         (void* self, void* edx, std::uint32_t* a2, void* a3, void* a4));

// Appends one 348-byte element to the array held at self+0x04/0x08/0x0C.
// Uses the spare capacity when available, otherwise delegates to S_00782f90_InsertElement348.
static std::uint32_t* __fastcall S_00782f90_AppendElement348(
    S_00782f90* self, void* /*edx*/, C_0082d410* element)
{
    // Vtable reference from the original's call site; no store is performed here.
    (void)CSF_VTBL(0x00782F90);

    const std::uint32_t begin = self->field_0004;
    std::uint32_t index = 0;

    if (begin != 0)
    {
        index = static_cast<std::uint32_t>(
            static_cast<std::int32_t>(self->field_0008 - begin) / 348);
    }

    if (begin == 0 ||
        index >= static_cast<std::uint32_t>(
                     static_cast<std::int32_t>(self->field_000c - begin) / 348))
    {
        return S_00782f90_InsertElement348(
            self, nullptr,
            reinterpret_cast<std::uint32_t*>(&element),
            reinterpret_cast<void*>(static_cast<std::uintptr_t>(self->field_0008)),
            reinterpret_cast<void*>(element));
    }

    const std::uint32_t end = self->field_0008;
    std::uint32_t* result = reinterpret_cast<std::uint32_t*>(
        static_cast<std::intptr_t>(S_00782f90_ConstructElement348(
            static_cast<int>(end), 1,
            static_cast<int>(reinterpret_cast<std::intptr_t>(element)),
            static_cast<int>(reinterpret_cast<std::intptr_t>(self)),
            static_cast<int>(reinterpret_cast<std::intptr_t>(element)))));

    self->field_0008 = end + 348u;
    return result;
}

HOOK(0x00783000, S_00782f90_AppendElement348, "auto");
