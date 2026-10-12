#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, ResolveEntryLookupValue, 0x00471540, (void* self, void* edx));
CSF_FUNC(int, __stdcall, AiStateFactory_ApplyRequestData, 0x004ACED0, (void* a1, uint32_t* a2));
CSF_FUNC(int, __fastcall, AiStateFactory_ReadRequestData, 0x004B1310, (void* self, void* edx, uint32_t* a2));
CSF_FUNC(int, __fastcall, GetLinkedObjectField10, 0x004DB5D0, (void* self, void*));
CSF_FUNC(int, __fastcall, AiCoverAttackTasks_ResetLinkedTaskState, 0x004DC380, (void* self, void* edx, int a2));
CSF_FUNC(void*, __fastcall, GetCachedObjectProvider, 0x00686C10, (void* self, void* edx));

static char __stdcall AiStateFactory_HandleRequest22(C_0085e760* self)
{
    alignas(8) uint8_t requestBytes[0xC];
    std::memset(requestBytes, 0, sizeof(requestBytes));
    auto* requestData = reinterpret_cast<uint32_t*>(requestBytes);

    int linkedObject = GetLinkedObjectField10(self, nullptr);
    AiStateFactory_ReadRequestData(
        reinterpret_cast<void*>(linkedObject + 0x64), nullptr, requestData);

    linkedObject = GetLinkedObjectField10(self, nullptr);
    AiCoverAttackTasks_ResetLinkedTaskState(self, nullptr, linkedObject + 0x20);

    int entry = ResolveEntryLookupValue(self, nullptr);
    void* provider = GetCachedObjectProvider(reinterpret_cast<void*>(entry), nullptr);

    GetLinkedObjectField10(self, nullptr);
    AiStateFactory_ApplyRequestData(provider, requestData);
    return 1;
}

HOOK(0x004B75D0, AiStateFactory_HandleRequest22, "auto");
