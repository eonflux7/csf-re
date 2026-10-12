#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Fills `count` dwords at `destination` from `source`, using `context` for
// bookkeeping; the original pushes a fifth argument, a zeroed byte flag
// (0x46BFB0, __cdecl, pops 0).
CSF_FUNC(int, __cdecl, IntVector_FillUninitialized, 0x0046BFB0,
         (std::uint32_t* destination, std::uint32_t count,
          const std::uint32_t* source, void* context, int fillFlag));

// Grows the vector-like container and inserts elements at `position`
// (0x46C940, __thiscall, pops 12 stack bytes).
CSF_FUNC(unsigned int, __fastcall, IntVector_InsertN, 0x0046C940,
         (void* self, void* edx, char* Src, unsigned int a3, uint32_t* a4));

// Appends one dword element to the int vector at `self` (0x4FD290,
// __thiscall, pops 4 stack bytes). Grows through IntVector_InsertN when the
// spare capacity is exhausted, otherwise fills the slot at the end pointer
// and advances it.
static unsigned int __fastcall IntArray_PushBack(S_0046c410* self, void* /*edx*/, std::uint32_t* a3)
{
    const std::uint32_t begin = self->field_0004;
    std::uint32_t size = 0;

    if (begin != 0) {
        const std::uint32_t difference = self->field_0008 - begin;
        size = static_cast<std::uint32_t>(
            static_cast<std::int32_t>(difference) >> 2);
    }

    if (begin == 0 ||
        size >= static_cast<std::uint32_t>(
                    static_cast<std::int32_t>(self->field_000c - begin) >> 2)) {
        return IntVector_InsertN(
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
    const unsigned int result = static_cast<unsigned int>(
        IntVector_FillUninitialized(
            reinterpret_cast<std::uint32_t*>(
                static_cast<std::uintptr_t>(end)),
            1,
            reinterpret_cast<const std::uint32_t*>(a3),
            static_cast<void*>(self),
            0));
    self->field_0008 = end + 4u;
    return result;
}

HOOK(0x004FD290, IntArray_PushBack, "auto");
