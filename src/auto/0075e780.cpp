#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Constructs one 72-byte element at the given slot (cdecl, 5 stack args, caller cleans 0x14).
CSF_FUNC(std::uint32_t*, __cdecl, S_0075e710_ConstructElement72, 0x0075E130,
         (std::uint32_t* a1, int a2, std::uint32_t* a3, int a4, int a5));

// Grows the 72-byte-element array and inserts one element (__thiscall, ret 12).
CSF_FUNC(std::uint32_t*, __fastcall, S_0075e710_InsertElement72, 0x0075E710,
         (void* self, void* edx, std::uint32_t* a2, char* a3, int a4));

// Appends one 72-byte element to the array held at self+0x04/0x08/0x0C.
// Uses the spare capacity when available, otherwise delegates to S_0075e710_InsertElement72.
static std::uint32_t* __fastcall S_0075e710_AppendElement72(
    S_0075e710* self, void* /*edx*/, std::uint32_t* element)
{
    // Vtable reference from the original's call site; no store is performed here.
    (void)CSF_VTBL(0x0075E710);

    const std::uint32_t begin = self->field_0004;
    std::uint32_t count = 0;

    if (begin != 0)
    {
        count = static_cast<std::uint32_t>(
            static_cast<std::int32_t>(self->field_0008 - begin) / 72);
    }

    if (begin == 0 ||
        count >= static_cast<std::uint32_t>(
                     static_cast<std::int32_t>(self->field_000c - begin) / 72))
    {
        return S_0075e710_InsertElement72(
            self, nullptr,
            reinterpret_cast<std::uint32_t*>(&element),
            reinterpret_cast<char*>(static_cast<std::uintptr_t>(self->field_0008)),
            static_cast<int>(reinterpret_cast<std::intptr_t>(element)));
    }

    const std::uint32_t slot = self->field_0008;
    std::uint32_t* const result = S_0075e710_ConstructElement72(
        reinterpret_cast<std::uint32_t*>(static_cast<std::uintptr_t>(slot)), 1,
        element,
        static_cast<int>(reinterpret_cast<std::intptr_t>(self)),
        static_cast<int>(reinterpret_cast<std::intptr_t>(element)));

    self->field_0008 = slot + 0x48u;
    return result;
}

HOOK(0x0075E780, S_0075e710_AppendElement72, "auto");
