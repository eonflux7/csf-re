#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(char*, __fastcall, AiCore_GetTaskTargetNameBuffer, 0x0047AE10, (void* self, void* edx));
CSF_FUNC(void, __fastcall, TFncGirar_OnRotationComplete, 0x004DB1D0, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, CharTask_LookupByOffset, 0x004DB440, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, PairLink_PrepareForRebind, 0x004E55A0, (void* self, void* edx));
CSF_FUNC(char, __fastcall, DecrementByteCounter, 0x004E55B0, (void* self, void*));
CSF_FUNC(char, __fastcall, PairLink_TestEndpoints, 0x004E5B60, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, GetCurrentItemCount, 0x00681F40, (void* self, void* edx));

static void __fastcall TCharTask_CheckTargetAndContinue(
    TCharTask* self, void* /*edx*/, int /*unused1*/, int /*unused2*/)
{
    void* character = CSF_AT(void*, CSF_AT(void*, self, 0x08), 0x14);
    void* pairLink = CSF_AT(void*, character, 0x3C);
    void* itemProvider = CSF_AT(void*, character, 0x44);

    alignas(8) uint8_t endpointBytes[0x18];
    CSF_AT(int, endpointBytes, 0x14) = 0;
    bool targetMatches = false;

    PairLink_PrepareForRebind(pairLink, nullptr);

    void* items =
        reinterpret_cast<void* (__fastcall*)(void*, void*)>(
            CSF_AT(void**, itemProvider, 0)[0xD8 / 4])(
                itemProvider, nullptr);

    if (GetCurrentItemCount(items, nullptr) != 0)
    {
        char* targetName = AiCore_GetTaskTargetNameBuffer(
            CSF_AT(void*, self, 0x08), nullptr);

        if (CSF_AT(int, targetName, 0x18) >= 0)
        {
            char* targetFields = AiCore_GetTaskTargetNameBuffer(
                CSF_AT(void*, self, 0x08), nullptr) + 16;

            CSF_AT(int, endpointBytes, 0x00) = -1;
            CSF_AT(int, endpointBytes, 0x04) = -1;
            CSF_AT(void**, endpointBytes, 0x08) = CSF_VTBL(0x0082D448);
            CSF_AT(uint32_t, endpointBytes, 0x0C) =
                CSF_AT(uint32_t, targetFields, 0x04);
            CSF_AT(uint32_t, endpointBytes, 0x10) =
                CSF_AT(uint32_t, targetFields, 0x08);

            targetMatches = PairLink_TestEndpoints(
                CSF_AT(void*, CSF_AT(void*, CSF_AT(void*, self, 0x08), 0x14), 0x3C),
                nullptr, reinterpret_cast<int>(endpointBytes)) != 0;
        }
    }

    DecrementByteCounter(
        CSF_AT(void*, CSF_AT(void*, CSF_AT(void*, self, 0x08), 0x14), 0x3C),
        nullptr);

    if (targetMatches)
    {
        CharTask_LookupByOffset(
            self, nullptr,
            static_cast<int>(CSF_AT(uint32_t, self, 0x0C) + 20));
    }
    else
    {
        TFncGirar_OnRotationComplete(self, nullptr, 0);
    }
}

HOOK(0x0047C510, TCharTask_CheckTargetAndContinue, "auto");
