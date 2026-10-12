#include <cstdint>
#include "globals.h"
#include <cstddef>
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(int, __fastcall, GetLinkedObjectValue_4DB5C0, 0x004DB5C0, (void* self, void*));
CSF_FUNC(int, __fastcall, GetLinkedObjectField10, 0x004DB5D0, (void* self, void*));
CSF_FUNC(int, __fastcall, AiCoverAttackTasks_ResetLinkedTaskState, 0x004DC380, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, GetCurrentItemCount, 0x00681F40, (void* self, void* edx));
CSF_FUNC(void*, __cdecl, _memcpy, 0x007AEE60, (void* destination, const void* source, std::size_t byteCount));

static char __stdcall AiStateFactory_HandleRequest8(int request)
{
    void* self = reinterpret_cast<void*>(request);
    void* linkedFields = reinterpret_cast<void*>(GetLinkedObjectField10(self, nullptr));
    float* firstEntry = CSF_AT(float*, linkedFields, 0x74);
    const float threshold = *firstEntry;
    const int remaining = CSF_AT(int, linkedFields, 0x7C) - 1;

    CSF_AT(int, linkedFields, 0x7C) = remaining;
    if (remaining != 0)
    {
        void* entries = CSF_AT(void*, linkedFields, 0x74);
        _memcpy(entries, static_cast<std::uint8_t*>(entries) + 4,
                static_cast<std::uint32_t>(remaining) * 4u);
    }

    const int linkedValue = GetLinkedObjectValue_4DB5C0(self, nullptr);
    void* owner = reinterpret_cast<void*>(linkedValue ? linkedValue - 4 : 0);
    void* ownerMember = CSF_AT(void*, owner, 0x2C);
    void* provider = CSF_AT(void*, ownerMember, 0x44);
    void* items = reinterpret_cast<void* (__fastcall*)(void*, void*)>(
        CSF_AT(void**, provider, 0)[0xD8 / 4])(provider, nullptr);
    const int count = GetCurrentItemCount(items, nullptr);
    const int requestedCount = static_cast<int>(static_cast<std::int64_t>(threshold));

    if (count < requestedCount)
    {
        const int fields = GetLinkedObjectField10(self, nullptr);
        AiCoverAttackTasks_ResetLinkedTaskState(self, nullptr, fields + 0x14);
        return 1;
    }
    return 0;
}

HOOK(0x004B7230, AiStateFactory_HandleRequest8, "auto");
