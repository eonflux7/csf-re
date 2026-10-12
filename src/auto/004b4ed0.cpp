#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(int, __fastcall, CoverAttack_SetStance_492720, 0x00492720, (void* self, void* edx, int a2));
CSF_FUNC(void, __fastcall, AiDistracted_UpdateLinkedState, 0x004B3430, (void* self, void* edx));
CSF_FUNC(int, __fastcall, GetLinkedObjectValue_4DB5C0, 0x004DB5C0, (void* self, void*));
CSF_FUNC(int, __fastcall, UnnamedApply_4DC180, 0x004DC180, (void* self, void* edx, int a2));

static int __stdcall AiAlert_ResetLinkedState(void* self)
{
    int linkedValue = GetLinkedObjectValue_4DB5C0(self, nullptr);
    void* linkedObject = linkedValue
        ? reinterpret_cast<void*>(linkedValue - 4) : nullptr;

    CoverAttack_SetStance_492720(
        CSF_AT(void*, linkedObject, 0x2C), nullptr, 3);

    linkedValue = GetLinkedObjectValue_4DB5C0(self, nullptr);
    linkedObject = linkedValue
        ? reinterpret_cast<void*>(linkedValue - 4) : nullptr;
    void* actor = CSF_AT(void*, linkedObject, 0x2C);
    reinterpret_cast<void (__fastcall*)(void*, void*, int, int, int, int)>(
        CSF_AT(void**, actor, 0)[0x8C / 4])(
            actor, nullptr, 0, 0, -1, 0);

    linkedValue = GetLinkedObjectValue_4DB5C0(self, nullptr);
    linkedObject = linkedValue
        ? reinterpret_cast<void*>(linkedValue - 4) : nullptr;
    AiDistracted_UpdateLinkedState(linkedObject, nullptr);

    linkedValue = GetLinkedObjectValue_4DB5C0(self, nullptr);
    return UnnamedApply_4DC180(
        self, nullptr, (linkedValue ? linkedValue - 4 : 0) + 572);
}

HOOK(0x004B4ED0, AiAlert_ResetLinkedState, "auto");
