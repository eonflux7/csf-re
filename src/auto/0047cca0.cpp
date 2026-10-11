#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_FUNC(void*, __fastcall, ResetPairAndCopyEndpoints, 0x00462AE0, (void* self, void* , const void* source));
CSF_FUNC(char*, __fastcall, AiCore_GetTaskTargetNameBuffer, 0x0047AE10, (void* self, void* edx));
CSF_FUNC(int, __fastcall, GetCandidateCount, 0x00489C60, (void* self, void* edx));
CSF_FUNC(int, __fastcall, IsTargetActive, 0x0048E180, (void* self, void* edx));
CSF_FUNC(float*, __fastcall, GetObjectPositionVector, 0x0048E430, (void* self, void* edx));
CSF_FUNC(int, __fastcall, PairLink_PrepareForRebind, 0x004E55A0, (void* self, void* edx));
CSF_FUNC(char, __fastcall, DecrementByteCounter, 0x004E55B0, (void* self, void*));
CSF_FUNC(int, __fastcall, GetCandidateHandle, 0x004E5870, (void* self, void* edx, int a2));
CSF_FUNC(char, __fastcall, PairLink_TestEndpoints, 0x004E5B60, (void* self, void* edx, int a2));

static bool S_0047ae10_AcceptsTarget(S_0047ae10* self, C_008342e8* target)
{
    if (!static_cast<uint8_t>(IsTargetActive(target, nullptr)))
        return false;

    return reinterpret_cast<bool(__fastcall*)(void*, void*, uint32_t)>(
        reinterpret_cast<void**>(self->field_0000)[0xB0 / 4])(
            self, nullptr, target->field_0050);
}

static void InitializeEndpointPair(void* pair, uint32_t first, uint32_t second)
{
    CSF_AT(int, pair, 0x00) = -1;
    CSF_AT(int, pair, 0x04) = -1;
    CSF_AT(void**, pair, 0x08) = CSF_VTBL(0x0082D448);
    CSF_AT(uint32_t, pair, 0x0C) = first;
    CSF_AT(uint32_t, pair, 0x10) = second;
}

