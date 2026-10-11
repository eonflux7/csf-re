#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, AiAttackTasks_CopyTargetHandle, 0x0048C9F0, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, CharActionFsm_IsWeaponActionBlocked, 0x00492BD0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, Autonomo_GetMovementState, 0x004AEB90, (void* self, void* edx));
CSF_FUNC(char, __fastcall, Autonomo_PrepareMovement, 0x004D7670, (void* self, void* edx));
CSF_FUNC(int, __fastcall, CharTask_StartMovement, 0x004DB140, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, CharTask_LookupByOffset, 0x004DB440, (void* self, void* edx, int a2));

static char __fastcall C_00834fa8_UpdateAttackTask(C_00834fa8* self, void* /*edx*/, int /*a2*/)
{
    uint8_t* owner = reinterpret_cast<uint8_t*>(self->field_0008);
    void* actionFsm = CSF_AT(void*, owner, 0x14);

    if ((uint8_t)CharActionFsm_IsWeaponActionBlocked(actionFsm, nullptr) != 0) {
        return (char)CharTask_LookupByOffset(self, nullptr, (int)(self->field_000c + 0x10C));
    }

    void* subTask = CSF_AT(void*, self, 0x14);
    if (subTask != nullptr) {
        char done = ((char(__fastcall*)(void*, void*))CSF_AT(void**, subTask, 0)[15])(subTask, nullptr);
        if (!done) {
            uint8_t* o = reinterpret_cast<uint8_t*>(self->field_0008);
            CSF_AT(uint32_t, self, 0x14) = 0;
            void* inner = CSF_AT(void*, CSF_AT(void*, o, 0x14), 0x48);
            ((void(__fastcall*)(void*, void*))CSF_AT(void**, inner, 0)[23])(inner, nullptr);
            return (char)CharTask_LookupByOffset(self, nullptr, (int)(self->field_000c + 0x54));
        }
    }

    uint8_t* o = reinterpret_cast<uint8_t*>(self->field_0008);
    void* attackTasks = CSF_AT(void*, CSF_AT(void*, o, 0x14), 0x204);
    AiAttackTasks_CopyTargetHandle(attackTasks, nullptr, (int)(CSF_AT(uint32_t, o, 0x308) + 0x18));

    uint8_t* actor = reinterpret_cast<uint8_t*>(self->field_0008);
    char result = Autonomo_PrepareMovement(actor, nullptr);
    if (result) {
        int state = Autonomo_GetMovementState(CSF_AT(void*, actor, 0x30C), nullptr);
        result = (char)state;
        if (state != -1) {
            if (state != 0 || (result = CSF_AT(char, actor, 0x33C)) == 0) {
                result = (char)CharTask_StartMovement(actor, nullptr, (int)(actor + 0x164));
            }
        }
    }
    return result;
}

HOOK(0x004D8040, C_00834fa8_UpdateAttackTask, "auto");
