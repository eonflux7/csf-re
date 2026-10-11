#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(char*, __fastcall, AiCore_GetTaskTargetNameBuffer, 0x0047AE10, (void* self, void* edx));
CSF_FUNC(void, __fastcall, TCharTask_SetAttackTarget, 0x0047C3E0, (void* self, void* edx, void* a2));
CSF_FUNC(void, __fastcall, TFncGirar_OnRotationComplete, 0x004DB1D0, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, CharTask_LookupByOffset, 0x004DB440, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, PairLink_PrepareForRebind, 0x004E55A0, (void* self, void* edx));
CSF_FUNC(char, __fastcall, DecrementByteCounter, 0x004E55B0, (void* self, void*));
CSF_FUNC(char, __fastcall, PairLink_TestEndpoints, 0x004E5B60, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, GetCurrentItemCount, 0x00681F40, (void* self, void* edx));

static void __fastcall TCharTask_SelectAttackContinuation(
    TCharTask* self, void* /*edx*/, int /*a2*/, int /*a3*/)
{
    alignas(8) uint8_t endpointBytes[0x18];
    CSF_AT(int, endpointBytes, 0x14) = 0;

    // The packet also lists the getter's code address as a vtable reference.
    (void)CSF_VTBL(0x0047AE10);

    void* inventory = CSF_AT(
        void*, CSF_AT(void*, CSF_AT(void*, self, 0x08), 0x14), 0x44);
    void* items =
        ((void* (__fastcall*)(void*, void*))
             CSF_AT(void**, inventory, 0)[0xD8 / 4])(inventory, nullptr);

    if (!GetCurrentItemCount(items, nullptr))
    {
        TFncGirar_OnRotationComplete(self, nullptr, 0);
        return;
    }

    PairLink_PrepareForRebind(
        CSF_AT(void*, CSF_AT(void*, CSF_AT(void*, self, 0x08), 0x14), 0x3C),
        nullptr);

    char* target = AiCore_GetTaskTargetNameBuffer(
        CSF_AT(void*, self, 0x08), nullptr);

    if (CSF_AT(int, target, 0x18) >= 0)
    {
        target = AiCore_GetTaskTargetNameBuffer(
            CSF_AT(void*, self, 0x08), nullptr);

        CSF_AT(int, endpointBytes, 0x00) = -1;
        CSF_AT(int, endpointBytes, 0x04) = -1;
        void* core = CSF_AT(void*, self, 0x08);
        target += 0x10;
        CSF_AT(void**, endpointBytes, 0x08) = CSF_VTBL(0x0082D448);
        CSF_AT(uint32_t, endpointBytes, 0x0C) = CSF_AT(uint32_t, target, 0x04);
        CSF_AT(uint32_t, endpointBytes, 0x10) = CSF_AT(uint32_t, target, 0x08);

        if (PairLink_TestEndpoints(
                CSF_AT(void*, CSF_AT(void*, core, 0x14), 0x3C),
                nullptr, reinterpret_cast<int>(endpointBytes)))
        {
            int attackTask = CharTask_LookupByOffset(
                self, nullptr, CSF_AT(int, self, 0x0C) + 60);
            target = AiCore_GetTaskTargetNameBuffer(
                CSF_AT(void*, self, 0x08), nullptr);
            TCharTask_SetAttackTarget(
                reinterpret_cast<void*>(attackTask), nullptr, target + 0x10);

            DecrementByteCounter(
                CSF_AT(void*, CSF_AT(void*, CSF_AT(void*, self, 0x08), 0x14), 0x3C),
                nullptr);
            return;
        }
    }

    CharTask_LookupByOffset(self, nullptr, CSF_AT(int, self, 0x0C) + 20);
    DecrementByteCounter(
        CSF_AT(void*, CSF_AT(void*, CSF_AT(void*, self, 0x08), 0x14), 0x3C),
        nullptr);
}

HOOK(0x0047C6A0, TCharTask_SelectAttackContinuation, "auto");
