#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, GrowPointBuffer, 0x004DF500, (void* self, void* edx, int a2));
CSF_FUNC(int*, __fastcall, S_004dfbc0_BuildPathPoints, 0x004DFBC0, (void* self, void* edx, uint32_t* a2, float* a3, float a4, int a5, int a6));
CSF_FUNC(int, __fastcall, S_004dfbc0_FindRoute, 0x004E0440, (void* self, void* edx, int a2, int a3, float* a4, float* a5, char a6, uint32_t* a7));

// The packet also labels this callee address as a vtable reference.
[[maybe_unused]] static const auto kReportedPathVtable = CSF_VTBL(0x004DFBC0);

static char __fastcall S_004dfbc0_CreatePath(
    S_004dfbc0* self, void* /*edx*/,
    float* startPosition, int endPositionAddress,
    int startRegion, int endRegion, uint32_t* output,
    char flags, int* routeResult, int /*unused*/)
{
    if (startRegion == endRegion)
    {
        const uint32_t capacity = CSF_AT(uint32_t, output, 0x04);
        if (capacity < 2)
            GrowPointBuffer(output, nullptr, static_cast<int>(2 - capacity));

        auto* firstPoint = reinterpret_cast<uint8_t*>(
            CSF_AT(uint32_t, output, 0x00)) + 4;
        CSF_AT(uint32_t, output, 0x08) = 2;

        CSF_AT(uint32_t, firstPoint, 0x00) = CSF_AT(uint32_t, startPosition, 0x00);
        CSF_AT(uint32_t, firstPoint, 0x04) = CSF_AT(uint32_t, startPosition, 0x04);
        CSF_AT(uint32_t, firstPoint, 0x08) = CSF_AT(uint32_t, startPosition, 0x08);

        auto* secondPoint = reinterpret_cast<uint8_t*>(
            CSF_AT(uint32_t, output, 0x00)) + 24;
        auto* endPosition = reinterpret_cast<float*>(endPositionAddress);
        CSF_AT(uint32_t, secondPoint, 0x00) = CSF_AT(uint32_t, endPosition, 0x00);
        CSF_AT(uint32_t, secondPoint, 0x04) = CSF_AT(uint32_t, endPosition, 0x04);
        CSF_AT(uint32_t, secondPoint, 0x08) = CSF_AT(uint32_t, endPosition, 0x08);
        return 1;
    }

    CSF_AT(uint8_t, self, 0x2C) = 1;
    CSF_AT(uint32_t, self, 0x14) = 0;

    const int route = S_004dfbc0_FindRoute(
        self, nullptr, startRegion, endRegion, startPosition,
        reinterpret_cast<float*>(endPositionAddress), flags,
        reinterpret_cast<uint32_t*>(reinterpret_cast<uint8_t*>(self) + 0x0C));

    if (route)
    {
        if ((flags & 4) != 0)
            *routeResult = route;

        if ((flags & 2) == 0)
        {
            float regionBits;
            std::memcpy(&regionBits, &endRegion, sizeof(regionBits));
            S_004dfbc0_BuildPathPoints(
                self, nullptr, reinterpret_cast<uint32_t*>(startPosition),
                reinterpret_cast<float*>(endPositionAddress), regionBits,
                route, reinterpret_cast<int>(output));
        }

        CSF_AT(uint8_t, self, 0x2C) = 0;
        return 1;
    }

    CSF_AT(uint8_t, self, 0x2C) = 0;
    return 0;
}

HOOK(0x004E05E0, S_004dfbc0_CreatePath, "auto");
