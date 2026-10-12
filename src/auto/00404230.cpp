#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Copies one dword into the slot at `destination`, using the owning
// object and a tag for bookkeeping (0x403790, __cdecl, pops 0).
CSF_FUNC(int, __cdecl, CopyDwordIntoSlot_403790, 0x00403790,
         (int a1, int a2, int a3, int a4, int a5));

// Grows the vector-like container and inserts elements at `position`
// (0x403EE0, __thiscall, pops 12 stack bytes).
CSF_FUNC(unsigned int, __fastcall, InsertDwordElements_403EE0, 0x00403EE0,
         (void* self, void* edx, char* Src, unsigned int a3, uint32_t* a4));

// Appends one pointer-sized element to the vector-like container
// (0x404230, __thiscall, pops 4 stack bytes).
static unsigned int __fastcall AppendDwordPointerElement(S_00403a70* self, void* /*edx*/, std::uint32_t* a3)
{
    const uint32_t begin = self->field_0004;
    std::uint32_t size = 0;

    if (begin != 0) {
        const std::uint32_t difference = self->field_0008 - begin;
        size = static_cast<std::uint32_t>(
            static_cast<std::int32_t>(difference) >> 2);
    }

    if (begin == 0 ||
        size >= static_cast<std::uint32_t>(
                    static_cast<std::int32_t>(self->field_000c - begin) >> 2)) {
        return InsertDwordElements_403EE0(
            self, nullptr,
            reinterpret_cast<char*>(
                static_cast<std::uintptr_t>(self->field_0008)),
            1,
            reinterpret_cast<std::uint32_t*>(
                reinterpret_cast<std::uintptr_t>(a3)));
    }

    const std::uint32_t end = self->field_0008;

    // The original's fifth argument is a local byte set to zero whose
    // upper bytes are uninitialized; pass 0.
    const int result = CopyDwordIntoSlot_403790(
        static_cast<int>(end),
        1,
        static_cast<int>(reinterpret_cast<std::uintptr_t>(a3)),
        static_cast<int>(reinterpret_cast<std::uintptr_t>(self)),
        0);
    self->field_0008 = end + 4u;
    return static_cast<unsigned int>(result);
}

HOOK(0x00404230, AppendDwordPointerElement, "auto");
