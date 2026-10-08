#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

// Copies one four-byte element into the slot at `destination`, using the
// owning container and a tag for whatever bookkeeping is required (0x50C950).
CSF_FUNC(int, __cdecl, CopyFourByteElement, 0x0050C950,
         (std::uint32_t destination, int count, std::uint32_t* source,
          void* owner, std::uint32_t tag));

// Grows the vector-like container and inserts one element at `position`
// (0x50CAA0, __thiscall, pops 12 stack bytes).
CSF_FUNC(unsigned int, __fastcall, InsertFourByteElements, 0x0050CAA0,
         (void* self, void* edx, std::uint32_t* position,
          std::uint32_t count, std::uint32_t* source));

static unsigned int __fastcall AppendFourByteElement(
    void* self, void* /*edx*/, std::uint32_t* source)
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
        return InsertFourByteElements(
            self, nullptr,
            reinterpret_cast<std::uint32_t*>(
                CSF_AT(std::uint32_t, self, 0x08)),
            1, source);
    }

    const std::uint32_t end = CSF_AT(std::uint32_t, self, 0x08);

    // `push ecx` saved `this`; the low byte of that slot is then zeroed before
    // the dword is pushed as the fifth argument to the copier.
    const std::uint32_t tag =
        static_cast<std::uint32_t>(reinterpret_cast<std::uintptr_t>(self)) &
        0xFFFFFF00u;

    const int result = CopyFourByteElement(end, 1, source, self, tag);
    CSF_AT(std::uint32_t, self, 0x08) = end + 4u;
    return static_cast<unsigned int>(result);
}

HOOK(0x0050CDF0, AppendFourByteElement, "auto");
