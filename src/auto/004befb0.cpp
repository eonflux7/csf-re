#include <cstdint>
#include "globals.h"
#include "hook.h"

// Callees, bound by address (signatures as given in the packet).
CSF_FUNC(int, __fastcall, GetLinkedObjectValue_4DB5C0, 0x004DB5C0, (void* self, void*));
CSF_FUNC(int, __fastcall, CoverAttack_SetState_4DC150, 0x004DC150, (void* self, void* edx, int a2));
CSF_FUNC(void*, __fastcall, GetCachedObjectProvider, 0x00686C10, (void* self, void* edx));
CSF_FUNC(int, __fastcall, sub_4AACE0, 0x004AACE0, (void* self, void* edx, int a3, char a4));
CSF_FUNC(int, __stdcall, sub_4BA3C0, 0x004BA3C0, (void* a1, char a2));

// __thiscall(this, aiState) with ret 4 -> __fastcall(self, edx, aiState).
// Selects the cover-attack state for the AI object and dispatches to the
// matching follow-up routine, depending on whether the linked object's
// slots 0x608/0x60C are both unset (-1).
static int __fastcall AiCore_DispatchCoverAttackState(void* self, void* /*edx*/, void* aiState)
{
    uint8_t* thisBytes = reinterpret_cast<uint8_t*>(self);

    int linkedValue = GetLinkedObjectValue_4DB5C0(aiState, nullptr);
    uintptr_t linkedBase = linkedValue ? static_cast<uintptr_t>(linkedValue) - 4 : 0;
    uint8_t* linkedObject = reinterpret_cast<uint8_t*>(linkedBase);

    if (CSF_AT(int, linkedObject, 0x608) == -1 && CSF_AT(int, linkedObject, 0x60C) == -1)
    {
        CoverAttack_SetState_4DC150(aiState, nullptr, static_cast<int>(reinterpret_cast<uintptr_t>(thisBytes + 8)));
        void* cachedObjectProvider = GetCachedObjectProvider(aiState, nullptr);
        return sub_4AACE0(thisBytes + 8, nullptr, reinterpret_cast<int>(cachedObjectProvider), static_cast<char>(1));
    }
    else
    {
        CoverAttack_SetState_4DC150(aiState, nullptr, static_cast<int>(reinterpret_cast<uintptr_t>(thisBytes + 4)));
        void* cachedObjectProvider = GetCachedObjectProvider(aiState, nullptr);
        return sub_4BA3C0(cachedObjectProvider, static_cast<char>(1));
    }
}

HOOK(0x004BEFB0, AiCore_DispatchCoverAttackState, "auto");
