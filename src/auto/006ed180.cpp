#include <cstdint>
#include "globals.h"
#include <cstring>
#include "hook.h"
#include "types/auto_classes.h"

static CSF_FUNC(char, __fastcall, QueryIntersection, 0x006ECE90,
    (void* self, void* edx, uint32_t* a2, int a3, int* a4, unsigned int a5));

static char __fastcall C_00862028_QuerySegmentIntersection(
    C_00862028* self, void* /*edx*/, float* endpoints,
    int* result, unsigned int flags)
{
    if (endpoints[0] == endpoints[3] &&
        endpoints[1] == endpoints[4] &&
        endpoints[2] == endpoints[5])
        return 0;

    alignas(8) uint8_t segmentBytes[0x1C];
    std::memcpy(segmentBytes, endpoints, 6 * sizeof(float));
    reinterpret_cast<uint32_t*>(segmentBytes)[6] = 1;

    return QueryIntersection(
        self, nullptr, nullptr,
        reinterpret_cast<int>(segmentBytes), result, flags);
}

HOOK(0x006ED180, C_00862028_QuerySegmentIntersection, "auto");
