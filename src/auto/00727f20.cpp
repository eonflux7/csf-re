#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

// Copy `count` four-byte elements from `source` into `destination`,
// passing the owning container and a context tag to the element hook.
CSF_FUNC(int, __cdecl, CopyFourByteElementsWithContext_726E80, 0x00726E80,
         (std::uint32_t destination, int count, const std::uint32_t* source,
          void* owner, std::uint32_t tag));

// Insert `count` elements from `source` at `position` of the container,
// reallocating when the container is full or empty.
CSF_FUNC(std::uint32_t, __fastcall, InsertFourByteElementsAtPosition, 0x007275F0,
         (void* self, void* edx, std::uint32_t* position,
          std::uint32_t count, std::uint32_t* source));

static std::uint32_t __fastcall AppendFourByteElement(
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
        return InsertFourByteElementsAtPosition(
            self, nullptr,
            reinterpret_cast<std::uint32_t*>(
                CSF_AT(std::uint32_t, self, 0x08)),
            1, source);
    }

    const std::uint32_t end = CSF_AT(std::uint32_t, self, 0x08);

    // The original stores the low byte of a saved stack slot to 0 before
    // reloading it; that slot holds the incoming ECX (`this`), so the tag
    // is the pointer with its low byte cleared.
    const std::uint32_t tag =
        static_cast<std::uint32_t>(reinterpret_cast<std::uintptr_t>(self)) &
        0xFFFFFF00u;

    const int result =
        CopyFourByteElementsWithContext_726E80(end, 1, source, self, tag);
    CSF_AT(std::uint32_t, self, 0x08) = end + 4u;
    return static_cast<std::uint32_t>(result);
}

HOOK(0x00727F20, AppendFourByteElement, "auto");
