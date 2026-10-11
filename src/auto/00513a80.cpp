#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Constructs one 200-byte element at `destination` (cdecl, 5 stack args, caller cleans 0x14).
CSF_FUNC(int, __cdecl, S_005139c0_ConstructElement200, 0x00512E90,
         (int a1, int a2, int a3, int a4, int a5));

// Grows the 200-byte-element array and inserts one element (__thiscall, ret 12).
CSF_FUNC(std::uint32_t*, __fastcall, S_005139c0_InsertElement200, 0x005139C0,
         (void* self, void* edx, std::uint32_t* a2, int a3, int a4));

// Appends one 200-byte element to the array held at self+0x04/0x08/0x0C.
// Uses the spare capacity when available, otherwise delegates to S_005139c0_InsertElement200.
static std::uint32_t* __fastcall S_005139c0_AppendElement200(
    S_005139c0* self, void* /*edx*/, int a3)
{
    // Vtable reference from the original's call site; no store is performed here.
    (void)CSF_VTBL(0x005139C0);

    const std::uint32_t begin = self->field_0004;
    std::uint32_t size = 0;

    if (begin != 0)
    {
        size = static_cast<std::uint32_t>(
            static_cast<std::int32_t>(self->field_0008 - begin) / 200);
    }

    if (begin == 0 ||
        size >= static_cast<std::uint32_t>(
                    static_cast<std::int32_t>(self->field_000c - begin) / 200))
    {
        return S_005139c0_InsertElement200(
            self, nullptr,
            reinterpret_cast<std::uint32_t*>(&a3),
            static_cast<int>(self->field_0008),
            a3);
    }

    const std::uint32_t end = self->field_0008;
    const int result = S_005139c0_ConstructElement200(
        static_cast<int>(end), 1, a3,
        static_cast<int>(reinterpret_cast<std::intptr_t>(self)),
        a3);

    self->field_0008 = end + 200u;
    return reinterpret_cast<std::uint32_t*>(static_cast<std::intptr_t>(result));
}

HOOK(0x00513A80, S_005139c0_AppendElement200, "auto");
