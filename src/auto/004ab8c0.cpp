#include <cstdint>
#include "globals.h"
#include "hook.h"

CSF_FUNC(int, __fastcall, CoverAttack_SetStance_492720, 0x00492720, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, GetLinkedObjectValue_4DB5C0, 0x004DB5C0, (void* self, void* edx));

// Resolves the linked object at a1 (object minus 4, or null when none), applies
// cover-attack stance 3 to its member at +0x2C, then invokes that member's vtable
// slot 35 with (0, 0, -1, 0) and returns its result.
static int __stdcall AiMoveTasks_SetCoverAttackStanceAndInvoke(int a1)
{
    int linked = GetLinkedObjectValue_4DB5C0(reinterpret_cast<void*>(static_cast<uintptr_t>(a1)), nullptr);
    int base = linked ? linked - 4 : 0;
    void* stanceOwner = CSF_AT(void*, reinterpret_cast<void*>(static_cast<uintptr_t>(base)), 0x2C);
    CoverAttack_SetStance_492720(stanceOwner, nullptr, 3);

    linked = GetLinkedObjectValue_4DB5C0(reinterpret_cast<void*>(static_cast<uintptr_t>(a1)), nullptr);
    base = linked ? linked - 4 : 0;
    void* target = CSF_AT(void*, reinterpret_cast<void*>(static_cast<uintptr_t>(base)), 0x2C);
    return ((int(__fastcall*)(void*, void*, int, int, int))(CSF_AT(void**, target, 0)[0x8C / 4]))(
        target, nullptr, 0, -1, 0);
}

HOOK(0x4AB8C0, AiMoveTasks_SetCoverAttackStanceAndInvoke, "auto");
