#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Constructs one 80-byte element at `end` (cdecl, 5 stack args, caller cleans 0x14).
CSF_FUNC(std::uint32_t*, __cdecl, S_00756260_ConstructElement80, 0x00755E00,
         (std::uint32_t* a1, int a2, std::uint32_t* a3, int a4, int a5));

// Grows the 80-byte-element array and inserts one element (__thiscall, ret 12).
CSF_FUNC(std::uint32_t*, __fastcall, S_00756260_InsertElement80, 0x00756260,
         (void* self, void* edx, std::uint32_t* a2, char* a3, int a4));

// Appends one 80-byte element to the array held at self+0x04/0x08/0x0C.
// Uses the spare capacity when available, otherwise delegates to S_00756260_InsertElement80.
static std::uint32_t* __fastcall S_00756260_AppendElement80(
    S_00756260* self, void* /*edx*/, std::uint32_t* element)
{
    // Vtable reference from the original's call site; no store is performed here.
    (void)CSF_VTBL(0x00756260);

    const std::uint32_t begin = self->field_0004;
    std::uint32_t size = 0;

    if (begin != 0)
    {
        size = static_cast<std::uint32_t>(
            static_cast<std::int32_t>(self->field_0008 - begin) / 80);
    }

    if (begin == 0 ||
        size >= static_cast<std::uint32_t>(
                    static_cast<std::int32_t>(self->field_000c - begin) / 80))
    {
        return S_00756260_InsertElement80(
            self, nullptr,
            reinterpret_cast<std::uint32_t*>(&element),
            reinterpret_cast<char*>(static_cast<std::uintptr_t>(self->field_0008)),
            static_cast<int>(reinterpret_cast<std::intptr_t>(element)));
    }

    const std::uint32_t end = self->field_0008;
    std::uint32_t* const result = S_00756260_ConstructElement80(
        reinterpret_cast<std::uint32_t*>(static_cast<std::uintptr_t>(end)), 1,
        element,
        static_cast<int>(reinterpret_cast<std::intptr_t>(self)),
        static_cast<int>(reinterpret_cast<std::intptr_t>(element)));

    self->field_0008 = end + 80u;
    return result;
}

HOOK(0x00756320, S_00756260_AppendElement80, "auto");
