#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, GetLinkedObjectField10, 0x004DB5D0, (void* self, void*));
CSF_FUNC(int, __fastcall, AiCoverAttackTasks_ResetLinkedTaskState, 0x004DC380, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, ResolveEntryLookupValue, 0x00471540, (void* self, void* edx));
CSF_FUNC(void*, __fastcall, GetCachedObjectProvider, 0x00686C10, (void* self, void* edx));
CSF_FUNC(void, __stdcall, AiStateFactory_NotifyCachedProviderWithEntry, 0x004C0510, (void* a1, float a2, float a3));
CSF_FUNC(void*, __cdecl, _memcpy, 0x007AEE60, (void* destination, const void* source, std::size_t byteCount));

// Pops the first entry of the list stored in the linked object, shrinks the list
// (shifting the rest down), resets the linked task state and notifies the cached
// provider with the popped entry. Always returns 1.
static char __stdcall AiStateFactory_DequeueFirstEntryAndNotify(C_0085e760* a1)
{
    int linkedObjectField10 = GetLinkedObjectField10(a1, nullptr);
    void* linkedObject = reinterpret_cast<void*>(linkedObjectField10);

    // First element of the list: **(linked + 116)
    uint32_t listBuffer = CSF_AT(uint32_t, linkedObject, 116);
    C_0085e760* firstEntry = *reinterpret_cast<C_0085e760**>(listBuffer);

    // Sub-object at linked + 100 holds the list buffer (+16) and count (+24)
    int subObjectOffsetBase = linkedObjectField10 + 100;
    void* subObject = reinterpret_cast<void*>(subObjectOffsetBase);

    // Remaining count after removing the head (stored at linked + 124)
    int remainingCount = CSF_AT(int, linkedObject, 124) - 1;
    if (remainingCount != 0)
    {
        CSF_AT(int, subObject, 24) = remainingCount;
        _memcpy(reinterpret_cast<void*>(CSF_AT(uint32_t, subObject, 16)),
                reinterpret_cast<const void*>(CSF_AT(uint32_t, subObject, 16) + 4),
                static_cast<std::size_t>(4 * remainingCount));
    }
    else
    {
        CSF_AT(int, subObject, 24) = 0;
    }

    GetLinkedObjectField10(a1, nullptr);
    int taskLinkedObjectField10 = GetLinkedObjectField10(a1, nullptr);
    AiCoverAttackTasks_ResetLinkedTaskState(a1, nullptr, taskLinkedObjectField10 + 36);

    int resolvedEntry = ResolveEntryLookupValue(a1, nullptr);
    void* cachedProvider = GetCachedObjectProvider(reinterpret_cast<void*>(resolvedEntry), nullptr);
    GetLinkedObjectField10(a1, nullptr);

    // The popped entry pointer is passed bit-for-bit as a float argument.
    float entryAsFloat = *reinterpret_cast<float*>(&firstEntry);
    AiStateFactory_NotifyCachedProviderWithEntry(cachedProvider, entryAsFloat, 0.0f);

    return 1;
}

HOOK(0x4C3C60, AiStateFactory_DequeueFirstEntryAndNotify, "auto");
