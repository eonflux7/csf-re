#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(uint32_t*, __fastcall, AiAutonomous_ConstructTaskBase, 0x004D1660, (void* self, void* edx, int a2));
CSF_FUNC(uint32_t*, __fastcall, AiAttackTasks_InitAuxiliaryTask, 0x0048C810, (void* self, void* edx));
CSF_FUNC(uint32_t*, __fastcall, AttackTasks_ShiftPairAndClear, 0x0047AE70, (uint32_t* self, void* edx));
CSF_FUNC(int, __cdecl, GetGlobalValue_8BC1E0, 0x00756A10, ());
CSF_FUNC(int, __fastcall, AiAttackTasks_SetFirstTwoFieldsReturnFirst, 0x0047AE90, (void* self, void* edx, int firstValue, int secondValue));

static uint32_t* __fastcall C_0083b668_InitBaseTask(C_0083b668* self, void* /*edx*/, int a2)
{
    AiAutonomous_ConstructTaskBase(self, nullptr, a2);

    self->vtbl = CSF_VTBL(0x0083B668);
    CSF_AT(void*, self, 0x4) = CSF_VTBL(0x0083B648);

    AiAttackTasks_InitAuxiliaryTask(reinterpret_cast<uint8_t*>(self) + 0x40, nullptr);
    AttackTasks_ShiftPairAndClear(reinterpret_cast<uint32_t*>(reinterpret_cast<uint8_t*>(self) + 0x60), nullptr);

    int globalFirst = GetGlobalValue_8BC1E0();
    int globalSecond = GetGlobalValue_8BC1E0();

    int firstValue = CSF_AT(int, reinterpret_cast<void*>(static_cast<uintptr_t>(globalSecond)), 0xAE0);
    int secondValue = CSF_AT(int, reinterpret_cast<void*>(static_cast<uintptr_t>(globalFirst)), 0xAE4);

    AiAttackTasks_SetFirstTwoFieldsReturnFirst(
        reinterpret_cast<uint8_t*>(self) + 0x60, nullptr, firstValue, secondValue);

    return reinterpret_cast<uint32_t*>(self);
}

HOOK(0x004D0350, C_0083b668_InitBaseTask, "auto");
