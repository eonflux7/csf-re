#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Copies a dword into the slot at `position` using the owning object and a
// tag for bookkeeping (0x552EF0, __cdecl, pops 0). Name chosen locally.
CSF_FUNC(int, __cdecl, CopyDwordWithOwnerTag_552EF0, 0x00552EF0,
         (int a1, int a2, int a3, int a4, int a5));

// Grows the vector-like container and inserts `count` dwords at `position`
// (0x555B70, __thiscall, pops 12 stack bytes).
CSF_FUNC(std::uint32_t, __fastcall, AppendDwordCopies_555B70, 0x00555B70,
         (void* self, void* edx, std::uint32_t* position, std::uint32_t count,
          std::int32_t* value));

// Appends one dword to the vector-like container, reusing spare capacity when
// available and otherwise growing it (0x5570F0, __thiscall, pops 4 stack bytes).
static std::uint32_t __fastcall AppendSelectionIndex(S_00553fa0* self, void* /*edx*/,
                                                     std::int32_t* a3)
{
    const std::uint32_t begin = self->field_0004;
    std::uint32_t size = 0;

    if (begin != 0) {
        const std::uint32_t difference = self->field_0008 - begin;
        size = static_cast<std::uint32_t>(static_cast<std::int32_t>(difference) >> 2);
    }

    if (begin == 0 ||
        size >= static_cast<std::uint32_t>(
                    static_cast<std::int32_t>(self->field_000c - begin) >> 2)) {
        return AppendDwordCopies_555B70(
            self, nullptr,
            reinterpret_cast<std::uint32_t*>(
                static_cast<std::uintptr_t>(self->field_0008)),
            1u,
            a3);
    }

    const std::uint32_t end = self->field_0008;

    // `push ecx` saved `this` in the local slot; its low byte is then zeroed
    // before the slot is pushed as the fifth argument to the copier.
    const std::uint32_t tag =
        static_cast<std::uint32_t>(reinterpret_cast<std::uintptr_t>(self)) &
        0xFFFFFF00u;

    const int result = CopyDwordWithOwnerTag_552EF0(
        static_cast<int>(end),
        1,
        static_cast<int>(reinterpret_cast<std::uintptr_t>(a3)),
        static_cast<int>(reinterpret_cast<std::uintptr_t>(self)),
        static_cast<int>(tag));

    self->field_0008 = end + 4u;
    return static_cast<std::uint32_t>(result);
}

HOOK(0x005570F0, AppendSelectionIndex, "auto");
