#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, GetLinkedObjectField10, 0x004DB5D0, (void* self, void*));
CSF_FUNC(int, __fastcall, AiCoverAttackTasks_ResetLinkedTaskState, 0x004DC380, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, ResolveEntryLookupValue, 0x00471540, (void* self, void* edx));
CSF_FUNC(void*, __fastcall, GetCachedObjectProvider, 0x00686C10, (void* self, void* edx));
CSF_FUNC(int, __fastcall, AiStatic_DispatchAction, 0x004AACE0, (void* self, void* edx, int a3, char a4));

// Resets the linked cover-attack task state, resolves the cached provider for the
// entry lookup value, then dispatches the static AI action on the linked object
// (linked + 8). The action flag is true when the third argument is zero.
// The second parameter is unused by the original.
static int __stdcall AiStatic_ResetAndDispatchAction(C_0085e760* a1, int a2, int a3)
{
    (void)a2;

    int linkedField = GetLinkedObjectField10(a1, nullptr);
    AiCoverAttackTasks_ResetLinkedTaskState(a1, nullptr, linkedField + 8);

    int entryValue = ResolveEntryLookupValue(a1, nullptr);
    void* cachedProvider = GetCachedObjectProvider(
        reinterpret_cast<void*>(static_cast<uintptr_t>(entryValue)), nullptr);

    char isZeroFlag = (a3 == 0) ? 1 : 0;

    int linkedAgain = GetLinkedObjectField10(a1, nullptr);
    void* dispatchTarget = reinterpret_cast<void*>(static_cast<uintptr_t>(linkedAgain + 8));

    return AiStatic_DispatchAction(dispatchTarget, nullptr,
        static_cast<int>(reinterpret_cast<uintptr_t>(cachedProvider)), isZeroFlag);
}

HOOK(0x004AFDC0, AiStatic_ResetAndDispatchAction, "auto");
