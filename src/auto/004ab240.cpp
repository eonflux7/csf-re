#include <cstdint>
#include "globals.h"
#include "hook.h"

CSF_FUNC(int, __fastcall, GetLinkedObjectValue_4DB5C0, 0x004DB5C0, (void* self, void*));
CSF_FUNC(int, __fastcall, IsField1BCNonNegative, 0x00492B10, (void* self, void*));
CSF_FUNC(int, __fastcall, GetLinkedObjectField10, 0x004DB5D0, (void* self, void*));
CSF_FUNC(int, __fastcall, AiCoverAttackTasks_ResetLinkedTaskState, 0x004DC380, (void* self, void* edx, int a2));

static int __stdcall AiMoveTasks_ResetLinkedStateBySignOfValue(int linkedSelf, int /*unused a2*/, int /*unused a3*/)
{
    void* self = reinterpret_cast<void*>(linkedSelf);

    int linkedValue = GetLinkedObjectValue_4DB5C0(self, nullptr);
    int valueBase = linkedValue ? linkedValue - 4 : 0;
    uint8_t* base = reinterpret_cast<uint8_t*>(valueBase);

    // The dword at base+0x2C is passed as `this` to IsField1BCNonNegative.
    int fieldValue = CSF_AT(int, base, 0x2C);
    bool nonNegative = (uint8_t)IsField1BCNonNegative(reinterpret_cast<void*>(fieldValue), nullptr) != 0;

    if (nonNegative)
    {
        int field10 = GetLinkedObjectField10(self, nullptr);
        return AiCoverAttackTasks_ResetLinkedTaskState(self, nullptr, field10 + 16);
    }
    else
    {
        int field10 = GetLinkedObjectField10(self, nullptr);
        return AiCoverAttackTasks_ResetLinkedTaskState(self, nullptr, field10 + 8);
    }
}

HOOK(0x004AB240, AiMoveTasks_ResetLinkedStateBySignOfValue, "auto");
