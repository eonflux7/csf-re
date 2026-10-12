#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Constructs one 20-byte element at `destination` (cdecl, 5 stack args, caller cleans 0x14).
CSF_FUNC(std::uint32_t*, __cdecl, S_00559d90_ConstructElement20, 0x00559970,
         (std::uint32_t* a1, int a2, std::uint32_t* a3, int a4, int a5));

// Grows the 20-byte-element array and inserts one element (__thiscall, ret 12).
CSF_FUNC(std::uint32_t*, __fastcall, S_00559d90_InsertElement20, 0x00559D90,
         (void* self, void* edx, std::uint32_t* a2, char* a3, int* a4));

// Appends one 20-byte element to the array held at self+0x04/0x08/0x0C (mp_rules sabotage/team table).
// Uses the spare capacity when available, otherwise delegates to S_00559d90_InsertElement20.
static std::uint32_t* __fastcall S_00559d90_AppendElement20(
    S_00559d90* self, void* /*edx*/, int* element)
{
    // Vtable reference from the original's call site; no store is performed here.
    (void)CSF_VTBL(0x00559D90);

    const std::uint32_t begin = self->field_0004;
    std::uint32_t index = 0;

    if (begin != 0)
    {
        index = static_cast<std::uint32_t>(
            static_cast<std::int32_t>(self->field_0008 - begin) / 20);
    }

    if (begin == 0 ||
        index >= static_cast<std::uint32_t>(
                     static_cast<std::int32_t>(self->field_000c - begin) / 20))
    {
        return S_00559d90_InsertElement20(
            self, nullptr,
            reinterpret_cast<std::uint32_t*>(&element),
            reinterpret_cast<char*>(static_cast<std::uintptr_t>(self->field_0008)),
            reinterpret_cast<int*>(static_cast<std::intptr_t>(reinterpret_cast<std::intptr_t>(element))));
    }

    const std::uint32_t end = self->field_0008;
    std::uint32_t* result = S_00559d90_ConstructElement20(
        reinterpret_cast<std::uint32_t*>(static_cast<std::uintptr_t>(end)), 1,
        reinterpret_cast<std::uint32_t*>(element),
        static_cast<int>(reinterpret_cast<std::intptr_t>(self)),
        static_cast<int>(reinterpret_cast<std::intptr_t>(element)));

    self->field_0008 = end + 20u;
    return result;
}

HOOK(0x00559E00, S_00559d90_AppendElement20, "auto");
