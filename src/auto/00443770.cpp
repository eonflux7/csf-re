#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_GLOBAL(float, g_fReturnedConstant_0082E030, 0x0082E030);

static int __fastcall AiCandidates_CountWithinRadius(
    void* self, void* /*edx*/, float tolerance, const void* query)
{
    const uint32_t candidateCount = CSF_AT(uint32_t, self, 0x24);
    if (candidateCount == 0)
        return 0;

    const float threshold = CSF_AT(float, self, 0x08) - tolerance;
    const float radius = CSF_AT(float, query, 0x0C);
    const float radiusSquared = radius * radius;
    const uint8_t* candidate = CSF_AT(const uint8_t*, self, 0x1C);

    int matches = 0;
    for (uint32_t index = 0; index < candidateCount; ++index, candidate += 16)
    {
        if (!(CSF_AT(float, candidate, 0x00) > threshold))
            continue;
        if (!(CSF_AT(float, query, 0x04) - g_fReturnedConstant_0082E030
              < CSF_AT(float, candidate, 0x08)))
            continue;
        if (!(CSF_AT(float, query, 0x10) + CSF_AT(float, query, 0x04)
              > CSF_AT(float, candidate, 0x08)))
            continue;

        const float dx = CSF_AT(float, candidate, 0x04) - CSF_AT(float, query, 0x00);
        const float dy = CSF_AT(float, candidate, 0x08) - CSF_AT(float, query, 0x04);
        const float dz = CSF_AT(float, candidate, 0x0C) - CSF_AT(float, query, 0x08);

        if (dx * dx + dz * dz + dy * dy <= radiusSquared)
            ++matches;
    }
    return matches;
}

HOOK(0x00443770, AiCandidates_CountWithinRadius, "auto");
