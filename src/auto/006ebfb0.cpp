#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

static int __fastcall C_00862028_QueryNondegenerateIntersection(
    C_00862028* self, void* /*edx*/, int queryAddress,
    int segmentAddress, int options)
{
    auto* segment = reinterpret_cast<float*>(segmentAddress);
    if (CSF_AT(float, segment, 0x00) == CSF_AT(float, segment, 0x0C) &&
        CSF_AT(float, segment, 0x04) == CSF_AT(float, segment, 0x10) &&
        CSF_AT(float, segment, 0x08) == CSF_AT(float, segment, 0x14))
    {
        return 0;
    }

    int intersectionIndex = -1;
    return reinterpret_cast<int (__fastcall*)(
        C_00862028*, void*, float*, float*, int*, int)>(
        CSF_AT(void**, self, 0)[0x34 / 4])(
            self, nullptr, reinterpret_cast<float*>(queryAddress),
            segment, &intersectionIndex, options);
}

HOOK(0x006EBFB0, C_00862028_QueryNondegenerateIntersection, "auto");
