#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, S_0041e5f0_GrowArrayCapacity, 0x0041E5F0, (S_0041e5f0* self, void* , int additionalCapacity));
CSF_FUNC(int, __fastcall, CoverAttack_SetStance_492720, 0x00492720, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, UnnamedSetRangeParams_4BC3A0, 0x004BC3A0, (void* self, void* edx, float a2, float a3));
CSF_FUNC(int, __fastcall, GetLinkedObjectValue_4DB5C0, 0x004DB5C0, (void* self, void*));
CSF_FUNC(int, __fastcall, UnnamedApply_4DC180, 0x004DC180, (void* self, void* edx, int a2));

[[maybe_unused]] static const auto g_ArrayCapacityVtable = CSF_VTBL(0x0041E5F0);

static uint32_t __stdcall AiCore_InitializeCoverAttackSequence(int ownerAddress)
{
    void* owner = reinterpret_cast<void*>(ownerAddress);

    uint32_t linkedAddress = GetLinkedObjectValue_4DB5C0(owner, nullptr);
    uint32_t objectAddress = linkedAddress ? linkedAddress - 4 : 0;
    void* actor = CSF_AT(void*, reinterpret_cast<void*>(objectAddress), 0x2C);
    CoverAttack_SetStance_492720(actor, nullptr, 3);

    linkedAddress = GetLinkedObjectValue_4DB5C0(owner, nullptr);
    objectAddress = linkedAddress ? linkedAddress - 4 : 0;
    actor = CSF_AT(void*, reinterpret_cast<void*>(objectAddress), 0x2C);
    ((void(__fastcall*)(void*, void*, int, int, int, int))
        CSF_AT(void**, actor, 0)[0x8C / 4])(actor, nullptr, 0, 0, -1, 0);

    linkedAddress = GetLinkedObjectValue_4DB5C0(owner, nullptr);
    objectAddress = linkedAddress ? linkedAddress - 4 : 0;
    UnnamedApply_4DC180(owner, nullptr, static_cast<int>(objectAddress + 0x440));

    linkedAddress = GetLinkedObjectValue_4DB5C0(owner, nullptr);
    objectAddress = linkedAddress ? linkedAddress - 4 : 0;
    S_0041e5f0* commands =
        reinterpret_cast<S_0041e5f0*>(objectAddress + 0x50C);

    if (static_cast<uint32_t>(commands->field_0008) >=
        static_cast<uint32_t>(commands->field_0004))
    {
        S_0041e5f0_GrowArrayCapacity(commands, nullptr, commands->field_000c);
    }

    uint32_t count = commands->field_0008;
    uint32_t* destination =
        reinterpret_cast<uint32_t*>(commands->field_0000 + 4 * count);
    commands->field_0008 = count + 1;
    *destination = 31;

    if (static_cast<uint32_t>(commands->field_0008) >=
        static_cast<uint32_t>(commands->field_0004))
    {
        S_0041e5f0_GrowArrayCapacity(commands, nullptr, commands->field_000c);
    }

    count = commands->field_0008;
    destination = reinterpret_cast<uint32_t*>(commands->field_0000 + 4 * count);
    commands->field_0008 = count + 1;
    *destination = 13;

    UnnamedSetRangeParams_4BC3A0(commands, nullptr, 1.0f, 3.0f);

    if (static_cast<uint32_t>(commands->field_0008) >=
        static_cast<uint32_t>(commands->field_0004))
    {
        S_0041e5f0_GrowArrayCapacity(commands, nullptr, commands->field_000c);
    }

    count = commands->field_0008;
    destination = reinterpret_cast<uint32_t*>(commands->field_0000 + 4 * count);
    uint32_t result = count + 1;
    commands->field_0008 = result;
    *destination = 14;
    return result;
}

HOOK(0x004BF630, AiCore_InitializeCoverAttackSequence, "auto");
