#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, GetLinkedObjectField10, 0x004DB5D0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, AiCoverAttackTasks_ResetLinkedTaskState, 0x004DC380, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, ResolveEntryLookupValue, 0x00471540, (void* self, void* edx));
CSF_FUNC(void*, __fastcall, GetCachedObjectProvider, 0x00686C10, (void* self, void* edx));

// TOrden entry point: resets the task state linked to the order, re-resolves
// the entry lookup, touches its cached provider, then returns the linked field 10.
static int __stdcall TOrden_ResetLinkedTaskAndRefresh(C_0085e760* a1)
{
    int linkedField10 = GetLinkedObjectField10(a1, nullptr);
    AiCoverAttackTasks_ResetLinkedTaskState(a1, nullptr, linkedField10 + 4);

    C_0085e7b8* lookupEntry = reinterpret_cast<C_0085e7b8*>(ResolveEntryLookupValue(a1, nullptr));
    GetCachedObjectProvider(lookupEntry, nullptr);

    return GetLinkedObjectField10(a1, nullptr);
}

HOOK(0x4AD0C0, TOrden_ResetLinkedTaskAndRefresh, "auto");
