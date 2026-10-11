#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

static bool __fastcall GfxEnvironment_VisitClumps(
    void* self, void* /*edx*/,
    int (__cdecl* visitor)(uint32_t clump, int context),
    int context)
{
    bool result = true;

    for (uint32_t i = 0; i < CSF_AT(uint32_t, self, 0x14); ++i)
    {
        if (!result)
            break;
        result = visitor(CSF_AT(uint32_t*, self, 0x0C)[i], context) != 0;
    }

    for (uint32_t i = 0; i < CSF_AT(uint32_t, self, 0x24); ++i)
    {
        if (!result)
            break;
        result = visitor(CSF_AT(uint32_t*, self, 0x1C)[i], context) != 0;
    }

    auto* groupBase = static_cast<uint8_t*>(self);
    for (uint32_t group = 0; group < 3; ++group, groupBase += 0x10)
    {
        if (!result)
            break;

        const uint32_t count = CSF_AT(uint32_t, groupBase, 0x34);
        for (uint32_t i = 0; i < count; ++i)
        {
            if (!result)
                break;
            result = visitor(CSF_AT(uint32_t*, groupBase, 0x2C)[i], context) != 0;
        }
    }

    for (uint32_t i = 0; i < CSF_AT(uint32_t, self, 0x74); ++i)
    {
        if (!result)
            break;
        result = visitor(CSF_AT(uint32_t*, self, 0x6C)[i], context) != 0;
    }

    return result;
}

HOOK(0x006BCA60, GfxEnvironment_VisitClumps, "auto");
