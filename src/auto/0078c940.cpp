#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Constructs one 164-byte element at `destination` (cdecl, 5 stack args, caller cleans 0x14).
CSF_FUNC(std::uint32_t*, __cdecl, S_0078c870_ConstructElement164, 0x0078C1A0,
         (std::uint32_t* a1, int a2, std::uint32_t* a3, int a4, int a5));

// Grows the 164-byte-element array and inserts one element (__thiscall, ret 12).
CSF_FUNC(std::uint32_t*, __fastcall, S_0078c870_InsertElement164, 0x0078C870,
         (void* self, void* edx, std::uint32_t* a2, char* a3, int* a4));

// Appends one 164-byte element to the array held at self+0x04/0x08/0x0C.
// Uses the spare capacity when available, otherwise delegates to S_0078c870_InsertElement164.
static std::uint32_t* __fastcall S_0078c870_AppendElement164(
    S_0078c870* self, void* /*edx*/, int* element)
{
    // Vtable reference from the original's call site; no store is performed here.
    (void)CSF_VTBL(0x0078C870);

    const std::uint32_t begin = self->field_0004;
    std::uint32_t index = 0;

    if (begin != 0)
    {
        index = static_cast<std::uint32_t>(
            static_cast<std::int32_t>(self->field_0008 - begin) / 164);
    }

    if (begin == 0 ||
        index >= static_cast<std::uint32_t>(
                     static_cast<std::int32_t>(self->field_000c - begin) / 164))
    {
        return S_0078c870_InsertElement164(
            self, nullptr,
            reinterpret_cast<std::uint32_t*>(&element),
            reinterpret_cast<char*>(static_cast<std::uintptr_t>(self->field_0008)),
            element);
    }

    const std::uint32_t end = self->field_0008;
    std::uint32_t* result = S_0078c870_ConstructElement164(
        reinterpret_cast<std::uint32_t*>(static_cast<std::uintptr_t>(end)), 1,
        reinterpret_cast<std::uint32_t*>(element),
        static_cast<int>(reinterpret_cast<std::intptr_t>(self)),
        static_cast<int>(reinterpret_cast<std::intptr_t>(element)));

    self->field_0008 = end + 164u;
    return result;
}

HOOK(0x0078C940, S_0078c870_AppendElement164, "auto");
