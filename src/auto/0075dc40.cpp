#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Constructs one 44-byte element at `destination` (cdecl, 5 stack args, caller cleans 0x14).
CSF_FUNC(std::uint32_t*, __cdecl, S_0075d900_ConstructElement44, 0x0075D120,
         (std::uint32_t* a1, int a2, std::uint32_t* a3, int a4, int a5));

// Grows the 44-byte-element array and inserts one element (__thiscall, ret 12).
CSF_FUNC(std::uint32_t*, __fastcall, S_0075d900_InsertElement44, 0x0075D900,
         (void* self, void* edx, std::uint32_t* a2, char* a3, std::uint32_t* a4));

// Appends one 44-byte element to the array held at self+0x04/0x08/0x0C.
// Uses the spare capacity when available, otherwise delegates to S_0075d900_InsertElement44.
static std::uint32_t* __fastcall S_0075d900_AppendElement44(
    S_0075d900* self, void* /*edx*/, std::uint32_t* element)
{
    // Vtable reference from the original's call site; no store is performed here.
    (void)CSF_VTBL(0x0075D900);

    const std::uint32_t begin = self->field_0004;
    std::uint32_t size = 0;

    if (begin != 0)
    {
        size = static_cast<std::uint32_t>(
            static_cast<std::int32_t>(self->field_0008 - begin) / 44);
    }

    if (begin == 0 ||
        size >= static_cast<std::uint32_t>(
                    static_cast<std::int32_t>(self->field_000c - begin) / 44))
    {
        return S_0075d900_InsertElement44(
            self, nullptr,
            reinterpret_cast<std::uint32_t*>(&element),
            reinterpret_cast<char*>(static_cast<std::uintptr_t>(self->field_0008)),
            element);
    }

    std::uint32_t* end = reinterpret_cast<std::uint32_t*>(
        static_cast<std::uintptr_t>(self->field_0008));
    std::uint32_t* result = S_0075d900_ConstructElement44(
        end, 1, element,
        static_cast<int>(reinterpret_cast<std::intptr_t>(self)),
        static_cast<int>(reinterpret_cast<std::intptr_t>(element)));

    self->field_0008 = static_cast<std::uint32_t>(
        reinterpret_cast<std::uintptr_t>(end + 11));
    return result;
}

HOOK(0x0075DC40, S_0075d900_AppendElement44, "auto");
