#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Constructs one 12-byte element at `destination` (cdecl, 5 stack args, caller cleans 0x14).
CSF_FUNC(std::uint32_t*, __cdecl, S_00470340_ConstructElement12, 0x0046F0A0,
         (std::uint32_t* a1, int a2, int a3, int a4, int a5));

// Grows the 12-byte-element array and inserts one element (__thiscall, ret 12).
CSF_FUNC(std::uint32_t*, __fastcall, S_00470340_InsertElement12, 0x00470340,
         (void* self, void* edx, std::uint32_t* a2, void* a3, int a4));

// Appends one 12-byte element to the array held at self+0x04/0x08/0x0C.
// Uses the spare capacity when available, otherwise delegates to S_00470340_InsertElement12.
static std::uint32_t* __fastcall S_00470340_AppendElement12(
    S_00470340* self, void* /*edx*/, int element)
{
    // Vtable reference from the original's call site; no store is performed here.
    (void)CSF_VTBL(0x00470340);

    const std::uint32_t begin = self->field_0004;
    std::uint32_t size = 0;

    if (begin != 0)
    {
        size = static_cast<std::uint32_t>(
            static_cast<std::int32_t>(self->field_0008 - begin) / 12);
    }

    if (begin == 0 ||
        size >= static_cast<std::uint32_t>(
                    static_cast<std::int32_t>(self->field_000c - begin) / 12))
    {
        return S_00470340_InsertElement12(
            self, nullptr,
            reinterpret_cast<std::uint32_t*>(&element),
            reinterpret_cast<void*>(static_cast<std::uintptr_t>(self->field_0008)),
            element);
    }

    const std::uint32_t end = self->field_0008;
    std::uint32_t* const result = S_00470340_ConstructElement12(
        reinterpret_cast<std::uint32_t*>(static_cast<std::uintptr_t>(end)), 1,
        element,
        static_cast<int>(reinterpret_cast<std::intptr_t>(self)),
        element);

    self->field_0008 = end + 12u;
    return result;
}

HOOK(0x004709D0, S_00470340_AppendElement12, "auto");
