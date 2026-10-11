#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, GrowButtonArray, 0x0041E5F0, (void* self, void* edx, int growth));
CSF_FUNC(int, __fastcall, CoverTask_SetMode_492720, 0x00492720, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, CoverTask_AppendTarget_4B7150, 0x004B7150, (void* self, void* edx, uint32_t* a2));
CSF_FUNC(int, __fastcall, CoverTask_Finish_4BC3A0, 0x004BC3A0, (void* self, void* edx, float a2, float a3));
CSF_FUNC(int, __fastcall, CoverTask_AppendPoint_4C5150, 0x004C5150, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, GetLinkedObjectValue_4DB5C0, 0x004DB5C0, (void* self, void*));
CSF_FUNC(int, __fastcall, CoverTask_Apply_4DC180, 0x004DC180, (void* self, void* edx, int a2));

static uint8_t* GetOwner(void* task)
{
    int linked = GetLinkedObjectValue_4DB5C0(task, nullptr);
    return linked ? reinterpret_cast<uint8_t*>(linked - 4) : nullptr;
}

static void PushCommand(S_0041e5f0* commands, uint32_t command)
{
    if (commands->field_0008 >= commands->field_0004)
        GrowButtonArray(commands, nullptr, static_cast<int>(commands->field_000c));

    uint32_t count = commands->field_0008;
    uint32_t* slot = reinterpret_cast<uint32_t*>(commands->field_0000 + 4 * count);
    commands->field_0008 = count + 1;
    *slot = command;
}

static int __stdcall CoverAttackTasks_QueueAttackFromPosition(int taskValue)
{
    void* task = reinterpret_cast<void*>(taskValue);

    uint8_t* owner = GetOwner(task);
    void* character = reinterpret_cast<void*>(CSF_AT(uint32_t, owner, 0x2C));
    CoverTask_SetMode_492720(character, nullptr, 2);

    owner = GetOwner(task);
    character = reinterpret_cast<void*>(CSF_AT(uint32_t, owner, 0x2C));
    ((void(__fastcall*)(void*, void*, int, int, int, int))
        CSF_AT(void**, character, 0)[0x8C / 4])(character, nullptr, 0, 0, -1, 0);

    owner = GetOwner(task);
    CoverTask_Apply_4DC180(task, nullptr,
        static_cast<int>(reinterpret_cast<uintptr_t>(owner) + 0x440));

    uint8_t* commandOwner = GetOwner(task);
    owner = GetOwner(task);
    uint8_t* selectedPoint = owner +
        (CSF_AT(int, owner, 0x630) < 0 ? 0x610 : 0x628);

    S_0041e5f0* commands;
    if (CSF_AT(int, selectedPoint, 8) < 0)
    {
        uint8_t* targetOwner = GetOwner(task);
        commands = reinterpret_cast<S_0041e5f0*>(commandOwner + 0x50C);
        PushCommand(commands, 4);
        CoverTask_AppendTarget_4B7150(commands, nullptr,
            reinterpret_cast<uint32_t*>(targetOwner + 0x64C));
    }
    else
    {
        uint8_t* pointOwner = GetOwner(task);
        commands = reinterpret_cast<S_0041e5f0*>(commandOwner + 0x50C);
        int pointOffset = CSF_AT(int, pointOwner, 0x630) < 0 ? 0x610 : 0x628;
        CoverTask_AppendPoint_4C5150(commands, nullptr,
            static_cast<int>(reinterpret_cast<uintptr_t>(pointOwner) + pointOffset));
    }

    PushCommand(commands, 31);
    return CoverTask_Finish_4BC3A0(commands, nullptr, 1.0f, 3.0f);
}

HOOK(0x004C61D0, CoverAttackTasks_QueueAttackFromPosition, "auto");
