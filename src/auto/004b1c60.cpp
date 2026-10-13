#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(char*, __fastcall, AiCore_GetTaskTargetNameBuffer, 0x0047AE10, (S_0047ae10* self, void*));
CSF_FUNC(int, __stdcall, AiAlert_ApplyCoverAttack, 0x004ABF50, (int a1, uint32_t* a2));
CSF_FUNC(int, __fastcall, AiCore_UpdateTargetData, 0x004D1730, (S_0048c9f0* self, void* , int a2));
CSF_FUNC(int, __fastcall, GetLinkedObjectValue_4DB5C0, 0x004DB5C0, (void* self, void*));
CSF_FUNC(int, __fastcall, GetLinkedObjectField10, 0x004DB5D0, (void* self, void*));
CSF_FUNC(int, __fastcall, CoverAttack_SetState_4DC150, 0x004DC150, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, UnnamedApply_4DC180, 0x004DC180, (void* self, void* edx, int a2));
CSF_FUNC(void*, __fastcall, GetCachedObjectProvider, 0x00686C10, (void* self, void* edx));

static int __stdcall AiFncMacroOrder_ApplyRequest26Provider(C_0085e7b8* self)
{
    // Retain the vtable binding identified by the packet.
    (void)CSF_VTBL(0x0047AE10);

    uint32_t linkedValue = static_cast<uint32_t>(
        GetLinkedObjectValue_4DB5C0(self, nullptr));
    uint32_t linkedBase = linkedValue ? linkedValue - 4 : 0;

    S_0047ae10* target = CSF_AT(
        S_0047ae10*, reinterpret_cast<void*>(linkedBase), 0x718);

    int targetData = static_cast<int>(
        static_cast<uint32_t>(GetLinkedObjectField10(self, nullptr)) + 148);
    char* targetName = AiCore_GetTaskTargetNameBuffer(target, nullptr);
    AiCore_UpdateTargetData(
        reinterpret_cast<S_0048c9f0*>(targetName), nullptr, targetData);

    alignas(8) uint8_t requestBytes[0x10]{};
    CSF_AT(uint32_t, requestBytes, 0x04) = 0;
    CSF_AT(uint32_t, requestBytes, 0x08) = 0;
    CSF_AT(uint32_t, requestBytes, 0x0C) = 0;
    requestBytes[0] = 1;
    requestBytes[1] = 0;

    linkedValue = static_cast<uint32_t>(
        GetLinkedObjectValue_4DB5C0(self, nullptr));
    linkedBase = linkedValue ? linkedValue - 4 : 0;
    UnnamedApply_4DC180(self, nullptr, static_cast<int>(linkedBase + 984));

    linkedValue = static_cast<uint32_t>(
        GetLinkedObjectValue_4DB5C0(self, nullptr));
    linkedBase = linkedValue ? linkedValue - 4 : 0;

    void* provider = GetCachedObjectProvider(self, nullptr);
    CSF_AT(uint8_t, reinterpret_cast<void*>(linkedBase), 0x400) = 1;
    CoverAttack_SetState_4DC150(
        provider, nullptr, static_cast<int>(linkedBase + 988));

    void* attackProvider = GetCachedObjectProvider(provider, nullptr);
    return AiAlert_ApplyCoverAttack(
        static_cast<int>(reinterpret_cast<uintptr_t>(attackProvider)),
        reinterpret_cast<uint32_t*>(requestBytes));
}

HOOK(0x004B1C60, AiFncMacroOrder_ApplyRequest26Provider, "auto");
