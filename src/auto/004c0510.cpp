#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(char*, __fastcall, AiCore_GetTaskTargetNameBuffer, 0x0047AE10, (S_0047ae10* self, void*));
CSF_FUNC(void, __fastcall, AiStateFactory_BuildDirectionalAttackRecords, 0x004B8510, (void* self, void* edx, int a2, float a3, float a4));
CSF_FUNC(int, __fastcall, AiCore_UpdateTargetData, 0x004D1730, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, GetLinkedObjectValue_4DB5C0, 0x004DB5C0, (void* self, void*));
CSF_FUNC(int, __fastcall, GetLinkedObjectField10, 0x004DB5D0, (void* self, void*));
CSF_FUNC(int, __fastcall, CoverAttack_SetState_4DC150, 0x004DC150, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, UnnamedApply_4DC180, 0x004DC180, (void* self, void* edx, int a2));
CSF_FUNC(void*, __fastcall, GetCachedObjectProvider, 0x00686C10, (void* self, void* edx));

static void __stdcall AiStateFactory_UpdateCoverAttack(
    C_0085e7b8* object, float horizontal, float vertical)
{
    // Listed by the packet as a vtable reference; the instruction is a direct call.
    (void)CSF_VTBL(0x0047AE10);

    int linkedValue = GetLinkedObjectValue_4DB5C0(object, nullptr);
    std::uint32_t linkedBase =
        linkedValue ? static_cast<std::uint32_t>(linkedValue) - 4 : 0;

    S_0047ae10* taskOwner = CSF_AT(
        S_0047ae10*, reinterpret_cast<void*>(linkedBase), 0x718);

    const int targetData = static_cast<int>(
        static_cast<std::uint32_t>(GetLinkedObjectField10(object, nullptr)) + 148);
    char* targetBuffer = AiCore_GetTaskTargetNameBuffer(taskOwner, nullptr);
    AiCore_UpdateTargetData(targetBuffer, nullptr, targetData);

    linkedValue = GetLinkedObjectValue_4DB5C0(object, nullptr);
    linkedBase = linkedValue ? static_cast<std::uint32_t>(linkedValue) - 4 : 0;
    UnnamedApply_4DC180(object, nullptr, static_cast<int>(linkedBase + 1440));

    linkedValue = GetLinkedObjectValue_4DB5C0(object, nullptr);
    linkedBase = linkedValue ? static_cast<std::uint32_t>(linkedValue) - 4 : 0;

    void* provider = GetCachedObjectProvider(object, nullptr);
    const std::uint32_t attackRecordsAddress = linkedBase + 1444;
    CoverAttack_SetState_4DC150(
        provider, nullptr, static_cast<int>(attackRecordsAddress));

    void* stateObject = GetCachedObjectProvider(provider, nullptr);
    AiStateFactory_BuildDirectionalAttackRecords(
        reinterpret_cast<void*>(attackRecordsAddress), nullptr,
        static_cast<int>(reinterpret_cast<std::uintptr_t>(stateObject)),
        horizontal, vertical);
}

HOOK(0x004C0510, AiStateFactory_UpdateCoverAttack, "auto");
