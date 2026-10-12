#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(int, __fastcall, CoverAttack_SetStance_492720, 0x00492720, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, C_004D10F0_ResetFields, 0x004D10F0, (void* self, void*));
CSF_FUNC(int, __fastcall, GetLinkedObjectValue_4DB5C0, 0x004DB5C0, (void* self, void*));
CSF_FUNC(int, __fastcall, UnnamedApply_4DC180, 0x004DC180, (void* self, void* edx, int a2));

static int __stdcall AiMoveTasks_ResetCoverAttackState(int taskAddress)
{
    void* task = reinterpret_cast<void*>(taskAddress);

    int linkedObject = GetLinkedObjectValue_4DB5C0(task, nullptr);
    int objectAddress = linkedObject ? linkedObject - 4 : 0;
    void* coverAttack = CSF_AT(void*, reinterpret_cast<void*>(objectAddress), 0x2C);
    CoverAttack_SetStance_492720(coverAttack, nullptr, 3);

    linkedObject = GetLinkedObjectValue_4DB5C0(task, nullptr);
    objectAddress = linkedObject ? linkedObject - 4 : 0;
    coverAttack = CSF_AT(void*, reinterpret_cast<void*>(objectAddress), 0x2C);
    reinterpret_cast<void(__fastcall*)(void*, void*, int, int, int, int)>(
        CSF_AT(void**, coverAttack, 0)[0x8C / 4])(
            coverAttack, nullptr, 0, 0, -1, 0);

    linkedObject = GetLinkedObjectValue_4DB5C0(task, nullptr);
    objectAddress = linkedObject ? linkedObject - 4 : 0;
    C_004D10F0_ResetFields(
        reinterpret_cast<void*>(objectAddress + 1680), nullptr);

    linkedObject = GetLinkedObjectValue_4DB5C0(task, nullptr);
    objectAddress = linkedObject ? linkedObject - 4 : 0;
    return UnnamedApply_4DC180(task, nullptr, objectAddress + 572);
}

HOOK(0x004AB480, AiMoveTasks_ResetCoverAttackState, "auto");
