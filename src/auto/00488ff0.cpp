#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

// Copies one value into the slot at `destination` using the owning container
// and a tag for bookkeeping (0x484DD0, __cdecl, pops 0; the caller cleans up).
CSF_FUNC(int, __cdecl, CopyValueToSlot_484DD0, 0x00484DD0,
         (int a1, int a2, int a3, int a4, int a5));

// Grows the vector-like container and inserts elements at `position`
// (0x488C10, __thiscall, pops 12 stack bytes).
CSF_FUNC(unsigned int, __fastcall, InsertElementsAtPosition_488C10, 0x00488C10,
         (void* self, void* edx, char* Src, unsigned int a3, uint32_t* a4));

// Appends one value (passed as the single stack argument) to the vector-like
// container at `self` (0x488FF0, __thiscall, pops 4 stack bytes).
static int __fastcall AppendArgDwordToVector(void* self, void* /*edx*/, int element)
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
        return static_cast<int>(InsertElementsAtPosition_488C10(
            self, nullptr,
            reinterpret_cast<char*>(CSF_AT(std::uint32_t, self, 0x08)),
            1,
            reinterpret_cast<std::uint32_t*>(
                static_cast<std::uintptr_t>(element))));
    }

    const std::uint32_t end = CSF_AT(std::uint32_t, self, 0x08);

    // The fifth argument is an uninitialized stack slot whose low byte is
    // zeroed by the original; the stale upper bytes are garbage, so pass 0.
    const int tag = 0;

    const int result = CopyValueToSlot_484DD0(
        static_cast<int>(end), 1, element, static_cast<int>(reinterpret_cast<std::uintptr_t>(self)), tag);
    CSF_AT(std::uint32_t, self, 0x08) = end + 4u;
    return result;
}

HOOK(0x00488FF0, AppendArgDwordToVector, "auto");

