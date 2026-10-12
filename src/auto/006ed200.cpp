#include <cstdint>
#include "globals.h"
#include <cstring>
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(char, __fastcall, QueryIntersection, 0x006ECE90,
         (void* self, void* edx, uint32_t* a2, int a3, int* a4, unsigned int a5));

static char __fastcall C_00862028_QuerySegmentIntersection(
    C_00862028* self, void* /*edx*/, uint32_t* result,
    float* endpoints, int* details, unsigned int flags)
{
    if (endpoints[0] == endpoints[3] &&
        endpoints[1] == endpoints[4] &&
        endpoints[2] == endpoints[5])
        return 0;

    alignas(8) uint8_t segmentBytes[0x1C];
    std::memcpy(segmentBytes, endpoints, 6 * sizeof(float));
    const uint32_t segmentType = 1;
    std::memcpy(segmentBytes + 0x18, &segmentType, sizeof(segmentType));

    return QueryIntersection(
        self, nullptr, result, reinterpret_cast<int>(segmentBytes), details, flags);
}

HOOK(0x006ED200, C_00862028_QuerySegmentIntersection, "auto");
