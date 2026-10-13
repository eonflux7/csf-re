#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Constructs one 104-byte element at `destination` (cdecl, 5 stack args, caller cleans 0x14).
CSF_FUNC(std::uint32_t*, __cdecl, S_00771690_ConstructElement104, 0x00771130,
         (std::uint32_t* a1, int a2, std::uint32_t* a3, int a4, int a5));

// Grows the 104-byte-element array and inserts one element (__thiscall, ret 12).
CSF_FUNC(std::uint32_t*, __fastcall, S_00771690_InsertElement104, 0x00771690,
         (void* self, void* edx, std::uint32_t* a2, int* a3, int a4));

// Appends one 104-byte element to the array held at self+0x04/0x08/0x0C.
// Uses the spare capacity when available, otherwise delegates to S_00771690_InsertElement104.
static std::uint32_t* __fastcall S_00771690_AppendElement104(
    S_00771690* self, void* /*edx*/, std::uint32_t* element)
{
    // Vtable reference from the original's call site; no store is performed here.
    (void)CSF_VTBL(0x00771690);

    const std::uint32_t begin = self->field_0004;
    std::uint32_t index = 0;

    if (begin != 0)
    {
        index = static_cast<std::uint32_t>(
            static_cast<std::int32_t>(self->field_0008 - begin) / 104);
    }

    if (begin == 0 ||
        index >= static_cast<std::uint32_t>(
                     static_cast<std::int32_t>(self->field_000c - begin) / 104))
    {
        return S_00771690_InsertElement104(
            self, nullptr,
            reinterpret_cast<std::uint32_t*>(&element),
            reinterpret_cast<int*>(static_cast<std::uintptr_t>(self->field_0008)),
            static_cast<int>(reinterpret_cast<std::uintptr_t>(element)));
    }

    const std::uint32_t end = self->field_0008;
    std::uint32_t* result = S_00771690_ConstructElement104(
        reinterpret_cast<std::uint32_t*>(static_cast<std::uintptr_t>(end)), 1,
        element,
        static_cast<int>(reinterpret_cast<std::uintptr_t>(self)),
        static_cast<int>(reinterpret_cast<std::uintptr_t>(element)));

    self->field_0008 = end + 104u;
    return result;
}

HOOK(0x00771700, S_00771690_AppendElement104, "auto");
