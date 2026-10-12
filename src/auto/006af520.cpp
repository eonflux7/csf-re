#include <cstdint>
#include <cmath>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, VecGrow_4DE9B0, 0x004DE9B0, (void* self, void* edx, int a2));
CSF_GLOBAL(float, g_TransitionOne, 0x0082D4D4);

static int __fastcall S_006aef40_AppendWeightedTangents(
    S_006aef40* self, void* /*edx*/)
{
    const uint32_t pointCount = self->field_000c;
    auto* tangents = reinterpret_cast<uint8_t*>(self) + 0x24;
    uint32_t pointOffset = 0;

    for (uint32_t index = 0; index < pointCount; ++index, pointOffset += 12)
    {
        float incomingX = 0.0f;
        float incomingY = 0.0f;
        float incomingZ = 0.0f;
        float outgoingX = 0.0f;
        float outgoingY = 0.0f;
        float outgoingZ = 0.0f;

        if (pointOffset != 0)
        {
            auto* point = reinterpret_cast<uint8_t*>(self->field_0004) + pointOffset;
            incomingX = CSF_AT(float, point, 0) - CSF_AT(float, point, -12);
            incomingY = CSF_AT(float, point, 4) - CSF_AT(float, point, -8);
            incomingZ = CSF_AT(float, point, 8) - CSF_AT(float, point, -4);
        }

        if (index != pointCount - 1)
        {
            auto* point = reinterpret_cast<uint8_t*>(self->field_0004) + pointOffset;
            outgoingX = CSF_AT(float, point, 12) - CSF_AT(float, point, 0);
            outgoingY = CSF_AT(float, point, 16) - CSF_AT(float, point, 4);
            outgoingZ = CSF_AT(float, point, 20) - CSF_AT(float, point, 8);
        }

        const bool needsGrowth =
            CSF_AT(uint32_t, tangents, 8) >= CSF_AT(uint32_t, tangents, 4);

        const float incomingLength = std::sqrt(
            incomingZ * incomingZ + incomingY * incomingY + incomingX * incomingX);
        const float outgoingLength = std::sqrt(
            outgoingZ * outgoingZ + outgoingY * outgoingY + outgoingX * outgoingX);
        const float inverseLengthSum =
            g_TransitionOne / (outgoingLength + incomingLength);
        const float incomingWeight =
            g_TransitionOne - inverseLengthSum * incomingLength;
        const float outgoingWeight =
            g_TransitionOne - inverseLengthSum * outgoingLength;

        const float tangentX = incomingX * incomingWeight + outgoingX * outgoingWeight;
        const float tangentY = incomingY * incomingWeight + outgoingY * outgoingWeight;
        const float tangentZ = incomingZ * incomingWeight + outgoingZ * outgoingWeight;

        if (needsGrowth)
            VecGrow_4DE9B0(tangents, nullptr, CSF_AT(int, tangents, 12));

        const uint32_t tangentCount = CSF_AT(uint32_t, tangents, 8);
        auto* data = reinterpret_cast<uint8_t*>(CSF_AT(uint32_t, tangents, 0));
        CSF_AT(uint32_t, tangents, 8) = tangentCount + 1;

        auto* destination = data + 12 * tangentCount;
        CSF_AT(float, destination, 0) = tangentX;
        CSF_AT(float, destination, 4) = tangentY;
        CSF_AT(float, destination, 8) = tangentZ;
    }

    return static_cast<int>(pointCount);
}

HOOK(0x006AF520, S_006aef40_AppendWeightedTangents, "auto");
