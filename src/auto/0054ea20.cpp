#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Copies one dword into the slot at `destination`, using the owning
// object and a tag for bookkeeping (0x54E5A0, __cdecl, pops 0).
CSF_FUNC(int, __cdecl, CopyDwordIntoSlot_54E5A0, 0x0054E5A0,
         (int a1, int a2, int a3, int a4, int a5));

// Grows the vector-like container and inserts elements at `position`
// (0x54E7C0, __thiscall, pops 12 stack bytes).
CSF_FUNC(unsigned int, __fastcall, InsertDwordElements_54E7C0, 0x0054E7C0,
         (void* self, void* edx, char* Src, unsigned int a3, uint32_t* a4));

// Appends one pointer-sized element to the vector-like container
// (0x54EA20, __thiscall, pops 4 stack bytes).
static int __fastcall AppendPointerElement_54EA20(void* self, void* /*edx*/, std::uint32_t* element)
{
    const std::uint32_t begin = CSF_AT(std::uint32_t, self, 0x4);
    std::uint32_t size = 0;

    if (begin != 0) {
        const std::uint32_t difference = CSF_AT(std::uint32_t, self, 0x8) - begin;
        size = static_cast<std::uint32_t>(
            static_cast<std::int32_t>(difference) >> 2);
    }

    if (begin == 0 ||
        size >= static_cast<std::uint32_t>(
                    static_cast<std::int32_t>(CSF_AT(std::uint32_t, self, 0xc) - begin) >> 2)) {
        return static_cast<int>(InsertDwordElements_54E7C0(
            self, nullptr,
            reinterpret_cast<char*>(
                static_cast<std::uintptr_t>(CSF_AT(std::uint32_t, self, 0x8))),
            1,
            element));
    }

    const std::uint32_t end = CSF_AT(std::uint32_t, self, 0x8);

    // The original's fifth argument is a local byte set to zero whose
    // upper bytes are uninitialized; pass 0.
    const int result = CopyDwordIntoSlot_54E5A0(
        static_cast<int>(end),
        1,
        static_cast<int>(reinterpret_cast<std::uintptr_t>(element)),
        static_cast<int>(reinterpret_cast<std::uintptr_t>(self)),
        0);
    CSF_AT(std::uint32_t, self, 0x8) = end + 4u;
    return result;
}

HOOK(0x0054EA20, AppendPointerElement_54EA20, "auto");
