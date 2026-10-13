#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(char*, __fastcall, AiCore_GetTaskTargetNameBuffer, 0x0047AE10, (S_0047ae10* self, void*));
CSF_FUNC(int, __fastcall, AiCore_UpdateTargetData, 0x004D1730, (S_0048c9f0* self, void* , int a2));
CSF_FUNC(int, __fastcall, GetLinkedObjectValue_4DB5C0, 0x004DB5C0, (void* self, void*));
CSF_FUNC(int, __fastcall, GetLinkedObjectField10, 0x004DB5D0, (void* self, void*));
CSF_FUNC(int, __fastcall, CoverAttack_SetState_4DC150, 0x004DC150, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, UnnamedApply_4DC180, 0x004DC180, (void* self, void* edx, int a2));
CSF_FUNC(void*, __fastcall, GetCachedObjectProvider, 0x00686C10, (void* self, void* edx));

static int __stdcall AiOrder_ApplyRequestData(
    C_0085e7b8* self, const std::uint32_t* requestData)
{
    // Retain the packet's vtable binding; the actual instruction is a direct call.
    (void)CSF_VTBL(0x0047AE10);

    int linkedValue = GetLinkedObjectValue_4DB5C0(self, nullptr);
    std::uintptr_t linkedBase = linkedValue
        ? static_cast<std::uintptr_t>(linkedValue) - 4
        : 0;

    auto* aiCore = CSF_AT(
        S_0047ae10*, reinterpret_cast<void*>(linkedBase), 0x718);

    const int targetDataAddress = static_cast<int>(
        static_cast<std::uint32_t>(GetLinkedObjectField10(self, nullptr)) + 148);

    char* targetBuffer = AiCore_GetTaskTargetNameBuffer(aiCore, nullptr);
    AiCore_UpdateTargetData(
        reinterpret_cast<S_0048c9f0*>(targetBuffer), nullptr, targetDataAddress);

    linkedValue = GetLinkedObjectValue_4DB5C0(self, nullptr);
    linkedBase = linkedValue
        ? static_cast<std::uintptr_t>(linkedValue) - 4
        : 0;
    UnnamedApply_4DC180(self, nullptr, static_cast<int>(linkedBase + 984));

    linkedValue = GetLinkedObjectValue_4DB5C0(self, nullptr);
    linkedBase = linkedValue
        ? static_cast<std::uintptr_t>(linkedValue) - 4
        : 0;

    void* stateProvider = GetCachedObjectProvider(self, nullptr);

    CSF_AT(std::uint8_t, reinterpret_cast<void*>(linkedBase), 0x400) = 1;
    CSF_AT(std::uint32_t, reinterpret_cast<void*>(linkedBase), 0x3F4) = requestData[0];
    CSF_AT(std::uint32_t, reinterpret_cast<void*>(linkedBase), 0x3F8) = requestData[1];
    CSF_AT(std::uint32_t, reinterpret_cast<void*>(linkedBase), 0x3FC) = requestData[2];

    return CoverAttack_SetState_4DC150(
        stateProvider, nullptr, static_cast<int>(linkedBase + 992));
}

HOOK(0x004ACED0, AiOrder_ApplyRequestData, "auto");
