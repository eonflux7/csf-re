#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Constructs one 20-byte element at `destination` (cdecl, 5 stack args, caller cleans 0x14).
CSF_FUNC(int, __cdecl, S_00454c10_ConstructElement20, 0x00450A10,
         (int a1, int a2, int a3, int a4, int a5));

// Grows the 20-byte-element array and inserts one element (__thiscall, ret 12).
CSF_FUNC(std::uint32_t*, __fastcall, S_00454c10_InsertElement20, 0x00454C10,
         (void* self, void* edx, std::uint32_t* a2, char* a3, std::uint32_t* a4));

// Appends one 20-byte element to the array held at self+0x04/0x08/0x0C.
// Uses the spare capacity when available, otherwise delegates to S_00454c10_InsertElement20.
static std::uint32_t* __fastcall S_00454c10_AppendElement20(
    S_00454c10* self, void* /*edx*/, std::uint32_t* element)
{
    // Vtable reference from the original's call site; no store is performed here.
    (void)CSF_VTBL(0x00454C10);

    const std::uint32_t begin = self->field_0004;
    std::uint32_t size = 0;

    if (begin != 0)
    {
        size = static_cast<std::uint32_t>(
            static_cast<std::int32_t>(self->field_0008 - begin) / 20);
    }

    if (begin == 0 ||
        size >= static_cast<std::uint32_t>(
                    static_cast<std::int32_t>(self->field_000c - begin) / 20))
    {
        return S_00454c10_InsertElement20(
            self, nullptr,
            reinterpret_cast<std::uint32_t*>(&element),
            reinterpret_cast<char*>(static_cast<std::uintptr_t>(self->field_0008)),
            element);
    }

    const std::uint32_t end = self->field_0008;
    const int result = S_00454c10_ConstructElement20(
        static_cast<int>(end), 1, static_cast<int>(reinterpret_cast<std::intptr_t>(element)),
        static_cast<int>(reinterpret_cast<std::intptr_t>(self)),
        static_cast<int>(reinterpret_cast<std::intptr_t>(element)));

    self->field_0008 = end + 20u;
    return reinterpret_cast<std::uint32_t*>(static_cast<std::intptr_t>(result));
}

HOOK(0x004553E0, S_00454c10_AppendElement20, "auto");
