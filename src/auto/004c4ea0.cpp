#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_FUNC(int, __stdcall, AiCoverAttackTasks_UpdateCharacterTasks, 0x004DBD50, (int a1, int a2));
CSF_FUNC(int, __fastcall, IsField10Null, 0x004DB5B0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, GetLinkedObjectValue_4DB5C0, 0x004DB5C0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, CharActionFsm_IsWeaponActionBlocked, 0x00492BD0, (void* self, void* edx));
CSF_FUNC(void, __fastcall, TDudoso_UpdateCoverPlan, 0x004C4B80, (void* self, void* edx, int a2));
CSF_FUNC(char, __fastcall, TDudoso_UpdateTransition, 0x004BA170, (TDudoso* , void* , int actorValue));

// Returns linked object value - 4, or 0 when the object is unlinked.
static std::uintptr_t ResolveLinkedObjectBase(void* self)
{
    int linkedValue = GetLinkedObjectValue_4DB5C0(self, nullptr);
    return linkedValue ? static_cast<std::uintptr_t>(linkedValue - 4) : 0;
}

// Thiscall method: ecx = TDudoso instance (self), two stack args (a1 = actor, a2 = parameter).
static char __fastcall TDudoso_HandleGoToCover(TDudoso* self, void* /*edx*/, int a1, int a2)
{
    void* actor = reinterpret_cast<void*>(static_cast<std::uintptr_t>(a1));

    AiCoverAttackTasks_UpdateCharacterTasks(a1, a2);

    char result = static_cast<char>(IsField10Null(actor, nullptr));
    if (!result)
    {
        std::uintptr_t ownerBase = ResolveLinkedObjectBase(actor);
        void* weaponOwner = CSF_AT(void*, reinterpret_cast<void*>(ownerBase), 0x2C);
        result = static_cast<char>(CharActionFsm_IsWeaponActionBlocked(weaponOwner, nullptr));
        if (!result)
        {
            std::uintptr_t linkedBase = ResolveLinkedObjectBase(actor);
            TDudoso_UpdateCoverPlan(reinterpret_cast<void*>(linkedBase + 0x5DC), nullptr, a2);
            return TDudoso_UpdateTransition(self, nullptr, a1);
        }
    }
    return result;
}

HOOK(0x004C4EA0, TDudoso_HandleGoToCover, "auto");
