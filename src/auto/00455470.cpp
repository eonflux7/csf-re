#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Constructs one 24-byte element at `destination` (cdecl, 5 stack args, caller cleans 0x14).
CSF_FUNC(void, __cdecl, ConstructElement24At, 0x00453300,
         (std::uint32_t* a1, int a2, std::uint32_t* a3, int a4, int a5));

// Grows the 24-byte-element array and inserts one element (__thiscall, ret 12).
CSF_FUNC(std::uint32_t*, __fastcall, GrowAndInsertElement24, 0x00454C80,
         (void* self, void* edx, std::uint32_t* a2, char* a3, std::uint32_t* a4));

// Appends one 24-byte element to the array held at self+0x04/0x08/0x0C.
// Uses the spare capacity when available, otherwise delegates to GrowAndInsertElement24.
static void __fastcall S_00454c80_AppendElement24(
    S_00454c80* self, void* /*edx*/, std::uint32_t* element)
{
    // Vtable reference from the original's call site; no store is performed here.
    (void)CSF_VTBL(0x00454C80);

    const std::uint32_t begin = self->field_0004;
    std::uint32_t count = 0;

    if (begin != 0)
    {
        count = static_cast<std::uint32_t>(
            static_cast<std::int32_t>(self->field_0008 - begin) / 24);
    }

    if (begin != 0 &&
        count < static_cast<std::uint32_t>(
                    static_cast<std::int32_t>(self->field_000c - begin) / 24))
    {
        const std::uint32_t end = self->field_0008;
        ConstructElement24At(
            reinterpret_cast<std::uint32_t*>(static_cast<std::uintptr_t>(end)),
            1,
            element,
            static_cast<int>(reinterpret_cast<std::intptr_t>(self)),
            static_cast<int>(reinterpret_cast<std::intptr_t>(element)));

        self->field_0008 = end + 24u;
    }
    else
    {
        GrowAndInsertElement24(
            self, nullptr,
            reinterpret_cast<std::uint32_t*>(&element),
            reinterpret_cast<char*>(static_cast<std::uintptr_t>(self->field_0008)),
            element);
    }
}

HOOK(0x00455470, S_00454c80_AppendElement24, "auto");
