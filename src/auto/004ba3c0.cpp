#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Callees, bound by address.
CSF_FUNC(int, __fastcall, GetLinkedObjectValue_4DB5C0, 0x004DB5C0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, UnnamedApply_4DC180, 0x004DC180, (void* self, void* edx, int a2));
CSF_FUNC(void*, __fastcall, GetCachedObjectProvider, 0x00686C10, (void* self, void* edx));
CSF_FUNC(int, __fastcall, CoverAttack_SelectState, 0x004B7E40, (void* self, void* edx, void* state, char alternate));

// Activates the cover state assigned to this object. Stdcall, 2 stack args, ret 8.
static int __stdcall AiCore_ActivateAssignedCoverState(C_0085e7b8* a1, char a2)
{
    // First linked-object lookup: slot base = linked - 4, or 0 when there is no link.
    int linkedBefore = GetLinkedObjectValue_4DB5C0(a1, nullptr);
    int slotBase = linkedBefore ? linkedBefore - 4 : 0;
    UnnamedApply_4DC180(a1, nullptr, slotBase + 0x53c);

    // Second lookup: the cover-state owner offset is recomputed the same way.
    int linkedAfter = GetLinkedObjectValue_4DB5C0(a1, nullptr);
    int coverOffset = linkedAfter ? linkedAfter - 4 : 0;

    // The original pushes a2 before this call; GetCachedObjectProvider pops nothing,
    // so a2 stays on the stack as the 'alternate' argument of CoverAttack_SelectState.
    void* provider = GetCachedObjectProvider(a1, nullptr);

    return CoverAttack_SelectState(
        reinterpret_cast<void*>(static_cast<uintptr_t>(coverOffset + 0x53c)),
        nullptr,
        provider,
        a2);
}

HOOK(0x004BA3C0, AiCore_ActivateAssignedCoverState, "auto");
