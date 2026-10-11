#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_FUNC(char*, __fastcall, AiCore_GetTaskTargetNameBuffer, 0x0047AE10, (void* self, void* edx));
CSF_FUNC(int, __fastcall, SceneObj_IsInactive, 0x0048E100, (void* self, void* edx));
CSF_FUNC(int, __fastcall, SceneObject_PassesFirstCheck, 0x0048E120, (void* self, void* edx));
CSF_FUNC(int, __fastcall, PairLink_PrepareForRebind, 0x004E55A0, (void* self, void* edx));
CSF_FUNC(char, __fastcall, DecrementByteCounter, 0x004E55B0, (void* self, void*));
CSF_FUNC(char, __fastcall, PairLink_TestEndpoints, 0x004E5B60, (void* self, void* edx, int a2));

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

static char __fastcall S_0047ae10_TestTargetEndpoints(
    S_0047ae10* self, void* /*edx*/)
{
    char* targetBuffer = AiCore_GetTaskTargetNameBuffer(self, nullptr);
    if (CSF_AT(int32_t, targetBuffer, 0x18) < 0)
        return 0;

    char* targetHandle = targetBuffer + 0x10;
    void* pairLink = CSF_AT(
        void*, reinterpret_cast<void*>(self->field_0014), 0x3C);

    void* target = reinterpret_cast<void*>(
        CHandleTable_Resolve(
            g_pCurrentScene, nullptr, reinterpret_cast<int>(targetHandle)));

    if (!target)
        return 0;
    if (static_cast<uint8_t>(SceneObj_IsInactive(target, nullptr)) != 0)
        return 0;
    if (static_cast<uint8_t>(SceneObject_PassesFirstCheck(target, nullptr)) != 0)
        return 0;

    PairLink_PrepareForRebind(pairLink, nullptr);

    const int32_t handleValue = CSF_AT(int32_t, targetHandle, 0x4);
    const int32_t handleIndex = CSF_AT(int32_t, targetHandle, 0x8);

    alignas(8) uint8_t endpointBytes[0x14];
    CSF_AT(int32_t, endpointBytes, 0x0) = -1;
    CSF_AT(int32_t, endpointBytes, 0x4) = -1;
    CSF_AT(void**, endpointBytes, 0x8) = CSF_VTBL(0x0082D448);
    CSF_AT(int32_t, endpointBytes, 0xC) = handleValue;
    CSF_AT(int32_t, endpointBytes, 0x10) = handleIndex;

    const char result = PairLink_TestEndpoints(
        pairLink, nullptr, reinterpret_cast<int>(endpointBytes));
    DecrementByteCounter(pairLink, nullptr);
    return result;
}

HOOK(0x0047D020, S_0047ae10_TestTargetEndpoints, "auto");
