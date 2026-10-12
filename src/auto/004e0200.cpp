#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, GrowCandidateArray, 0x004DF5B0,
         (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, InsertCandidateScore, 0x004E01A0,
         (void* self, void* edx, uint32_t* a6, int a3));
CSF_FUNC(bool, __fastcall, IsLinkedObjectExcluded, 0x004E0910,
         (void* self, void* edx));
CSF_FUNC(int, __fastcall, GetLinkedObjectValue_4E0B60, 0x004E0B60,
         (void* self, void*));
CSF_FUNC(int, __fastcall, GetChildBounds, 0x004E0B70,
         (void* self, void* edx, uint32_t* a2, uint32_t* a3));
CSF_FUNC(double, __cdecl, RwV3dLength, 0x00623360, (float* a1));
CSF_FUNC(int, __fastcall, GetChildAtIndex, 0x00732150,
         (void* self, void* edx, int a2));

CSF_GLOBAL(float, g_HalfDurationScale, 0x0082D444);

static char __fastcall CollectNearbyLinkedObjects(
    float* self, void* /*edx*/, void* node, float* position,
    uint32_t* candidates, S_004e0130* scores)
{
    auto* children = CSF_AT(C_0086709c*, node, 0x04);
    const uint32_t childCount = CSF_AT(uint32_t, children, 0x08);

    if (childCount + candidates[2] >= candidates[1])
        return 0;

    if (childCount == 0)
        return 1;

    uint32_t index = 0;
    do
    {
        const int childValue =
            GetChildAtIndex(children, nullptr, static_cast<int>(index));
        void* child = reinterpret_cast<void*>(childValue);
        const int linkedValue = GetLinkedObjectValue_4E0B60(child, nullptr);
        void* linkedObject = reinterpret_cast<void*>(linkedValue);

        if (linkedValue != 0 &&
            !IsLinkedObjectExcluded(linkedObject, nullptr) &&
            (CSF_AT(uint8_t, linkedObject, 0x04) & 1) == 0)
        {
            alignas(8) uint8_t minimumBytes[0x0C];
            alignas(8) uint8_t maximumBytes[0x14];
            std::memset(minimumBytes, 0, 0x0C);
            std::memset(maximumBytes, 0, 0x0C);

            GetChildBounds(child, nullptr,
                           reinterpret_cast<uint32_t*>(minimumBytes),
                           reinterpret_cast<uint32_t*>(maximumBytes));

            float* minimum = reinterpret_cast<float*>(minimumBytes);
            float* maximum = reinterpret_cast<float*>(maximumBytes);
            const float centerX = (maximum[0] + minimum[0]) * g_HalfDurationScale;
            const float centerY = (maximum[1] + minimum[1]) * g_HalfDurationScale;
            const float centerZ = (maximum[2] + minimum[2]) * g_HalfDurationScale;

            uint32_t record[9];
            std::memcpy(&record[3], &centerX, sizeof(float));
            std::memcpy(&record[4], &centerY, sizeof(float));
            std::memcpy(&record[5], &centerZ, sizeof(float));

            alignas(8) uint8_t nodeDeltaBytes[0x18];
            float* nodeDelta = reinterpret_cast<float*>(nodeDeltaBytes);
            nodeDelta[0] = centerX - CSF_AT(float, node, 0x0C);
            nodeDelta[1] = centerY - CSF_AT(float, node, 0x10);
            nodeDelta[2] = centerZ - CSF_AT(float, node, 0x14);

            const double distance =
                RwV3dLength(nodeDelta) + CSF_AT(float, node, 0x1C);
            const float nodeDistance = static_cast<float>(distance);
            std::memcpy(&record[7], &nodeDistance, sizeof(float));

            if (distance < self[2])
            {
                alignas(8) uint8_t positionDeltaBytes[0x0C];
                float* positionDelta =
                    reinterpret_cast<float*>(positionDeltaBytes);
                positionDelta[0] = position[0] - centerX;
                positionDelta[1] = position[1] - centerY;
                positionDelta[2] = position[2] - centerZ;

                const float positionDistance =
                    static_cast<float>(RwV3dLength(positionDelta));
                std::memcpy(&record[8], &positionDistance, sizeof(float));

                uint32_t candidateIndex = candidates[2];
                const uint32_t depth = CSF_AT(uint32_t, node, 0x18);
                record[1] = static_cast<uint32_t>(linkedValue);
                const float score = positionDistance + nodeDistance;
                record[0] = reinterpret_cast<uint32_t>(node);
                record[6] = depth + 1;
                record[2] = static_cast<uint32_t>(childValue);

                int scoreBits;
                std::memcpy(&scoreBits, &score, sizeof(scoreBits));
                InsertCandidateScore(scores, nullptr, &candidateIndex, scoreBits);

                if (candidates[2] >= candidates[1])
                    GrowCandidateArray(candidates, nullptr,
                                       static_cast<int>(candidates[3]));

                const uint32_t count = candidates[2];
                void* destination = reinterpret_cast<void*>(
                    candidates[0] + 36 * count);
                candidates[2] = count + 1;
                std::memcpy(destination, record, 0x24);
            }
        }

        ++index;
    } while (index < CSF_AT(uint32_t, children, 0x08));

    return 1;
}

HOOK(0x004E0200, CollectNearbyLinkedObjects, "auto");
