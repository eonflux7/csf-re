#include <cstdint>
#include "globals.h"
#include <cstring>
#include "hook.h"
#include "types/auto_classes.h"

static CSF_FUNC(int, __fastcall, QueryIntersection, 0x006EDF60,
    (void* self, void* edx, float* a2, uint32_t* arg4, int* a4, unsigned int a5));

static int __fastcall C_00862028_QuerySegmentIntersection(
    C_00862028* self, void* /*edx*/, float* result,
    float* endpoints, int* hitInfo, unsigned int flags)
{
    if (endpoints[0] == endpoints[3] &&
        endpoints[1] == endpoints[4] &&
        endpoints[2] == endpoints[5])
        return 0;

    alignas(8) uint8_t queryBytes[0x1C];
    std::memcpy(queryBytes, endpoints, 6 * sizeof(float));
    uint32_t queryType = 1;
    std::memcpy(queryBytes + 0x18, &queryType, sizeof(queryType));

    return QueryIntersection(self, nullptr, result,
        reinterpret_cast<uint32_t*>(queryBytes), hitInfo, flags);
}

HOOK(0x006EE210, C_00862028_QuerySegmentIntersection, "auto");
