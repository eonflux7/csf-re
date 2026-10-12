#include <cstdint>
#include "globals.h"
#include <cstring>
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

static CSF_FUNC(char, __fastcall, GfxCollision_QueryIntersection, 0x006EC0B0,
               (void* self, void* edx, uint32_t* a2, int a3, unsigned int a4));

static char __fastcall C_00862028_QuerySegmentIntersection(
    C_00862028* self, void* /*edx*/, uint32_t* result,
    float* endpoints, unsigned int flags)
{
    if (endpoints[0] == endpoints[3] &&
        endpoints[1] == endpoints[4] &&
        endpoints[2] == endpoints[5])
        return 0;

    alignas(8) uint8_t queryBytes[0x1C];
    std::memcpy(queryBytes, endpoints, 6 * sizeof(float));
    CSF_AT(uint32_t, queryBytes, 0x18) = 1;

    return GfxCollision_QueryIntersection(
        self, nullptr, result,
        static_cast<int>(reinterpret_cast<uintptr_t>(queryBytes)), flags);
}

HOOK(0x006ECCA0, C_00862028_QuerySegmentIntersection, "auto");
