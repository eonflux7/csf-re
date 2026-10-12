#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, GetLinkedObjectField10, 0x004DB5D0, (void* self, void*));
CSF_FUNC(int, __fastcall, AiCoverAttackTasks_ResetLinkedTaskState, 0x004DC380, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, ResolveEntryLookupValue, 0x00471540, (void* self, void* edx));
CSF_FUNC(void*, __fastcall, GetCachedObjectProvider, 0x00686C10, (void* self, void* edx));
CSF_FUNC(int, __fastcall, AiCore_ActivateUnassignedCoverState, 0x004AA990, (void* self, void* edx, int a2, char a3));

// Resets the linked cover-attack task state on the object, resolves the cached
// provider for the entry lookup value, and activates the unassigned cover state
// on the linked object. The flag passed on is true when a3 is zero.
static int __stdcall AiStatic_ResetCoverAttackAndActivateUnassigned(C_0085e760* a1, int a2, int a3)
{
    (void)a2;

    int linkedField = GetLinkedObjectField10(a1, nullptr);
    AiCoverAttackTasks_ResetLinkedTaskState(a1, nullptr, linkedField + 8);

    int entryValue = ResolveEntryLookupValue(a1, nullptr);
    void* cachedProvider = GetCachedObjectProvider(
        reinterpret_cast<void*>(static_cast<uintptr_t>(entryValue)), nullptr);

    int linkedAgain = GetLinkedObjectField10(a1, nullptr);
    void* activateTarget = reinterpret_cast<void*>(static_cast<uintptr_t>(linkedAgain + 8));

    char isZeroFlag = (a3 == 0) ? 1 : 0;

    return AiCore_ActivateUnassignedCoverState(activateTarget, nullptr,
        static_cast<int>(reinterpret_cast<uintptr_t>(cachedProvider)), isZeroFlag);
}

HOOK(0x004AFC20, AiStatic_ResetCoverAttackAndActivateUnassigned, "auto");
