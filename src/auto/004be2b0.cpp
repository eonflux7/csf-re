#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, AiCore_DestroyTrackingTask, 0x004AE700, (void* self, void* edx));
CSF_FUNC(int, __fastcall, AiCore_DestroyCoverAttackTask, 0x004AE760, (void* self, void* edx));
CSF_FUNC(int, __fastcall, AiCore_DestroyEmbeddedTask, 0x004AEA30, (void* self, void* edx));
CSF_FUNC(int, __fastcall, AiCore_DestroyAlertTask, 0x004B2AB0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, AiCore_DestroyOrderTask, 0x004B2CF0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, AiCore_DestroyCombatTask, 0x004B9560, (void* self, void* edx));

using AiCoreDestructorEntry = int (__stdcall*)(char);
CSF_FUNC(AiCoreDestructorEntry*, __fastcall, AiCore_DestroyTrailingTask, 0x004B9660, (uint32_t* self, void* edx));

CSF_FUNC(void, __fastcall, C_0082d410_SetVtable, 0x004DBD40, (C_0082d410* self, void*));
CSF_FUNC(int, __fastcall, AiCore_DestroyBaseAgent, 0x004DBDB0, (void* self, void* edx));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

static int __fastcall C_0083aa80_Destroy(C_0083aa80* self, void* /*edx*/)
{
    auto* bytes = reinterpret_cast<uint8_t*>(self);

    self->vtbl = CSF_VTBL(0x0083AA80);
    self->field_0004 = reinterpret_cast<uint32_t>(CSF_VTBL(0x0083A9F8));

    if (self->field_070c >= 0x10 && self->field_06f8)
        Mem_FreeDebug(reinterpret_cast<void*>(self->field_06f8));

    self->field_070c = 15;
    self->field_0708 = 0;
    *reinterpret_cast<uint8_t*>(&self->field_06f8) = 0;

    CSF_AT(void*, self, 0x6D0) = CSF_VTBL(0x0082D410);
    AiCore_DestroyTrailingTask(reinterpret_cast<uint32_t*>(bytes + 0x5DC), nullptr);
    C_0082d410_SetVtable(reinterpret_cast<C_0082d410*>(bytes + 0x5CC), nullptr);
    C_0082d410_SetVtable(reinterpret_cast<C_0082d410*>(bytes + 0x5C8), nullptr);
    C_0082d410_SetVtable(reinterpret_cast<C_0082d410*>(bytes + 0x5C4), nullptr);
    C_0082d410_SetVtable(reinterpret_cast<C_0082d410*>(bytes + 0x5C0), nullptr);

    if (CSF_AT(uint32_t, self, 0x5A8))
    {
        Mem_Free(reinterpret_cast<void*>(CSF_AT(uint32_t, self, 0x5A8)));
        CSF_AT(uint32_t, self, 0x5A8) = 0;
    }

    C_0082d410_SetVtable(reinterpret_cast<C_0082d410*>(bytes + 0x5A4), nullptr);
    C_0082d410_SetVtable(reinterpret_cast<C_0082d410*>(bytes + 0x5A0), nullptr);
    CSF_AT(void*, self, 0x588) = CSF_VTBL(0x0082D410);
    C_0082d410_SetVtable(reinterpret_cast<C_0082d410*>(bytes + 0x584), nullptr);
    C_0082d410_SetVtable(reinterpret_cast<C_0082d410*>(bytes + 0x580), nullptr);
    C_0082d410_SetVtable(reinterpret_cast<C_0082d410*>(bytes + 0x57C), nullptr);
    C_0082d410_SetVtable(reinterpret_cast<C_0082d410*>(bytes + 0x578), nullptr);
    C_0082d410_SetVtable(reinterpret_cast<C_0082d410*>(bytes + 0x574), nullptr);
    C_0082d410_SetVtable(reinterpret_cast<C_0082d410*>(bytes + 0x570), nullptr);
    C_0082d410_SetVtable(reinterpret_cast<C_0082d410*>(bytes + 0x56C), nullptr);
    C_0082d410_SetVtable(reinterpret_cast<C_0082d410*>(bytes + 0x568), nullptr);
    C_0082d410_SetVtable(reinterpret_cast<C_0082d410*>(bytes + 0x564), nullptr);
    C_0082d410_SetVtable(reinterpret_cast<C_0082d410*>(bytes + 0x560), nullptr);
    C_0082d410_SetVtable(reinterpret_cast<C_0082d410*>(bytes + 0x55C), nullptr);

    if (CSF_AT(uint32_t, self, 0x544))
    {
        Mem_Free(reinterpret_cast<void*>(CSF_AT(uint32_t, self, 0x544)));
        CSF_AT(uint32_t, self, 0x544) = 0;
    }

    C_0082d410_SetVtable(reinterpret_cast<C_0082d410*>(bytes + 0x540), nullptr);
    C_0082d410_SetVtable(reinterpret_cast<C_0082d410*>(bytes + 0x53C), nullptr);
    AiCore_DestroyEmbeddedTask(bytes + 0x50C, nullptr);
    AiCore_DestroyOrderTask(bytes + 0x448, nullptr);
    C_0082d410_SetVtable(reinterpret_cast<C_0082d410*>(bytes + 0x444), nullptr);
    C_0082d410_SetVtable(reinterpret_cast<C_0082d410*>(bytes + 0x440), nullptr);
    AiCore_DestroyCoverAttackTask(bytes + 0x418, nullptr);
    C_0082d410_SetVtable(reinterpret_cast<C_0082d410*>(bytes + 0x414), nullptr);
    C_0082d410_SetVtable(reinterpret_cast<C_0082d410*>(bytes + 0x410), nullptr);
    C_0082d410_SetVtable(reinterpret_cast<C_0082d410*>(bytes + 0x40C), nullptr);
    C_0082d410_SetVtable(reinterpret_cast<C_0082d410*>(bytes + 0x408), nullptr);
    C_0082d410_SetVtable(reinterpret_cast<C_0082d410*>(bytes + 0x404), nullptr);

    if (self->field_03e4)
    {
        Mem_Free(reinterpret_cast<void*>(self->field_03e4));
        self->field_03e4 = 0;
    }

    C_0082d410_SetVtable(reinterpret_cast<C_0082d410*>(bytes + 0x3E0), nullptr);
    C_0082d410_SetVtable(reinterpret_cast<C_0082d410*>(bytes + 0x3DC), nullptr);
    C_0082d410_SetVtable(reinterpret_cast<C_0082d410*>(bytes + 0x3D8), nullptr);
    C_0082d410_SetVtable(reinterpret_cast<C_0082d410*>(bytes + 0x3B4), nullptr);
    CSF_AT(void*, self, 0x39C) = CSF_VTBL(0x0082D410);
    C_0082d410_SetVtable(reinterpret_cast<C_0082d410*>(bytes + 0x388), nullptr);
    C_0082d410_SetVtable(reinterpret_cast<C_0082d410*>(bytes + 0x364), nullptr);

    if (self->field_0340)
    {
        Mem_Free(reinterpret_cast<void*>(self->field_0340));
        self->field_0340 = 0;
    }

    C_0082d410_SetVtable(reinterpret_cast<C_0082d410*>(bytes + 0x32C), nullptr);
    C_0082d410_SetVtable(reinterpret_cast<C_0082d410*>(bytes + 0x310), nullptr);
    C_0082d410_SetVtable(reinterpret_cast<C_0082d410*>(bytes + 0x2F4), nullptr);
    CSF_AT(void*, self, 0x2E8) = CSF_VTBL(0x0082D410);
    C_0082d410_SetVtable(reinterpret_cast<C_0082d410*>(bytes + 0x2D4), nullptr);

    void* ownedObject = reinterpret_cast<void*>(self->field_02c4);
    if (ownedObject)
    {
        if (CSF_AT(uint32_t, reinterpret_cast<uint8_t*>(ownedObject) - 4, 0))
        {
            ((void (__fastcall*)(void*, void*, int))
                CSF_AT(void**, ownedObject, 0)[0])(ownedObject, nullptr, 3);
        }
        else
        {
            Mem_Free(reinterpret_cast<uint8_t*>(ownedObject) - 4);
        }
        self->field_02c4 = 0;
    }

    C_0082d410_SetVtable(reinterpret_cast<C_0082d410*>(bytes + 0x2A4), nullptr);

    if (self->field_0280)
    {
        Mem_Free(reinterpret_cast<void*>(self->field_0280));
        self->field_0280 = 0;
    }

    C_0082d410_SetVtable(reinterpret_cast<C_0082d410*>(bytes + 0x258), nullptr);
    C_0082d410_SetVtable(reinterpret_cast<C_0082d410*>(bytes + 0x248), nullptr);
    C_0082d410_SetVtable(reinterpret_cast<C_0082d410*>(bytes + 0x244), nullptr);
    C_0082d410_SetVtable(reinterpret_cast<C_0082d410*>(bytes + 0x240), nullptr);
    C_0082d410_SetVtable(reinterpret_cast<C_0082d410*>(bytes + 0x23C), nullptr);
    C_0082d410_SetVtable(reinterpret_cast<C_0082d410*>(bytes + 0x228), nullptr);
    C_0082d410_SetVtable(reinterpret_cast<C_0082d410*>(bytes + 0x224), nullptr);
    C_0082d410_SetVtable(reinterpret_cast<C_0082d410*>(bytes + 0x220), nullptr);
    C_0082d410_SetVtable(reinterpret_cast<C_0082d410*>(bytes + 0x21C), nullptr);
    AiCore_DestroyTrackingTask(bytes + 0x1C8, nullptr);
    C_0082d410_SetVtable(reinterpret_cast<C_0082d410*>(bytes + 0x1B0), nullptr);
    C_0082d410_SetVtable(reinterpret_cast<C_0082d410*>(bytes + 0x1AC), nullptr);
    C_0082d410_SetVtable(reinterpret_cast<C_0082d410*>(bytes + 0x1A4), nullptr);
    C_0082d410_SetVtable(reinterpret_cast<C_0082d410*>(bytes + 0x1A0), nullptr);
    C_0082d410_SetVtable(reinterpret_cast<C_0082d410*>(bytes + 0x19C), nullptr);
    AiCore_DestroyCombatTask(bytes + 0x10C, nullptr);
    AiCore_DestroyAlertTask(bytes + 0x84, nullptr);
    C_0082d410_SetVtable(reinterpret_cast<C_0082d410*>(bytes + 0x70), nullptr);
    C_0082d410_SetVtable(reinterpret_cast<C_0082d410*>(bytes + 0x6C), nullptr);
    C_0082d410_SetVtable(reinterpret_cast<C_0082d410*>(bytes + 0x68), nullptr);
    C_0082d410_SetVtable(reinterpret_cast<C_0082d410*>(bytes + 0x64), nullptr);
    C_0082d410_SetVtable(reinterpret_cast<C_0082d410*>(bytes + 0x5C), nullptr);
    C_0082d410_SetVtable(reinterpret_cast<C_0082d410*>(bytes + 0x58), nullptr);
    C_0082d410_SetVtable(reinterpret_cast<C_0082d410*>(bytes + 0x54), nullptr);
    C_0082d410_SetVtable(reinterpret_cast<C_0082d410*>(bytes + 0x50), nullptr);
    C_0082d410_SetVtable(reinterpret_cast<C_0082d410*>(bytes + 0x4C), nullptr);
    C_0082d410_SetVtable(reinterpret_cast<C_0082d410*>(bytes + 0x48), nullptr);
    C_0082d410_SetVtable(reinterpret_cast<C_0082d410*>(bytes + 0x44), nullptr);
    C_0082d410_SetVtable(reinterpret_cast<C_0082d410*>(bytes + 0x40), nullptr);
    C_0082d410_SetVtable(reinterpret_cast<C_0082d410*>(bytes + 0x3C), nullptr);
    C_0082d410_SetVtable(reinterpret_cast<C_0082d410*>(bytes + 0x38), nullptr);
    C_0082d410_SetVtable(reinterpret_cast<C_0082d410*>(bytes + 0x34), nullptr);
    C_0082d410_SetVtable(reinterpret_cast<C_0082d410*>(bytes + 0x30), nullptr);

    self->vtbl = CSF_VTBL(0x00835398);
    self->field_0004 = reinterpret_cast<uint32_t>(CSF_VTBL(0x00835310));
    return AiCore_DestroyBaseAgent(self, nullptr);
}

HOOK(0x004BE2B0, C_0083aa80_Destroy, "auto");
