#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(unsigned int, __fastcall, C_00862028_TraverseIntersectionTree, 0x006EE1C0, (void* self, void* edx, int a2, int a3));

static int __fastcall C_00862028_QuerySegmentIntersection(
    C_00862028* self, void* /*edx*/, int queryRoot,
    float* endpoints, int* filter, std::int16_t flags)
{
    if (endpoints[0] == endpoints[3] &&
        endpoints[1] == endpoints[4] &&
        endpoints[2] == endpoints[5])
    {
        return 0;
    }

    auto* result = reinterpret_cast<std::uint8_t*>(self) + 8;

    if (queryRoot)
    {
        alignas(8) std::uint8_t optionsBytes[0x14];
        CSF_AT(int, optionsBytes, 0x00) = 0;
        CSF_AT(int, optionsBytes, 0x04) = *filter;
        CSF_AT(std::uint8_t, optionsBytes, 0x08) = 0;
        CSF_AT(std::uint8_t, optionsBytes, 0x09) = (flags & 0x40) == 0;
        CSF_AT(std::uint8_t, optionsBytes, 0x0A) = (flags & 0x80) != 0;
        CSF_AT(std::uint8_t, optionsBytes, 0x0B) = (flags & 0x100) != 0;
        CSF_AT(void*, optionsBytes, 0x0C) = result;
        CSF_AT(C_00862028*, optionsBytes, 0x10) = self;

        std::uint32_t segment[7];
        std::memcpy(segment, endpoints, 0x18);
        segment[6] = 1;

        ((void(__fastcall*)(void*, void*))
            CSF_AT(void**, result, 0)[1])(result, nullptr);

        std::memcpy(result + 28, segment, 0x1C);
        CSF_AT(std::uint8_t, result, 56) = 0;

        C_00862028_TraverseIntersectionTree(
            self, nullptr, queryRoot,
            static_cast<int>(reinterpret_cast<std::uintptr_t>(optionsBytes)));
    }

    const std::uint8_t empty =
        ((std::uint8_t(__fastcall*)(void*, void*))
            CSF_AT(void**, result, 0)[2])(result, nullptr);

    return empty == 0
        ? static_cast<int>(reinterpret_cast<std::uintptr_t>(result))
        : 0;
}

HOOK(0x006EE300, C_00862028_QuerySegmentIntersection, "auto");
