#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_GLOBAL(float, g_fZero, 0x0082D4CC);
CSF_FUNC(int, __fastcall, QueryIntersection, 0x006EDF60,
         (void* self, void* edx, float* a2, uint32_t* arg4, int* a4, unsigned int a5));

static int __fastcall C_00862028_QuerySphereIntersection(
    C_00862028* self, void* /*edx*/, int sphereAddress,
    int* results, unsigned int flags)
{
    const void* sphere = reinterpret_cast<const void*>(sphereAddress);
    if (CSF_AT(float, sphere, 0x0C) == g_fZero)
        return 0;

    alignas(8) uint8_t queryBytes[0x1C];
    CSF_AT(uint32_t, queryBytes, 0x00) = CSF_AT(uint32_t, sphere, 0x00);
    CSF_AT(uint32_t, queryBytes, 0x04) = CSF_AT(uint32_t, sphere, 0x04);
    CSF_AT(uint32_t, queryBytes, 0x08) = CSF_AT(uint32_t, sphere, 0x08);
    CSF_AT(uint32_t, queryBytes, 0x0C) = CSF_AT(uint32_t, sphere, 0x0C);
    CSF_AT(uint32_t, queryBytes, 0x10) = 0;
    CSF_AT(uint32_t, queryBytes, 0x14) = 0;
    CSF_AT(uint32_t, queryBytes, 0x18) = 3;

    return QueryIntersection(self, nullptr, nullptr,
                             reinterpret_cast<uint32_t*>(queryBytes),
                             results, flags);
}

HOOK(0x006EE2A0, C_00862028_QuerySphereIntersection, "auto");
