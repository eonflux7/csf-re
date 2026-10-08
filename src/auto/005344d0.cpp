#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

// Copies `count` four-byte elements from `source` into the array slot at
// `position`, returning the value produced by the underlying copy.
CSF_FUNC(int, __cdecl, CopyFourByteElementsAt, 0x00533D20,
         (std::uint32_t position, int count, const std::uint32_t* source,
          void* owner, std::uint32_t tag));

// Grows the backing storage and inserts the elements (reallocating path).
CSF_FUNC(std::uint32_t, __fastcall, GrowAndCopyFourByteElements, 0x00533EB0,
         (void* self, void* /*edx*/, std::uint32_t position,
          std::uint32_t count, const std::uint32_t* source));

static std::uint32_t __fastcall PushBackFourByteElement(
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
        return GrowAndCopyFourByteElements(
            self, nullptr,
            CSF_AT(std::uint32_t, self, 0x08),
            1, source);
    }

    const std::uint32_t end = CSF_AT(std::uint32_t, self, 0x08);

    // The original writes only the low byte of its saved-ECX stack slot, so the
    // upper bytes still hold the `this` pointer value passed to the callee.
    const std::uint32_t tag =
        static_cast<std::uint32_t>(reinterpret_cast<std::uintptr_t>(self)) &
        0xFFFFFF00u;

    const int result = CopyFourByteElementsAt(end, 1, source, self, tag);
    CSF_AT(std::uint32_t, self, 0x08) = end + 4u;
    return static_cast<std::uint32_t>(result);
}

HOOK(0x005344D0, PushBackFourByteElement, "auto");
