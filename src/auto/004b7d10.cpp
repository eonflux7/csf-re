#include <cstdint>
#include "globals.h"
#include <cstddef>
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, ResolveEntryLookupValue, 0x00471540, (void* self, void* edx));
CSF_FUNC(int, __stdcall, AiStateFactory_ApplyRequestArguments, 0x004AD100, (int a1, int a2, int a3, int a4));
CSF_FUNC(int, __fastcall, ActionArgs_ReadPair, 0x004B7040, (void* self, void* edx, uint32_t* a2, uint32_t* a3));
CSF_FUNC(int, __fastcall, GetLinkedObjectField10, 0x004DB5D0, (void* self, void*));
CSF_FUNC(int, __fastcall, CoverAttack_SetState_4DC150, 0x004DC150, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, AiCoverAttackTasks_ResetLinkedTaskState, 0x004DC380, (void* self, void* edx, int a2));
CSF_FUNC(void*, __fastcall, GetCachedObjectProvider, 0x00686C10, (void* self, void* edx));
CSF_FUNC(void*, __cdecl, _memcpy, 0x007AEE60, (void* destination, const void* source, std::size_t byteCount));

static char __stdcall AiStateFactory_ApplyQueuedCoverAttackRequest(C_0085e760* self)
{
    auto* arguments =
        reinterpret_cast<uint8_t*>(GetLinkedObjectField10(self, nullptr)) + 100;

    uint32_t firstArgument = 0;
    uint32_t secondArgument = 0;
    ActionArgs_ReadPair(arguments, nullptr, &firstArgument, &secondArgument);

    const int queuedObject = CSF_AT(int, CSF_AT(void*, arguments, 0x10), 0);
    const uint32_t remaining = CSF_AT(uint32_t, arguments, 0x18) - 1;
    if (remaining == 0)
    {
        CSF_AT(uint32_t, arguments, 0x18) = 0;
    }
    else
    {
        CSF_AT(uint32_t, arguments, 0x18) = remaining;
        auto* entries = CSF_AT(uint8_t*, arguments, 0x10);
        _memcpy(entries, entries + 4, uint32_t(4 * remaining));
    }

    auto* linkedTask =
        reinterpret_cast<uint8_t*>(GetLinkedObjectField10(self, nullptr)) + 88;
    AiCoverAttackTasks_ResetLinkedTaskState(
        self, nullptr, reinterpret_cast<int>(linkedTask));

    void* entry = reinterpret_cast<void*>(ResolveEntryLookupValue(self, nullptr));
    void* stateProvider = GetCachedObjectProvider(entry, nullptr);

    auto* state =
        reinterpret_cast<uint8_t*>(GetLinkedObjectField10(self, nullptr)) + 92;
    CoverAttack_SetState_4DC150(
        stateProvider, nullptr, reinterpret_cast<int>(state));

    const int savedFirstArgument = static_cast<int>(firstArgument);
    const int savedSecondArgument = static_cast<int>(secondArgument);
    void* requestProvider = GetCachedObjectProvider(stateProvider, nullptr);
    AiStateFactory_ApplyRequestArguments(
        reinterpret_cast<int>(requestProvider),
        savedFirstArgument, savedSecondArgument, queuedObject);
    return 1;
}

HOOK(0x004B7D10, AiStateFactory_ApplyQueuedCoverAttackRequest, "auto");
