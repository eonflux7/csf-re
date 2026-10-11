#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, GetLinkedObjectField10, 0x004DB5D0, (void* self, void*));
CSF_FUNC(int, __fastcall, AiCoverAttackTasks_ResetLinkedTaskState, 0x004DC380, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, ResolveEntryLookupValue, 0x00471540, (void* self, void* edx));
CSF_FUNC(void*, __fastcall, GetCachedObjectProvider, 0x00686C10, (void* self, void* edx));
CSF_FUNC(int, __stdcall, AiCore_InitializeCoverAttackState, 0x004BEA10, (int aiAddress, int));

// Resets the linked cover-attack task state, resolves the cached provider for
// the entry lookup value, then initializes the cover-attack state with a flag
// that is 1 when the caller's third argument is zero. The second parameter is
// unused by the original.
static int __stdcall AiStateFactory_InitCoverAttackState(C_0085e760* a1, int a2, int a3)
{
    (void)a2;

    int linkedField = GetLinkedObjectField10(a1, nullptr);
    AiCoverAttackTasks_ResetLinkedTaskState(a1, nullptr, linkedField + 8);

    int isZeroFlag = (a3 == 0) ? 1 : 0;

    int entryValue = ResolveEntryLookupValue(a1, nullptr);
    void* cachedProvider = GetCachedObjectProvider(
        reinterpret_cast<void*>(static_cast<uintptr_t>(entryValue)), nullptr);

    // Result discarded by the original, but the call itself is performed.
    GetLinkedObjectField10(a1, nullptr);

    return AiCore_InitializeCoverAttackState(
        static_cast<int>(reinterpret_cast<uintptr_t>(cachedProvider)), isZeroFlag);
}

HOOK(0x004C38F0, AiStateFactory_InitCoverAttackState, "auto");
