#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(int, __fastcall, CoverAttack_SetStance_492720, 0x00492720, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, GetLinkedObjectValue_4DB5C0, 0x004DB5C0, (void* self, void*));
CSF_FUNC(int, __fastcall, UnnamedApply_4DC180, 0x004DC180, (void* self, void* edx, int a2));

static int __stdcall AiMoveTasks_PrepareCoverAttack(void* task)
{
    uint32_t linkedValue = static_cast<uint32_t>(
        GetLinkedObjectValue_4DB5C0(task, nullptr));
    uint32_t objectAddress = linkedValue ? linkedValue - 4 : 0;
    CSF_AT(uint32_t, reinterpret_cast<void*>(objectAddress), 0x680) |= 1u;

    linkedValue = static_cast<uint32_t>(
        GetLinkedObjectValue_4DB5C0(task, nullptr));
    objectAddress = linkedValue ? linkedValue - 4 : 0;
    void* actor = CSF_AT(void*, reinterpret_cast<void*>(objectAddress), 0x2C);
    CoverAttack_SetStance_492720(actor, nullptr, 2);

    linkedValue = static_cast<uint32_t>(
        GetLinkedObjectValue_4DB5C0(task, nullptr));
    objectAddress = linkedValue ? linkedValue - 4 : 0;
    actor = CSF_AT(void*, reinterpret_cast<void*>(objectAddress), 0x2C);
    reinterpret_cast<void(__fastcall*)(void*, void*, int, int, int, int)>(
        CSF_AT(void**, actor, 0)[0x8C / 4])(
            actor, nullptr, 0, 0, -1, 0);

    linkedValue = static_cast<uint32_t>(
        GetLinkedObjectValue_4DB5C0(task, nullptr));
    objectAddress = linkedValue ? linkedValue - 4 : 0;
    return UnnamedApply_4DC180(
        task, nullptr, static_cast<int>(objectAddress + 0x404));
}

HOOK(0x004AB080, AiMoveTasks_PrepareCoverAttack, "auto");
