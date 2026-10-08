#include <cstdint>
#include "hook.h"
#include "core/memory.h"
#include "globals.h"

CSF_FUNC(std::uint32_t, __fastcall, AppendDwordCopies_555B70, 0x00555B70,
         (void* self, void* edx, std::uint32_t* position,
          std::uint32_t count, std::int32_t* value));
CSF_FUNC(void*, __cdecl, _memcpy, 0x007AEE60,
         (void* destination, const void* source, std::uint32_t size));

static std::uint32_t __fastcall ResizeDwordVector(
    void* self, void* /*edx*/, std::uint32_t requestedCount,
    std::int32_t fillValue)
{
    const std::uint32_t begin = CSF_AT(std::uint32_t, self, 0x04);
    std::uint32_t result = 0;

    if (begin != 0) {
        result = static_cast<std::uint32_t>(
            static_cast<std::int32_t>(
                CSF_AT(std::uint32_t, self, 0x08) - begin) >> 2);
    }

    if (result < requestedCount) {
        std::uint32_t currentCount = 0;
        if (begin != 0) {
            currentCount = static_cast<std::uint32_t>(
                static_cast<std::int32_t>(
                    CSF_AT(std::uint32_t, self, 0x08) - begin) >> 2);
        }

        return AppendDwordCopies_555B70(
            self, nullptr,
            reinterpret_cast<std::uint32_t*>(
                CSF_AT(std::uint32_t, self, 0x08)),
            requestedCount - currentCount, &fillValue);
    }

    if (begin != 0) {
        result = CSF_AT(std::uint32_t, self, 0x08);
        const std::uint32_t currentCount = static_cast<std::uint32_t>(
            static_cast<std::int32_t>(result - begin) >> 2);

        if (requestedCount < currentCount) {
            const std::uint32_t newEnd = begin + requestedCount * 4u;
            if (newEnd != result) {
                // The original still calls memcpy with a zero byte count.
                result = static_cast<std::uint32_t>(
                    reinterpret_cast<std::uintptr_t>(
                        _memcpy(reinterpret_cast<void*>(newEnd),
                                  reinterpret_cast<const void*>(result), 0)));
                CSF_AT(std::uint32_t, self, 0x08) = result;
            }
        }
    }

    return result;
}

HOOK(0x00715FD0, ResizeDwordVector, "auto");
