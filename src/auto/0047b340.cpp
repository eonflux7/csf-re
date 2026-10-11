#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(char, __fastcall, AiAttackTasks_SetActionFlag, 0x0048C9D0,
         (void* self, void* edx, char a2));
CSF_FUNC(int, __fastcall, TaskCharAction_48CFE0, 0x0048CFE0,
         (void* self, void* edx, int a2, char a3, char a4));
CSF_FUNC(int, __fastcall, CharTask_StartMovement, 0x004DB140,
         (void* self, void* edx, int a2));

static int __fastcall C_0082f9f8_StartAttackMovement(
    C_0082f9f8* self, void* /*edx*/, int movementMode)
{
    void* owner = reinterpret_cast<void*>(self->field_0008);
    void* character = CSF_AT(void*, owner, 0x14);
    TaskCharAction_48CFE0(
        CSF_AT(void*, character, 0x204), nullptr,
        reinterpret_cast<int>(self), 1, -100);

    owner = reinterpret_cast<void*>(self->field_0008);
    character = CSF_AT(void*, owner, 0x14);
    AiAttackTasks_SetActionFlag(
        CSF_AT(void*, character, 0x204), nullptr, 1);

    owner = reinterpret_cast<void*>(self->field_0008);
    character = CSF_AT(void*, owner, 0x14);
    void* controller = CSF_AT(void*, character, 0x48);
    reinterpret_cast<void(__fastcall*)(void*, void*, uint32_t)>(
        CSF_AT(void**, controller, 0)[0x38 / 4])(
            controller, nullptr, self->field_0008);

    if (movementMode == 1)
    {
        owner = reinterpret_cast<void*>(self->field_0008);
        character = CSF_AT(void*, owner, 0x14);
        if (CSF_AT(int, character, 0x1C4) != 1)
        {
            return CharTask_StartMovement(
                self, nullptr,
                reinterpret_cast<int>(
                    reinterpret_cast<uint8_t*>(self) + 0x28));
        }

        return CharTask_StartMovement(
            self, nullptr,
            reinterpret_cast<int>(
                reinterpret_cast<uint8_t*>(self) + 0x78));
    }

    if (movementMode == 2)
    {
        return CharTask_StartMovement(
            self, nullptr,
            reinterpret_cast<int>(
                reinterpret_cast<uint8_t*>(self) + 0x78));
    }

    return CharTask_StartMovement(
        self, nullptr, reinterpret_cast<int>(&self->field_0014));
}

HOOK(0x0047B340, C_0082f9f8_StartAttackMovement, "auto");
