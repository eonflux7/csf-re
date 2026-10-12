#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, ResolveEntryLookupValue, 0x00471540, (void* self, void* edx));
CSF_FUNC(int, __stdcall, DispatchCachedTask_4AC170, 0x004AC170, (int a1, int a2));
CSF_FUNC(int, __fastcall, GetLinkedObjectValue_4DB5C0, 0x004DB5C0, (void* self, void*));
CSF_FUNC(int, __fastcall, GetLinkedObjectField10, 0x004DB5D0, (void* self, void*));
CSF_FUNC(int, __fastcall, AiCoverAttackTasks_ResetLinkedTaskState, 0x004DC380, (void* self, void* edx, int a2));
CSF_FUNC(void*, __fastcall, GetCachedObjectProvider, 0x00686C10, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetGlobalValue_8BC1E0, 0x00756A10, ());

static int __stdcall C_0085e760_ResetTasksAndDispatch(C_0085e760* self, int a2, int a3)
{
    int linkedValue = GetLinkedObjectValue_4DB5C0(self, nullptr);
    int linkedBaseValue = linkedValue ? linkedValue - 4 : 0;
    void* linkedBase = reinterpret_cast<void*>(static_cast<uintptr_t>(linkedBaseValue));

    // Flag selects which global task slot is used below.
    uint8_t useAlternateSlot = CSF_AT(uint8_t, linkedBase, 1740);

    int field10 = GetLinkedObjectField10(self, nullptr);
    AiCoverAttackTasks_ResetLinkedTaskState(self, nullptr, field10 + 12);

    int globalsValue = GetGlobalValue_8BC1E0();
    void* globals = reinterpret_cast<void*>(static_cast<uintptr_t>(globalsValue));
    int taskValue = (useAlternateSlot != 0)
        ? CSF_AT(int, globals, 2716)
        : CSF_AT(int, globals, 2720);

    C_0085e7b8* entry = reinterpret_cast<C_0085e7b8*>(ResolveEntryLookupValue(self, nullptr));
    void* provider = GetCachedObjectProvider(entry, nullptr);
    int cachedProvider = static_cast<int>(reinterpret_cast<uintptr_t>(provider));

    GetLinkedObjectField10(self, nullptr);
    return DispatchCachedTask_4AC170(cachedProvider, taskValue);
}

HOOK(0x004B1160, C_0085e760_ResetTasksAndDispatch, "auto");
