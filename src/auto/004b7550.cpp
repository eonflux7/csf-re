#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, ResolveEntryLookupValue, 0x00471540, (void* self, void* edx));
CSF_FUNC(int, __stdcall, AiStateFactory_ApplyCachedProvider, 0x004B1990, (void* a1));
CSF_FUNC(int, __fastcall, GetLinkedObjectValue_4DB5C0, 0x004DB5C0, (void* self, void*));
CSF_FUNC(int, __fastcall, GetLinkedObjectField10, 0x004DB5D0, (void* self, void*));
CSF_FUNC(int, __fastcall, AiCoverAttackTasks_ResetLinkedTaskState, 0x004DC380, (void* self, void* edx, int a2));
CSF_FUNC(void*, __fastcall, GetCachedObjectProvider, 0x00686C10, (void* self, void* edx));

CSF_GLOBAL(float, g_fZero, 0x0082D4CC);

static int __stdcall AiStateFactory_HandleRequest17(C_0085e760* self)
{
    int linkedValue = GetLinkedObjectValue_4DB5C0(self, nullptr);
    void* linkedObject = linkedValue
        ? reinterpret_cast<void*>(linkedValue - 4)
        : nullptr;

    // Unordered comparisons also satisfy the original condition.
    if (CSF_AT(float, linkedObject, 0x260) > g_fZero)
        return 0;

    int linkedField = GetLinkedObjectField10(self, nullptr);
    AiCoverAttackTasks_ResetLinkedTaskState(self, nullptr, linkedField + 32);

    void* entry = reinterpret_cast<void*>(ResolveEntryLookupValue(self, nullptr));
    void* provider = GetCachedObjectProvider(entry, nullptr);
    GetLinkedObjectField10(self, nullptr);
    AiStateFactory_ApplyCachedProvider(provider);
    return 1;
}

HOOK(0x004B7550, AiStateFactory_HandleRequest17, "auto");
