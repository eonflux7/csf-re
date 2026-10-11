#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(uint32_t*, __fastcall, AiAttackTasks_InitState, 0x0047AE70, (void* self, void* edx));
CSF_FUNC(int, __fastcall, AiAttackTasks_SetStateParameters, 0x0047AE90, (void* self, void* edx, int a2, int a3));
CSF_FUNC(uint32_t*, __fastcall, AiAttackTasks_InitTaskBase, 0x0047AF90, (void* self, void* edx, int a2));
CSF_FUNC(uint32_t*, __fastcall, AiAttackTasks_InitShootTask, 0x0047BDD0, (void* self, void* edx));
CSF_FUNC(uint32_t*, __fastcall, AiAttackTasks_InitAttackTask, 0x0047BE50, (void* self, void* edx));
CSF_FUNC(uint32_t*, __fastcall, AiAttackTasks_InitReloadTask, 0x0047C2C0, (void* self, void* edx));
CSF_FUNC(uint32_t*, __fastcall, AiAttackTasks_InitBurstAttackTask, 0x0047F4E0, (void* self, void* edx));
CSF_FUNC(uint32_t*, __fastcall, AiAttackTasks_InitAuxiliaryTask, 0x0048C810, (void* self, void* edx));
CSF_FUNC(uint32_t*, __fastcall, CharTask_InitBase, 0x004DB4C0, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetGlobalValue_8BC1E0, 0x00756A10, ());

static int __fastcall C_00831ac0_InitializeAttackTaskGroup(
    C_00831ac0* self, void* /*edx*/, int parameter)
{
    auto* bytes = reinterpret_cast<uint8_t*>(self);

    AiAttackTasks_InitTaskBase(self, nullptr, parameter);
    self->vtbl = CSF_VTBL(0x00833030);
    CSF_AT(void**, self, 0x04) = CSF_VTBL(0x00833010);

    CharTask_InitBase(bytes + 0x24, nullptr);
    CSF_AT(void**, self, 0x24) = CSF_VTBL(0x00831BA0);
    CSF_AT(void**, self, 0x28) = CSF_VTBL(0x00831B84);

    CharTask_InitBase(bytes + 0x38, nullptr);
    CSF_AT(void**, self, 0x38) = CSF_VTBL(0x00831C58);
    CSF_AT(void**, self, 0x3C) = CSF_VTBL(0x00831C38);

    CharTask_InitBase(bytes + 0x4C, nullptr);
    CSF_AT(void**, self, 0x4C) = CSF_VTBL(0x00831D18);
    CSF_AT(void**, self, 0x50) = CSF_VTBL(0x00831CFC);

    CharTask_InitBase(bytes + 0x60, nullptr);
    CSF_AT(void**, self, 0x60) = CSF_VTBL(0x00831DD0);
    CSF_AT(void**, self, 0x64) = CSF_VTBL(0x00831DB0);

    AiAttackTasks_InitReloadTask(bytes + 0x74, nullptr);

    CharTask_InitBase(bytes + 0x100, nullptr);
    CSF_AT(void**, self, 0x100) = CSF_VTBL(0x00832250);
    CSF_AT(void**, self, 0x104) = CSF_VTBL(0x00832230);

    AiAttackTasks_InitShootTask(bytes + 0x114, nullptr);
    AiAttackTasks_InitAttackTask(bytes + 0x164, nullptr);
    AiAttackTasks_InitBurstAttackTask(bytes + 0x1E0, nullptr);
    AiAttackTasks_InitAuxiliaryTask(bytes + 0x2B8, nullptr);
    AiAttackTasks_InitState(bytes + 0x2D8, nullptr);

    CSF_AT(uint8_t, self, 0x2A8) = 0;
    CSF_AT(uint32_t, self, 0x2AC) = 0;
    CSF_AT(uint32_t, self, 0x2B0) = 0;
    CSF_AT(uint32_t, self, 0x2B4) = 0;
    CSF_AT(uint32_t, self, 0x2E8) = 0;
    CSF_AT(uint8_t, self, 0x2EC) = 1;
    CSF_AT(uint8_t, self, 0x2ED) = 1;

    void* firstContext = reinterpret_cast<void*>(GetGlobalValue_8BC1E0());
    void* secondContext = reinterpret_cast<void*>(GetGlobalValue_8BC1E0());
    int secondParameter = CSF_AT(int, firstContext, 0xADC);
    int firstParameter = CSF_AT(int, secondContext, 0xAD8);
    AiAttackTasks_SetStateParameters(
        bytes + 0x2D8, nullptr, firstParameter, secondParameter);

    CSF_AT(uint32_t, self, 0x08) = reinterpret_cast<uint32_t>(self);
    return reinterpret_cast<int>(self);
}

HOOK(0x0047F640, C_00831ac0_InitializeAttackTaskGroup, "auto");
