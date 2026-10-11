#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_FUNC(int, __fastcall, GetCandidateCount, 0x00489C60, (void* self, void* edx));
CSF_FUNC(int, __fastcall, SceneObj_IsInactive, 0x0048E100, (void* self, void* edx));
CSF_FUNC(int, __fastcall, SceneObject_PassesFirstCheck, 0x0048E120, (void* self, void* edx));
CSF_FUNC(bool, __fastcall, C_0083dd58_IsCandidateEligible, 0x004D0410, (void* self, void* edx, int a2, int a3));
CSF_FUNC(int, __fastcall, PairLink_PrepareForRebind, 0x004E55A0, (C_0083e9d0* self, void*));
CSF_FUNC(char, __fastcall, DecrementByteCounter, 0x004E55B0, (void* self, void*));
CSF_FUNC(int, __fastcall, C_00832eec_GetCandidateHandle, 0x004E5870, (C_00832eec* self, void* , int index));
CSF_FUNC(int, __fastcall, PairLink_TestEndpoints, 0x004E5B60, (C_0083e9d0* self, void* , int a2));

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

static std::uint32_t* __fastcall C_0083dd58_FindCompatibleCandidate(
    C_0083dd58* self, void* /*edx*/, int selectionContext)
{
    void* owner = reinterpret_cast<void*>(self->field_0014);
    auto* pairLink = reinterpret_cast<C_0083e9d0*>(
        CSF_AT(std::uint32_t, owner, 0x3C));
    if (!pairLink)
        return nullptr;

    PairLink_PrepareForRebind(pairLink, nullptr);

    owner = reinterpret_cast<void*>(self->field_0014);
    const auto candidateCount = static_cast<std::uint32_t>(
        GetCandidateCount(
            reinterpret_cast<void*>(CSF_AT(std::uint32_t, owner, 0x3C)),
            nullptr));

    std::uint32_t* selected = nullptr;
    for (std::uint32_t index = 0; index < candidateCount; ++index)
    {
        owner = reinterpret_cast<void*>(self->field_0014);
        const int handle = C_00832eec_GetCandidateHandle(
            reinterpret_cast<C_00832eec*>(
                CSF_AT(std::uint32_t, owner, 0x3C)),
            nullptr, static_cast<int>(index));

        void* candidate = reinterpret_cast<void*>(
            CHandleTable_Resolve(g_pCurrentScene, nullptr, handle));
        if (!candidate)
            continue;
        if (static_cast<std::uint8_t>(
                SceneObj_IsInactive(candidate, nullptr)) != 0)
            continue;
        if (static_cast<std::uint8_t>(
                SceneObject_PassesFirstCheck(candidate, nullptr)) != 0)
            continue;
        if (!C_0083dd58_IsCandidateEligible(
                self, nullptr, handle, selectionContext))
            continue;

        void* handleData = reinterpret_cast<void*>(handle);
        const int secondEndpoint = CSF_AT(int, handleData, 0x08);
        owner = reinterpret_cast<void*>(self->field_0014);

        alignas(8) std::uint8_t endpointsBytes[0x24];
        CSF_AT(int, endpointsBytes, 0x00) = -1;
        CSF_AT(int, endpointsBytes, 0x04) = -1;
        CSF_AT(int, endpointsBytes, 0x0C) =
            CSF_AT(int, handleData, 0x04);
        CSF_AT(void**, endpointsBytes, 0x08) = CSF_VTBL(0x0082D448);
        CSF_AT(int, endpointsBytes, 0x10) = secondEndpoint;

        const bool endpointsMatch = static_cast<std::uint8_t>(
            PairLink_TestEndpoints(
                reinterpret_cast<C_0083e9d0*>(
                    CSF_AT(std::uint32_t, owner, 0x3C)),
                nullptr, reinterpret_cast<int>(endpointsBytes))) != 0;

        CSF_AT(void**, endpointsBytes, 0x08) = CSF_VTBL(0x0082D410);

        if (endpointsMatch)
        {
            selected = reinterpret_cast<std::uint32_t*>(candidate);
            break;
        }
    }

    owner = reinterpret_cast<void*>(self->field_0014);
    DecrementByteCounter(
        reinterpret_cast<void*>(CSF_AT(std::uint32_t, owner, 0x3C)),
        nullptr);
    return selected;
}

HOOK(0x004D0750, C_0083dd58_FindCompatibleCandidate, "auto");
