#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, Path_IntersectSegmentsXZ, 0x004DF240,
         (float a1, float a2, float a3, float a4, float a5, float a6,
          float a7, float a8, float* a9, float* a10));
CSF_FUNC(int, __fastcall, Path_GrowOutput, 0x004DF500,
         (void* self, void* edx, int a2));
CSF_FUNC(int*, __fastcall, Path_FinalizeOutput, 0x004DF800,
         (void* self, void* edx, int* a2));
CSF_FUNC(int, __fastcall, Path_GetSegmentEndpoints, 0x004E0B70,
         (void* self, void* edx, uint32_t* a2, uint32_t* a3));
CSF_FUNC(float*, __fastcall, Path_GetOrigin, 0x004E0DE0,
         (void* self, void* edx, float* a2));
CSF_FUNC(int, __fastcall, Path_GetSegment, 0x00732150,
         (void* self, void* edx, int a2));

CSF_GLOBAL(float, g_fZero, 0x0082D4CC);
CSF_GLOBAL(float, g_TransitionOne, 0x0082D4D4);

// The packet also classifies this direct-call address as a vtable reference.
static const auto g_PathOriginCallReference = CSF_VTBL(0x004E0DE0);

static int* __fastcall S_004dfbc0_BuildPath(
    S_004dfbc0* self, void* /*edx*/, uint32_t* initialPosition,
    float* targetPosition, float excludedOwner, int nodeAddress, int outputAddress)
{
    void* node = reinterpret_cast<void*>(static_cast<uintptr_t>(
        static_cast<uint32_t>(nodeAddress)));
    int* output = reinterpret_cast<int*>(static_cast<uintptr_t>(
        static_cast<uint32_t>(outputAddress)));

    const uint32_t lastNodeIndex = CSF_AT(uint32_t, node, 0x18);
    const uint32_t requiredCount = lastNodeIndex + 2;
    const uint32_t capacity = CSF_AT(uint32_t, output, 0x04);
    if (requiredCount > capacity)
        Path_GrowOutput(output, nullptr, static_cast<int>(requiredCount - capacity));

    auto* firstPosition = reinterpret_cast<uint8_t*>(
        static_cast<uintptr_t>(CSF_AT(uint32_t, output, 0x00))) + 4;
    CSF_AT(uint32_t, output, 0x08) = requiredCount;
    CSF_AT(uint32_t, firstPosition, 0x00) = initialPosition[0];
    CSF_AT(uint32_t, firstPosition, 0x04) = initialPosition[1];
    CSF_AT(uint32_t, firstPosition, 0x08) = initialPosition[2];

    alignas(8) uint8_t originBytes[0x18];
    alignas(8) uint8_t endBytes[0x18];
    alignas(8) uint8_t startBytes[0x0C];
    alignas(8) uint8_t intersectionBytes[0x10];

    CSF_AT(float, endBytes, 0x0C) = targetPosition[0];
    CSF_AT(float, endBytes, 0x10) = targetPosition[1];
    CSF_AT(float, endBytes, 0x14) = targetPosition[2];

    uint32_t excludedOwnerAddress;
    std::memcpy(&excludedOwnerAddress, &excludedOwner, sizeof(excludedOwnerAddress));
    const uint32_t ownerAddress = CSF_AT(uint32_t, node, 0x04);
    if (excludedOwnerAddress != ownerAddress)
    {
        Path_GetOrigin(
            reinterpret_cast<void*>(static_cast<uintptr_t>(ownerAddress)),
            nullptr, reinterpret_cast<float*>(originBytes));

        const void* owner = reinterpret_cast<void*>(static_cast<uintptr_t>(
            CSF_AT(uint32_t, node, 0x04)));
        const uint32_t segmentCount = CSF_AT(uint32_t, owner, 0x08);

        for (uint32_t index = 0; index < segmentCount; ++index)
        {
            const int segmentAddress = Path_GetSegment(
                reinterpret_cast<void*>(static_cast<uintptr_t>(
                    CSF_AT(uint32_t, node, 0x04))),
                nullptr, static_cast<int>(index));

            CSF_AT(float, startBytes, 0x00) = 0.0f;
            CSF_AT(float, startBytes, 0x04) = 0.0f;
            CSF_AT(float, startBytes, 0x08) = 0.0f;
            CSF_AT(float, endBytes, 0x00) = 0.0f;
            CSF_AT(float, endBytes, 0x04) = 0.0f;
            CSF_AT(float, endBytes, 0x08) = 0.0f;

            Path_GetSegmentEndpoints(
                reinterpret_cast<void*>(static_cast<uintptr_t>(
                    static_cast<uint32_t>(segmentAddress))),
                nullptr, reinterpret_cast<uint32_t*>(startBytes),
                reinterpret_cast<uint32_t*>(endBytes));

            const float deltaZ = CSF_AT(float, endBytes, 0x08)
                               - CSF_AT(float, startBytes, 0x08);
            const float deltaX = CSF_AT(float, endBytes, 0x00)
                               - CSF_AT(float, startBytes, 0x00);
            const float targetDeltaZ = targetPosition[2]
                                    - CSF_AT(float, originBytes, 0x08);
            const float targetDeltaX = targetPosition[0]
                                    - CSF_AT(float, originBytes, 0x00);
            float segmentParameter = 0.0f;
            Path_IntersectSegmentsXZ(
                CSF_AT(float, originBytes, 0x00),
                CSF_AT(float, originBytes, 0x08),
                targetDeltaX, targetDeltaZ,
                CSF_AT(float, startBytes, 0x00),
                CSF_AT(float, startBytes, 0x08),
                deltaX, deltaZ,
                reinterpret_cast<float*>(intersectionBytes), &segmentParameter);

            if (segmentParameter >= g_fZero &&
                segmentParameter <= g_TransitionOne)
            {
                CSF_AT(float, originBytes, 0x0C) =
                    deltaX * segmentParameter + CSF_AT(float, startBytes, 0x00);
                CSF_AT(float, endBytes, 0x0C) =
                    CSF_AT(float, originBytes, 0x0C);
                CSF_AT(float, originBytes, 0x10) =
                    (CSF_AT(float, endBytes, 0x04) -
                     CSF_AT(float, startBytes, 0x04)) * segmentParameter +
                    CSF_AT(float, startBytes, 0x04);
                CSF_AT(float, endBytes, 0x10) =
                    CSF_AT(float, originBytes, 0x10);
                CSF_AT(float, originBytes, 0x14) =
                    deltaZ * segmentParameter + CSF_AT(float, startBytes, 0x08);
                CSF_AT(float, endBytes, 0x14) =
                    CSF_AT(float, originBytes, 0x14);
                break;
            }
        }
    }

    const uint32_t finalPositionOffset =
        20 * CSF_AT(uint32_t, output, 0x08) - 16;
    auto* finalPosition = reinterpret_cast<uint8_t*>(
        static_cast<uintptr_t>(CSF_AT(uint32_t, output, 0x00))) +
        finalPositionOffset;
    CSF_AT(uint32_t, finalPosition, 0x00) = CSF_AT(uint32_t, endBytes, 0x0C);
    CSF_AT(uint32_t, finalPosition, 0x04) = CSF_AT(uint32_t, endBytes, 0x10);
    CSF_AT(uint32_t, finalPosition, 0x08) = CSF_AT(uint32_t, endBytes, 0x14);

    if (CSF_AT(uint32_t, node, 0x00) != 0)
    {
        uint32_t nodeOffset = 20 * lastNodeIndex;
        do
        {
            auto* position = reinterpret_cast<uint8_t*>(
                static_cast<uintptr_t>(CSF_AT(uint32_t, output, 0x00))) +
                nodeOffset + 4;
            CSF_AT(uint32_t, position, 0x00) = CSF_AT(uint32_t, node, 0x0C);
            CSF_AT(uint32_t, position, 0x04) = CSF_AT(uint32_t, node, 0x10);
            CSF_AT(uint32_t, position, 0x08) = CSF_AT(uint32_t, node, 0x14);

            auto* entry = reinterpret_cast<uint8_t*>(
                static_cast<uintptr_t>(CSF_AT(uint32_t, output, 0x00))) +
                nodeOffset;
            CSF_AT(uint32_t, entry, 0x00) = CSF_AT(uint32_t, node, 0x08);
            node = reinterpret_cast<void*>(static_cast<uintptr_t>(
                CSF_AT(uint32_t, node, 0x00)));
            nodeOffset -= 20;
        }
        while (CSF_AT(uint32_t, node, 0x00) != 0);
    }

    return Path_FinalizeOutput(self, nullptr, output);
}

HOOK(0x004DFBC0, S_004dfbc0_BuildPath, "auto");
