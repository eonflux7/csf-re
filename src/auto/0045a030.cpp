#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Constructs one 40-byte element at `destination` (cdecl, 5 stack args, caller cleans 0x14).
CSF_FUNC(void, __cdecl, S_00459ec0_ConstructElement40, 0x00458660,
         (char* a1, int a2, void* a3, int a4, int a5));

// Grows the 40-byte-element array and inserts one element (__thiscall, ret 12).
CSF_FUNC(std::uint32_t*, __fastcall, S_00459ec0_InsertElement40, 0x00459EC0,
         (void* self, void* edx, std::uint32_t* a2, char* a3, void* a4));

// Appends one 40-byte element to the array held at self+0x04/0x08/0x0C.
// Uses the spare capacity when available, otherwise delegates to S_00459ec0_InsertElement40.
static void __fastcall S_00459ec0_AppendElement40(
    S_00459ec0* self, void* /*edx*/, void* element)
{
    // Vtable reference from the original's call site; no store is performed here.
    (void)CSF_VTBL(0x00459EC0);

    const std::uint32_t begin = self->field_0004;
    std::uint32_t index = 0;

    if (begin != 0)
    {
        index = static_cast<std::uint32_t>(
            static_cast<std::int32_t>(self->field_0008 - begin) / 40);
    }

    if (begin != 0 &&
        index < static_cast<std::uint32_t>(
                    static_cast<std::int32_t>(self->field_000c - begin) / 40))
    {
        const std::uint32_t slot = self->field_0008;
        S_00459ec0_ConstructElement40(
            reinterpret_cast<char*>(static_cast<std::uintptr_t>(slot)), 1,
            element,
            static_cast<int>(reinterpret_cast<std::intptr_t>(self)),
            static_cast<int>(reinterpret_cast<std::intptr_t>(element)));

        self->field_0008 = slot + 40u;
        return;
    }

    S_00459ec0_InsertElement40(
        self, nullptr,
        reinterpret_cast<std::uint32_t*>(&element),
        reinterpret_cast<char*>(static_cast<std::uintptr_t>(self->field_0008)),
        element);
}

HOOK(0x0045A030, S_00459ec0_AppendElement40, "auto");
