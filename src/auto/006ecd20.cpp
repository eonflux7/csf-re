#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_GLOBAL(float, g_fZero, 0x0082D4CC);
CSF_FUNC(char, __fastcall, GfxCollision_QueryIntersection, 0x006EC0B0,
         (void* self, void* edx, uint32_t* a2, int a3, unsigned int a4));

static char __fastcall C_00862028_QuerySphereIntersection(
    C_00862028* self, void* /*edx*/, int sphereAddress, unsigned int flags)
{
    const auto* sphere = reinterpret_cast<const uint8_t*>(sphereAddress);
    if (CSF_AT(float, sphere, 0x0C) == g_fZero)
        return 0;

    alignas(8) uint8_t queryBytes[0x1C] = {};
    CSF_AT(uint32_t, queryBytes, 0x00) = CSF_AT(uint32_t, sphere, 0x00);
    CSF_AT(uint32_t, queryBytes, 0x04) = CSF_AT(uint32_t, sphere, 0x04);
    CSF_AT(uint32_t, queryBytes, 0x08) = CSF_AT(uint32_t, sphere, 0x08);
    CSF_AT(uint32_t, queryBytes, 0x0C) = CSF_AT(uint32_t, sphere, 0x0C);
    CSF_AT(uint32_t, queryBytes, 0x18) = 3;

    return GfxCollision_QueryIntersection(
        self, nullptr, nullptr, reinterpret_cast<int>(queryBytes), flags);
}

HOOK(0x006ECD20, C_00862028_QuerySphereIntersection, "auto");
