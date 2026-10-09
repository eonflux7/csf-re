#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Constructs one 48-byte element at `destination` (cdecl, 5 stack args, caller cleans 0x14).
CSF_FUNC(void*, __cdecl, ConstructElement48, 0x00454920,
         (int a1, int a2, int a3, int a4, int a5));

// Grows the 48-byte-element array and inserts one element (__thiscall, ret 12).
CSF_FUNC(std::uint32_t*, __fastcall, GrowAndInsertElement48, 0x00455ED0,
         (void* self, void* edx, std::uint32_t* a2, int a3, int a4));

// Appends one 48-byte element to the array held at self+0x04/0x08/0x0C.
// Uses the spare capacity when available, otherwise delegates to GrowAndInsertElement48.
static std::uint32_t* __fastcall S_00455ed0_AppendElement48(
    S_00455ed0* self, void* /*edx*/, int element)
{
    // Vtable reference from the original's call site; no store is performed here.
    (void)CSF_VTBL(0x00455ED0);

    const std::uint32_t begin = self->field_0004;
    std::uint32_t size = 0;

    if (begin != 0)
    {
        size = static_cast<std::uint32_t>(
            static_cast<std::int32_t>(self->field_0008 - begin) / 48);
    }

    if (begin == 0 ||
        size >= static_cast<std::uint32_t>(
                    static_cast<std::int32_t>(self->field_000c - begin) / 48))
    {
        return GrowAndInsertElement48(
            self, nullptr,
            reinterpret_cast<std::uint32_t*>(&element),
            static_cast<int>(self->field_0008),
            element);
    }

    const std::uint32_t end = self->field_0008;
    void* const result = ConstructElement48(
        static_cast<int>(end), 1, element,
        static_cast<int>(reinterpret_cast<std::intptr_t>(self)), element);

    self->field_0008 = end + 48u;
    return reinterpret_cast<std::uint32_t*>(result);
}

HOOK(0x00456360, S_00455ed0_AppendElement48, "auto");
