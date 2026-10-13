#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Copies one dword into the slot at `destination`, using the owning
// object and a tag for bookkeeping (0x6C17E0, __cdecl, pops 0).
CSF_FUNC(int, __cdecl, CopyDwordIntoSlot_6C17E0, 0x006C17E0,
         (int a1, int a2, int a3, int a4, int a5));

// Grows the vector-like container and inserts elements at `position`
// (0x6C1910, __thiscall, pops 12 stack bytes). Returns a byte.
CSF_FUNC(char, __fastcall, InsertPointerElements_6C1910, 0x006C1910,
         (void* self, void* edx, uint32_t* Src, unsigned int a3, uint32_t* a4));

// Appends one pointer-sized element to the vector-like container
// (0x6C1C00, __thiscall, pops 4 stack bytes).
static int __fastcall GfxObjectInstances_AppendMaterialReference(void* self, void* /*edx*/, std::uint32_t a3)
{
    const std::uint32_t begin = CSF_AT(std::uint32_t, self, 0x04);
    std::uint32_t size = 0;

    if (begin != 0) {
        const std::uint32_t difference = CSF_AT(std::uint32_t, self, 0x08) - begin;
        size = static_cast<std::uint32_t>(
            static_cast<std::int32_t>(difference) >> 2);
    }

    if (begin == 0 ||
        size >= static_cast<std::uint32_t>(
                    static_cast<std::int32_t>(CSF_AT(std::uint32_t, self, 0x0c) - begin) >> 2)) {
        return static_cast<int>(InsertPointerElements_6C1910(
            self, nullptr,
            reinterpret_cast<std::uint32_t*>(
                static_cast<std::uintptr_t>(CSF_AT(std::uint32_t, self, 0x08))),
            1,
            reinterpret_cast<std::uint32_t*>(
                static_cast<std::uintptr_t>(a3))));
    }

    const std::uint32_t end = CSF_AT(std::uint32_t, self, 0x08);

    // The original's fifth argument is a local byte set to zero whose
    // upper bytes are uninitialized; pass 0.
    const int result = CopyDwordIntoSlot_6C17E0(
        static_cast<int>(end),
        1,
        static_cast<int>(a3),
        static_cast<int>(reinterpret_cast<std::uintptr_t>(self)),
        0);
    CSF_AT(std::uint32_t, self, 0x08) = end + 4u;
    return result;
}

HOOK(0x006C1C00, GfxObjectInstances_AppendMaterialReference, "auto");
