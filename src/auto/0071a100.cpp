#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(int, __cdecl, CopyFourByteElements, 0x007199B0,
         (std::uint32_t destination, int count, const std::uint32_t* source,
          void* owner, std::uint32_t tag));

CSF_FUNC(std::uint32_t, __fastcall, InsertFourByteElements, 0x00719DB0,
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
        return InsertFourByteElements(
            self, nullptr,
            reinterpret_cast<std::uint32_t*>(
                CSF_AT(std::uint32_t, self, 0x08)),
            1, source);
    }

    const std::uint32_t end = CSF_AT(std::uint32_t, self, 0x08);

    // The original overwrites the low byte of its saved ECX stack slot.
    const std::uint32_t tag =
        static_cast<std::uint32_t>(reinterpret_cast<std::uintptr_t>(self)) &
        0xFFFFFF00u;

    const int result = CopyFourByteElements(end, 1, source, self, tag);
    CSF_AT(std::uint32_t, self, 0x08) = end + 4u;
    return static_cast<std::uint32_t>(result);
}

HOOK(0x0071A100, AppendFourByteElement, "auto");
