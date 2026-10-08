#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

// int __cdecl sub_72A770(int a1, int a2, int a3) -- but the call site pushes
// five dwords and cleans 0x14 bytes (see the verified clone at 0x007199B0),
// so it is bound with the real five-argument __cdecl signature.
CSF_FUNC(int, __cdecl, CopyFourByteElementsWithContext_72A770, 0x0072A770,
         (std::uint32_t destination, int count, const std::uint32_t* source,
          void* owner, std::uint32_t tag));

// unsigned int __thiscall sub_72C170(int *this, std::uint32_t *Src, unsigned int a3,
//                                   std::uint32_t *a4)   -- pops 12 bytes (3 stack args).
CSF_FUNC(std::uint32_t, __fastcall, InsertElements, 0x0072C170,
         (void* self, void* edx, std::uint32_t* position,
          std::uint32_t count, std::uint32_t* source));

// Appends a single four-byte element to the dynamic array [begin,end,capacity)
// stored at self+4/+8/+0xC, growing the array when there is no spare room.
// A context pointer is forwarded to the copy/insert helpers.
static std::uint32_t __fastcall AppendFourByteElementWithContext(
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

    // No storage yet, or the array is already at capacity: take the slow
    // (insert-with-growth) path and return its result.
    if (begin == 0 ||
        size >= static_cast<std::uint32_t>(
                    static_cast<std::int32_t>(
                        CSF_AT(std::uint32_t, self, 0x0C) - begin) >> 2)) {
        return InsertElements(
            self, nullptr,
            reinterpret_cast<std::uint32_t*>(
                CSF_AT(std::uint32_t, self, 0x08)),
            1, source);
    }

    const std::uint32_t end = CSF_AT(std::uint32_t, self, 0x08);

    // The original pushes the incoming ECX (== this) as a four-byte local
    // slot, clears its low byte and passes the whole dword as the context tag.
    const std::uint32_t tag =
        static_cast<std::uint32_t>(reinterpret_cast<std::uintptr_t>(self)) &
        0xFFFFFF00u;

    const int result =
        CopyFourByteElementsWithContext_72A770(end, 1, source, self, tag);
    CSF_AT(std::uint32_t, self, 0x08) = end + 4u;
    return static_cast<std::uint32_t>(result);
}

HOOK(0x0072C780, AppendFourByteElementWithContext, "auto");
