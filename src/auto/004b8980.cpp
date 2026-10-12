#include <cstdint>
#include <cstddef>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int16_t, __stdcall, GetFlaggedSceneObjectShortValue, 0x004481F0, (int handle));
CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_FUNC(float*, __fastcall, GetObjectPositionVector, 0x0048E430, (void* self, void* edx));
CSF_FUNC(int, __fastcall, PairLink_PrepareForRebind, 0x004E55A0, (C_0083e9d0* self, void*));
CSF_FUNC(char, __fastcall, DecrementByteCounter, 0x004E55B0, (void* self, void*));
CSF_FUNC(bool, __fastcall, C_0083e9d0_ResolveAndDispatchHandleEntry, 0x004E55C0, (C_0083e9d0* self, void* , int a2, int a3));
CSF_FUNC(int, __fastcall, PairLink_TestEndpoints, 0x004E5B60, (C_0083e9d0* self, void* , int a2));

CSF_GLOBAL(float, g_Const200_0f, 0x0082DDEC);
CSF_GLOBAL(float, g_ProximityMidDistanceSquared, 0x008339A8);
CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

static int __fastcall AiCombat_SelectBestOrder(
    void* self, void* /*edx*/, uint32_t* output)
{
    const uint32_t count = CSF_AT(uint32_t, self, 0x0C);
    if (!count)
        return 0;

    int selectedIndex = -1;
    bool selectedDifferentGroup = false;
    float bestScore = FLT_MAX;
    int fallbackIndex = -1;
    float fallbackScore = FLT_MAX;

    if (count == 1)
    {
        selectedIndex = 0;
    }
    else
    {
        float* ownerPosition = GetObjectPositionVector(
            CSF_AT(C_008342e8*, self, 0x00), nullptr);

        PairLink_PrepareForRebind(
            reinterpret_cast<C_0083e9d0*>(
                CSF_AT(C_008342e8*, self, 0x00)->field_003c),
            nullptr);

        uint32_t entryOffset = 0;
        for (uint32_t index = 0; index < count; ++index, entryOffset += 52)
        {
            auto* entry = CSF_AT(uint8_t*, self, 0x04) + entryOffset;
            if (CSF_AT(int32_t, entry, 0x2C) < 0)
                continue;

            auto* candidate = reinterpret_cast<C_008342e8*>(
                CHandleTable_Resolve(
                    g_pCurrentScene, nullptr,
                    reinterpret_cast<int>(entry + 0x24)));
            if (!candidate)
                continue;

            const bool differentGroup =
                CSF_AT(C_008342e8*, self, 0x00)->field_0078 !=
                candidate->field_0078;

            if (selectedDifferentGroup && !differentGroup)
                continue;

            float* candidatePosition = GetObjectPositionVector(candidate, nullptr);
            const float dx = candidatePosition[0] - ownerPosition[0];
            const float dy = candidatePosition[1] - ownerPosition[1];
            const float dz = candidatePosition[2] - ownerPosition[2];
            float score = dz * dz + dy * dy + dx * dx;

            if (differentGroup && score < bestScore)
            {
                const int16_t flaggedValue = GetFlaggedSceneObjectShortValue(
                    reinterpret_cast<int>(&candidate->field_001c));
                const float penalty = flaggedValue * g_Const200_0f;
                score = penalty * penalty + score;

                if (score < bestScore)
                {
                    void* evaluator = reinterpret_cast<void*>(
                        CSF_AT(C_008342e8*, self, 0x00)->field_0034);
                    const bool eligible =
                        reinterpret_cast<bool(__fastcall*)(void*, void*, uint32_t*)>(
                            CSF_AT(void**, evaluator, 0x00)[0xB4 / 4])(
                                evaluator, nullptr, &candidate->field_001c);
                    if (!eligible)
                        score += g_ProximityMidDistanceSquared;
                }
            }

            if (!(score < bestScore ||
                  (!selectedDifferentGroup && differentGroup)))
                continue;

            if (CSF_AT(int32_t, entry, 0x04) != -1)
            {
                alignas(8) uint8_t endpointBytes[0x40] = {};
                CSF_AT(int32_t, endpointBytes, 0x00) = -1;
                CSF_AT(int32_t, endpointBytes, 0x04) = -1;
                CSF_AT(uint32_t, endpointBytes, 0x0C) =
                    CSF_AT(uint32_t, entry, 0x10);
                CSF_AT(void**, endpointBytes, 0x08) = CSF_VTBL(0x0082D448);
                CSF_AT(uint32_t, endpointBytes, 0x10) =
                    CSF_AT(uint32_t, entry, 0x14);

                const int endpointsMatch = PairLink_TestEndpoints(
                    reinterpret_cast<C_0083e9d0*>(
                        CSF_AT(C_008342e8*, self, 0x00)->field_003c),
                    nullptr, reinterpret_cast<int>(endpointBytes));
                CSF_AT(void**, endpointBytes, 0x08) = CSF_VTBL(0x0082D410);

                if (static_cast<uint8_t>(endpointsMatch) == 0)
                    continue;
            }

            if (C_0083e9d0_ResolveAndDispatchHandleEntry(
                    reinterpret_cast<C_0083e9d0*>(
                        CSF_AT(C_008342e8*, self, 0x00)->field_003c),
                    nullptr, reinterpret_cast<int>(&candidate->field_001c), 4))
            {
                selectedIndex = static_cast<int>(index);
                bestScore = score;
                selectedDifferentGroup = differentGroup;
            }
            else if (differentGroup && score < fallbackScore)
            {
                fallbackIndex = static_cast<int>(index);
                fallbackScore = score;
            }
        }

        DecrementByteCounter(
            reinterpret_cast<void*>(
                CSF_AT(C_008342e8*, self, 0x00)->field_003c),
            nullptr);

        if (!selectedDifferentGroup && fallbackIndex != -1)
            selectedIndex = fallbackIndex;
        if (selectedIndex == -1)
            return 0;
    }

    auto* selected = reinterpret_cast<uint32_t*>(
        CSF_AT(uint8_t*, self, 0x04) +
        52 * static_cast<uint32_t>(selectedIndex));

    output[0] = selected[0];
    output[1] = selected[1];
    output[2] = selected[2];
    output[4] = selected[4];
    output[5] = selected[5];
    output[6] = selected[6];
    output[7] = selected[7];
    output[8] = selected[8];
    output[10] = selected[10];
    output[11] = selected[11];
    output[12] = selected[12];

    if (selectedDifferentGroup)
    {
        auto* entryType = reinterpret_cast<uint32_t*>(
            CSF_AT(uint8_t*, self, 0x04) + 8);
        for (uint32_t index = 0; index < count; ++index, entryType += 13)
        {
            if (entryType[8] == output[10] &&
                (*entryType == 8 || *entryType == 9))
            {
                output[0] = 3;
                break;
            }
        }
    }

    return 1;
}

HOOK(0x004B8980, AiCombat_SelectBestOrder, "auto");