static uint32_t* __fastcall S_0047ae10_SelectAttackTarget(
    S_0047ae10* self, void* /*edx*/, uint32_t* result)
{
    alignas(8) uint8_t pairBytes[0x34];
    int selectedFirst = -1;
    int selectedSecond = -1;

    PairLink_PrepareForRebind(
        CSF_AT(void*, reinterpret_cast<void*>(self->field_0014), 0x3C),
        nullptr);

    if (self->field_02ed)
    {
        const uint32_t candidateCount = static_cast<uint32_t>(GetCandidateCount(
            CSF_AT(void*, reinterpret_cast<void*>(self->field_0014), 0x3C),
            nullptr));

        for (uint32_t index = 0; index < candidateCount; ++index)
        {
            void* handle = reinterpret_cast<void*>(GetCandidateHandle(
                CSF_AT(void*, reinterpret_cast<void*>(self->field_0014), 0x3C),
                nullptr, static_cast<int>(index)));
            auto* target = reinterpret_cast<C_008342e8*>(
                CHandleTable_Resolve(g_pCurrentScene, nullptr,
                                     reinterpret_cast<int>(handle)));

            if (!target || !S_0047ae10_AcceptsTarget(self, target))
                continue;

            const uint32_t first = CSF_AT(uint32_t, handle, 0x04);
            if (first != self->field_001c)
                continue;

            InitializeEndpointPair(pairBytes, first,
                                   CSF_AT(uint32_t, handle, 0x08));
            const bool matches = PairLink_TestEndpoints(
                CSF_AT(void*, reinterpret_cast<void*>(self->field_0014), 0x3C),
                nullptr, reinterpret_cast<int>(pairBytes)) != 0;
            CSF_AT(void**, pairBytes, 0x08) = CSF_VTBL(0x0082D410);

            if (matches)
            {
                selectedFirst = CSF_AT(int, handle, 0x04);
                selectedSecond = CSF_AT(int, handle, 0x08);
                break;
            }
        }

        if (selectedSecond < 0)
        {
            if (CSF_AT(int, AiCore_GetTaskTargetNameBuffer(self, nullptr), 0x18) >= 0)
            {
                char* targetName = AiCore_GetTaskTargetNameBuffer(self, nullptr);
                auto* target = reinterpret_cast<C_008342e8*>(
                    CHandleTable_Resolve(g_pCurrentScene, nullptr,
                                         reinterpret_cast<int>(targetName + 0x10)));

                if (target && S_0047ae10_AcceptsTarget(self, target))
                {
                    char* source = AiCore_GetTaskTargetNameBuffer(self, nullptr) + 0x10;
                    InitializeEndpointPair(pairBytes,
                                           CSF_AT(uint32_t, source, 0x04),
                                           CSF_AT(uint32_t, source, 0x08));

                    if (PairLink_TestEndpoints(
                            CSF_AT(void*, reinterpret_cast<void*>(self->field_0014), 0x3C),
                            nullptr, reinterpret_cast<int>(pairBytes)))
                    {
                        char* currentTarget = AiCore_GetTaskTargetNameBuffer(self, nullptr);
                        selectedFirst = CSF_AT(int, currentTarget, 0x14);
                        selectedSecond = CSF_AT(int, currentTarget, 0x18);
                    }
                }
            }

            if (selectedSecond < 0)
            {
                float closestDistance = 3.4028235e38f;
                float* ownerPosition = GetObjectPositionVector(
                    reinterpret_cast<void*>(self->field_0014), nullptr);

                for (uint32_t index = 0; index < candidateCount; ++index)
                {
                    void* handle = reinterpret_cast<void*>(GetCandidateHandle(
                        CSF_AT(void*, reinterpret_cast<void*>(self->field_0014), 0x3C),
                        nullptr, static_cast<int>(index)));
                    auto* target = reinterpret_cast<C_008342e8*>(
                        CHandleTable_Resolve(g_pCurrentScene, nullptr,
                                             reinterpret_cast<int>(handle)));

                    if (!target || !S_0047ae10_AcceptsTarget(self, target))
                        continue;

                    float* position = GetObjectPositionVector(target, nullptr);
                    const float dx = position[0] - ownerPosition[0];
                    const float dy = position[1] - ownerPosition[1];
                    const float dz = position[2] - ownerPosition[2];
                    const float distance = dz * dz + dy * dy + dx * dx;

                    if (distance < closestDistance)
                    {
                        InitializeEndpointPair(pairBytes,
                                               CSF_AT(uint32_t, handle, 0x04),
                                               CSF_AT(uint32_t, handle, 0x08));
                        const bool matches = PairLink_TestEndpoints(
                            CSF_AT(void*, reinterpret_cast<void*>(self->field_0014), 0x3C),
                            nullptr, reinterpret_cast<int>(pairBytes)) != 0;
                        CSF_AT(void**, pairBytes, 0x08) = CSF_VTBL(0x0082D410);

                        if (matches)
                        {
                            selectedFirst = CSF_AT(int, handle, 0x04);
                            selectedSecond = CSF_AT(int, handle, 0x08);
                            closestDistance = distance;
                        }
                    }
                }
            }
        }
    }
    else if (*AiCore_GetTaskTargetNameBuffer(self, nullptr))
    {
        if (CSF_AT(int, AiCore_GetTaskTargetNameBuffer(self, nullptr), 0x18) >= 0)
        {
            char* targetName = AiCore_GetTaskTargetNameBuffer(self, nullptr);
            auto* target = reinterpret_cast<C_008342e8*>(
                CHandleTable_Resolve(g_pCurrentScene, nullptr,
                                     reinterpret_cast<int>(targetName + 0x10)));

            if (target && S_0047ae10_AcceptsTarget(self, target))
            {
                char* source = AiCore_GetTaskTargetNameBuffer(self, nullptr);
                void* pairLink = CSF_AT(
                    void*, reinterpret_cast<void*>(self->field_0014), 0x3C);
                void* pair = ResetPairAndCopyEndpoints(
                    pairBytes, nullptr, source + 0x10);

                if (PairLink_TestEndpoints(pairLink, nullptr,
                                           reinterpret_cast<int>(pair)))
                {
                    char* currentTarget = AiCore_GetTaskTargetNameBuffer(self, nullptr);
                    selectedFirst = CSF_AT(int, currentTarget, 0x14);
                    selectedSecond = CSF_AT(int, currentTarget, 0x18);
                }
            }
        }
    }

    DecrementByteCounter(
        CSF_AT(void*, reinterpret_cast<void*>(self->field_0014), 0x3C),
        nullptr);

    CSF_AT(void**, result, 0x00) = CSF_VTBL(0x0082D448);
    result[1] = static_cast<uint32_t>(selectedFirst);
    result[2] = static_cast<uint32_t>(selectedSecond);
    return result;
}

HOOK(0x0047CCA0, S_0047ae10_SelectAttackTarget, "auto");
