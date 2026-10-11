#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, AiAttackTasks_CopyTargetHandle, 0x0048C9F0, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, SceneObj_IsInactive, 0x0048E100, (void* self, void* edx));
CSF_FUNC(int, __fastcall, SceneObject_PassesFirstCheck, 0x0048E120, (void* self, void* edx));
CSF_FUNC(int, __fastcall, CharActionFsm_IsWeaponActionBlocked, 0x00492BD0, (void* self, void* edx));
CSF_FUNC(char*, __fastcall, Task_GetTargetNameBuffer, 0x004D1510, (void* self, void* edx));
CSF_FUNC(int, __fastcall, ResetState, 0x004D16F0, (void* self, void*));
CSF_FUNC(int, __fastcall, AiCore_UpdateTargetData, 0x004D1730, (void* self, void* edx, int a2));
CSF_FUNC(char, __fastcall, AiAutonomous_FinishTask, 0x004D7730, (void* self, void* edx));
CSF_FUNC(int, __fastcall, CharTask_LookupByOffset, 0x004DB440, (void* self, void* edx, int a2));

static char __fastcall AiAutonomous_PrepareTargetAndFinish(C_00834fa8* self, void* /*edx*/, int a2)
{
    (void)a2;
    uint8_t* owner = reinterpret_cast<uint8_t*>(self->field_0008);
    void* actor = CSF_AT(void*, owner, 0x14);

    if (CharActionFsm_IsWeaponActionBlocked(actor, nullptr)) {
        return (char)CharTask_LookupByOffset(self, nullptr, (int)(self->field_000c + 0x10c));
    }

    owner = reinterpret_cast<uint8_t*>(self->field_0008);
    actor = CSF_AT(void*, owner, 0x14);
    char result = (char)SceneObject_PassesFirstCheck(actor, nullptr);
    if (result) {
        return result;
    }

    owner = reinterpret_cast<uint8_t*>(self->field_0008);
    actor = CSF_AT(void*, owner, 0x14);
    result = (char)SceneObj_IsInactive(actor, nullptr);
    if (result) {
        return result;
    }

    owner = reinterpret_cast<uint8_t*>(self->field_0008);
    actor = CSF_AT(void*, owner, 0x14);
    void* sub = CSF_AT(void*, actor, 0x48);
    void** subVtbl = CSF_AT(void**, sub, 0);
    if (((char(__fastcall*)(void*, void*))subVtbl[0x34 / 4])(sub, nullptr)) {
        owner = reinterpret_cast<uint8_t*>(self->field_0008);
        actor = CSF_AT(void*, owner, 0x14);
        void** actorVtbl = CSF_AT(void**, actor, 0);
        ((void(__fastcall*)(void*, void*, int, int, int, int))actorVtbl[0x8c / 4])(actor, nullptr, 0, 0, -1, 0);
    }

    owner = reinterpret_cast<uint8_t*>(self->field_0008);
    if (*Task_GetTargetNameBuffer(owner, nullptr) != 0) {
        owner = reinterpret_cast<uint8_t*>(self->field_0008);
        uint8_t* data = CSF_AT(uint8_t*, owner, 0x308);
        if (CSF_AT(int, data, 0x20) < 0) {
            char* buf = Task_GetTargetNameBuffer(owner, nullptr);
            ResetState(buf, nullptr);
        } else {
            int handle = (int)(CSF_AT(uint8_t*, owner, 0x308) + 0x18);
            char* buf = Task_GetTargetNameBuffer(owner, nullptr);
            AiCore_UpdateTargetData(buf, nullptr, handle);
        }
    }

    owner = reinterpret_cast<uint8_t*>(self->field_0008);
    uint8_t* data = CSF_AT(uint8_t*, owner, 0x308);
    uint8_t* act = CSF_AT(uint8_t*, owner, 0x14);
    AiAttackTasks_CopyTargetHandle(CSF_AT(void*, act, 0x204), nullptr, (int)(data + 0x18));

    owner = reinterpret_cast<uint8_t*>(self->field_0008);
    return AiAutonomous_FinishTask(owner, nullptr);
}

HOOK(0x004D7C20, AiAutonomous_PrepareTargetAndFinish, "auto");
