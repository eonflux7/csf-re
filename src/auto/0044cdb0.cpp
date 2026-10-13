#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Constructs one 100-byte element at `destination` (cdecl, 5 stack args, caller cleans 0x14).
CSF_FUNC(std::uint32_t*, __cdecl, S_0044cd10_ConstructElement100, 0x0044C340,
         (std::uint32_t* a1, int a2, int a3, int a4, int a5));

// Grows the 100-byte-element array and inserts one element (__thiscall, ret 12).
CSF_FUNC(std::uint32_t*, __fastcall, S_0044cd10_InsertElement100, 0x0044CD10,
         (void* self, void* edx, std::uint32_t* a2, char* a3, int a4));

// Appends one 100-byte element to the array held at self+0x04/0x08/0x0C.
// Uses the spare capacity when available, otherwise delegates to S_0044cd10_InsertElement100.
static std::uint32_t* __fastcall S_0044cd10_AppendElement100(
    S_0044cd10* self, void* /*edx*/, int element)
{
    // Vtable reference from the original's call site; no store is performed here.
    (void)CSF_VTBL(0x0044CD10);

    const std::uint32_t begin = self->field_0004;
    std::uint32_t index = 0;

    if (begin != 0)
    {
        index = static_cast<std::uint32_t>(
            static_cast<std::int32_t>(self->field_0008 - begin) / 100);
    }

    if (begin == 0 ||
        index >= static_cast<std::uint32_t>(
                     static_cast<std::int32_t>(self->field_000c - begin) / 100))
    {
        return S_0044cd10_InsertElement100(
            self, nullptr,
            reinterpret_cast<std::uint32_t*>(&element),
            reinterpret_cast<char*>(static_cast<std::uintptr_t>(self->field_0008)),
            element);
    }

    const std::uint32_t end = self->field_0008;
    std::uint32_t* result = S_0044cd10_ConstructElement100(
        reinterpret_cast<std::uint32_t*>(static_cast<std::uintptr_t>(end)), 1,
        element,
        static_cast<int>(reinterpret_cast<std::intptr_t>(self)),
        element);

    self->field_0008 = end + 100u;
    return result;
}

HOOK(0x0044CDB0, S_0044cd10_AppendElement100, "auto");
