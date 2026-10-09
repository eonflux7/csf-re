#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

// Copies one dword into the slot at `destination`, using the owning
// object and a tag for bookkeeping (0x480DF0, __cdecl, pops 0).
CSF_FUNC(int, __cdecl, CopyDwordIntoSlot_480DF0, 0x00480DF0,
         (int a1, int a2, int a3, int a4, int a5));

// Grows the vector-like container and inserts elements at `position`
// (0x481A30, __thiscall, pops 12 stack bytes).
CSF_FUNC(unsigned int, __fastcall, InsertDwordElements_481A30, 0x00481A30,
         (void* self, void* edx, char* Src, unsigned int a3, uint32_t* a4));

// Appends one dword element to the vector-like container (0x481C90,
// __thiscall, pops 4 stack bytes).
static int __fastcall AppendDwordElement(void* self, void* /*edx*/, int sourceValue)
{
    const std::uint32_t begin = CSF_AT(std::uint32_t, self, 0x04);
    std::uint32_t size = 0;

    if (begin != 0) {
        const std::uint32_t difference =
            CSF_AT(std::uint32_t, self, 0x08) - begin;
        size = static_cast<std::uint32_t>(
            static_cast<std::int32_t>(difference) >> 2);
    }

    if (begin == 0 ||
        size >= static_cast<std::uint32_t>(
                    static_cast<std::int32_t>(
                        CSF_AT(std::uint32_t, self, 0x0C) - begin) >> 2)) {
        return static_cast<int>(InsertDwordElements_481A30(
            self, nullptr,
            reinterpret_cast<char*>(
                static_cast<std::uintptr_t>(CSF_AT(std::uint32_t, self, 0x08))),
            1,
            reinterpret_cast<std::uint32_t*>(
                static_cast<std::uintptr_t>(sourceValue))));
    }

    const std::uint32_t end = CSF_AT(std::uint32_t, self, 0x08);

    // `push ecx` saved `this` in the local slot; its low byte is then zeroed
    // before the slot is pushed as the fifth argument to the copier.
    const std::uint32_t tag =
        static_cast<std::uint32_t>(reinterpret_cast<std::uintptr_t>(self)) &
        0xFFFFFF00u;

    const int result = CopyDwordIntoSlot_480DF0(
        static_cast<int>(end),
        1,
        sourceValue,
        static_cast<int>(reinterpret_cast<std::uintptr_t>(self)),
        static_cast<int>(tag));
    CSF_AT(std::uint32_t, self, 0x08) = end + 4u;
    return result;
}

HOOK(0x00481C90, AppendDwordElement, "auto");
