#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Constructs one 68-byte element at `destination` (cdecl, 5 stack args, caller cleans 0x14).
CSF_FUNC(char*, __cdecl, sub_46A320, 0x0046A320, (char* a1, int a2, int a3, int a4, int a5));

// Grows the 68-byte-element array and inserts one element (__thiscall, ret 12).
CSF_FUNC(std::uint32_t*, __fastcall, sub_46B100, 0x0046B100, (void* self, void* edx, std::uint32_t* a2, int a3, int a4));

// Appends one 68-byte element to the array held at self+0x04/0x08/0x0C.
// Uses the spare capacity when available, otherwise delegates to sub_46B100 (grow and insert).
static char* __fastcall S_0046b100_AppendElement68(
    S_0046b100* self, void* /*edx*/, int a3)
{
    // Vtable reference from the original's call site; no store is performed here.
    (void)CSF_VTBL(0x0046B100);

    const std::uint32_t begin = self->field_0004;
    std::uint32_t size = 0;

    if (begin != 0)
    {
        size = static_cast<std::uint32_t>(
            static_cast<std::int32_t>(self->field_0008 - begin) / 68);
    }

    if (begin == 0 ||
        size >= static_cast<std::uint32_t>(
                    static_cast<std::int32_t>(self->field_000c - begin) / 68))
    {
        const std::uint32_t* insertPos = reinterpret_cast<std::uint32_t*>(
            static_cast<std::uintptr_t>(self->field_0008));
        (void)insertPos;
        std::uint32_t* grown = sub_46B100(
            self, nullptr,
            reinterpret_cast<std::uint32_t*>(&a3),
            static_cast<int>(self->field_0008),
            a3);
        return reinterpret_cast<char*>(grown);
    }

    const std::uint32_t end = self->field_0008;
    char* result = sub_46A320(
        reinterpret_cast<char*>(static_cast<std::uintptr_t>(end)),
        1,
        a3,
        static_cast<int>(reinterpret_cast<std::intptr_t>(self)),
        a3);

    self->field_0008 = end + 68u;
    return result;
}

HOOK(0x0046B2B0, S_0046b100_AppendElement68, "auto");
