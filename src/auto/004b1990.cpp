#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(char*, __fastcall, AiCore_GetTaskTargetNameBuffer, 0x0047AE10, (S_0047ae10* self, void*));
CSF_FUNC(int, __stdcall, AiAlert_ApplyCoverAttack, 0x004ABF50, (int a1, uint32_t* a2));
CSF_FUNC(int, __fastcall, AiCore_UpdateTargetData, 0x004D1730, (S_0048c9f0* self, void* , int a2));
CSF_FUNC(int, __fastcall, GetLinkedObjectValue_4DB5C0, 0x004DB5C0, (void* self, void*));
CSF_FUNC(int, __fastcall, GetLinkedObjectField10, 0x004DB5D0, (void* self, void*));
CSF_FUNC(int, __fastcall, CoverAttack_SetState_4DC150, 0x004DC150, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, UnnamedApply_4DC180, 0x004DC180, (void* self, void* edx, int a2));
CSF_FUNC(void*, __fastcall, GetCachedObjectProvider, 0x00686C10, (void* self, void* edx));

static int __stdcall AiFncMacroOrder_ApplyCachedProvider(C_0085e7b8* self)
{
    // Packet-listed vtable address; the actual instruction is a direct call.
    (void)CSF_VTBL(0x0047AE10);

    int linkedObject = GetLinkedObjectValue_4DB5C0(self, nullptr);
    uint32_t ownerAddress = linkedObject ? static_cast<uint32_t>(linkedObject) - 4 : 0;
    auto* aiCore = CSF_AT(S_0047ae10*, reinterpret_cast<void*>(ownerAddress), 0x718);

    int targetDataAddress = GetLinkedObjectField10(self, nullptr) + 148;
    auto* targetNameBuffer = reinterpret_cast<S_0048c9f0*>(
        AiCore_GetTaskTargetNameBuffer(aiCore, nullptr));
    AiCore_UpdateTargetData(targetNameBuffer, nullptr, targetDataAddress);

    linkedObject = GetLinkedObjectValue_4DB5C0(self, nullptr);
    ownerAddress = linkedObject ? static_cast<uint32_t>(linkedObject) - 4 : 0;
    UnnamedApply_4DC180(self, nullptr, static_cast<int>(ownerAddress + 984));

    linkedObject = GetLinkedObjectValue_4DB5C0(self, nullptr);
    ownerAddress = linkedObject ? static_cast<uint32_t>(linkedObject) - 4 : 0;
    void* cachedProvider = GetCachedObjectProvider(self, nullptr);

    CSF_AT(uint8_t, reinterpret_cast<void*>(ownerAddress), 0x400) = 1;

    alignas(8) uint8_t attackOptionsBytes[0x10]{};
    CSF_AT(uint8_t, attackOptionsBytes, 0x01) = 1;
    CSF_AT(uint32_t, attackOptionsBytes, 0x04) = 0;
    CSF_AT(uint32_t, attackOptionsBytes, 0x08) = 0;
    CSF_AT(uint32_t, attackOptionsBytes, 0x0C) = 0;
    CSF_AT(uint8_t, attackOptionsBytes, 0x00) = 0;

    CoverAttack_SetState_4DC150(
        cachedProvider, nullptr, static_cast<int>(ownerAddress + 988));

    void* attackProvider = GetCachedObjectProvider(cachedProvider, nullptr);
    return AiAlert_ApplyCoverAttack(
        reinterpret_cast<int>(attackProvider),
        reinterpret_cast<uint32_t*>(attackOptionsBytes));
}

HOOK(0x004B1990, AiFncMacroOrder_ApplyCachedProvider, "auto");
