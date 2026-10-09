#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// Copy-constructs one 168-byte element at `a1` (cdecl, 5 stack args, caller cleans 0x14).
CSF_FUNC(std::uint32_t*, __cdecl, ConstructElement168_775830, 0x00775830,
         (int a1, int a2, void* a3, int a4, int a5));

// Grows the 168-byte-element array and inserts one element (__thiscall, ret 12).
CSF_FUNC(std::uint32_t*, __fastcall, InsertElement168_776CE0, 0x00776CE0,
         (void* self, void* edx, std::uint32_t* a2, void* a3, int a4));

// Appends one 168-byte element to the array held at self+0x04/0x08/0x0C.
// Uses the spare capacity when available, otherwise delegates to InsertElement168_776CE0.
// The single stack argument is a pointer-sized value (passed through unchanged).
static std::uint32_t* __fastcall S_00776ce0_AppendElement168(
    S_00776ce0* self, void* /*edx*/, void* a3)
{
    const std::uint32_t begin = self->field_0004;
    std::uint32_t size = 0;

    if (begin != 0)
    {
        size = static_cast<std::uint32_t>(
            static_cast<std::int32_t>(self->field_0008 - begin) / 168);
    }

    if (begin != 0)
    {
        const std::uint32_t capacity = static_cast<std::uint32_t>(
            static_cast<std::int32_t>(self->field_000c - begin) / 168);

        if (size < capacity)
        {
            const std::uint32_t element = self->field_0008;
            std::uint32_t* const result = ConstructElement168_775830(
                static_cast<int>(element), 1,
                a3,
                static_cast<int>(reinterpret_cast<std::uintptr_t>(self)),
                static_cast<int>(reinterpret_cast<std::uintptr_t>(a3)));

            self->field_0008 = element + 168u;
            return result;
        }
    }

    (void)CSF_VTBL(0x00776CE0);
    return InsertElement168_776CE0(
        self, nullptr,
        reinterpret_cast<std::uint32_t*>(&a3),
        reinterpret_cast<void*>(self->field_0008),
        static_cast<int>(reinterpret_cast<std::uintptr_t>(a3)));
}

HOOK(0x00777FC0, S_00776ce0_AppendElement168, "auto");
