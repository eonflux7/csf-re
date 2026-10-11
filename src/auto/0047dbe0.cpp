#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, Archive_SerializeFloatQuad, 0x004076D0, (void* self, void* , void* context));
CSF_FUNC(int, __fastcall, AiAttackTasks_SerializeBase, 0x0047B040, (void* self, void* edx, void* a2));
CSF_FUNC(int, __cdecl, TaskState_ResetSlot, 0x0047BCB0, (int a1, void* a2));
CSF_FUNC(int, __cdecl, Archive_SerializeString, 0x00496080, (int archiveAddress, S_00691860* string));

static bool AiAttackTasks_BeginArchiveField(void* archive)
{
    const bool ready = CSF_AT(uint8_t, archive, 0x00) == 0;
    CSF_AT(int, archive, 0x1C) = 0;
    if (ready)
    {
        CSF_AT(uint8_t, archive, 0x15) = 0;
        CSF_AT(uint8_t, archive, 0x16) = 0;
    }
    return ready;
}

static void AiAttackTasks_SerializeTaskSlot(void* slot, void* archive)
{
    if (AiAttackTasks_BeginArchiveField(archive))
        TaskState_ResetSlot(reinterpret_cast<int>(slot), archive);
}

static int __fastcall S_0041f0a0_SerializeAttackTasks(
    S_0041f0a0* self, void* /*edx*/, int archiveAddress)
{
    void* archive = reinterpret_cast<void*>(archiveAddress);
    auto* fields = reinterpret_cast<uint8_t*>(self);

    bool failed = false;
    if (CSF_AT(uint8_t, archive, 0x00) == 0)
    {
        void* stream = CSF_AT(void*, archive, 0x04);
        failed = !((bool(__fastcall*)(void*, void*, int, void*))
            CSF_AT(void**, stream, 0)[128 / 4])(stream, nullptr, 0, archive);
    }

    const int savedField = CSF_AT(int, archive, 0x1C);
    CSF_AT(uint8_t, archive, 0x00) = failed;
    AiAttackTasks_SerializeBase(self, nullptr, archive);

    int taskType = 0;
    if (CSF_AT(uint8_t, archive, 0x14) != 0)
    {
        void* task = reinterpret_cast<void*>(self->field_000c);
        if (task)
        {
            taskType = ((int(__fastcall*)(void*, void*))
                CSF_AT(void**, task, 0)[128 / 4])(task, nullptr);
        }
    }

    if (AiAttackTasks_BeginArchiveField(archive))
    {
        void* stream = CSF_AT(void*, archive, 0x04);
        CSF_AT(uint8_t, archive, 0x00) =
            ((int(__fastcall*)(void*, void*, void*, void*))
                CSF_AT(void**, stream, 0)[88 / 4])(
                    stream, nullptr, &taskType, archive) == -1;
    }

    if (AiAttackTasks_BeginArchiveField(archive))
    {
        void* stream = CSF_AT(void*, archive, 0x04);
        CSF_AT(uint8_t, archive, 0x00) =
            ((int(__fastcall*)(void*, void*, void*, void*))
                CSF_AT(void**, stream, 0)[88 / 4])(
                    stream, nullptr, fields + 0x2E4, archive) == -1;
    }

    if (AiAttackTasks_BeginArchiveField(archive))
    {
        void* stream = CSF_AT(void*, archive, 0x04);
        CSF_AT(uint8_t, archive, 0x00) =
            ((int(__fastcall*)(void*, void*, void*, void*))
                CSF_AT(void**, stream, 0)[68 / 4])(
                    stream, nullptr, fields + 0x2A4, archive) == -1;
    }

    if (AiAttackTasks_BeginArchiveField(archive))
    {
        void* stream = CSF_AT(void*, archive, 0x04);
        CSF_AT(uint8_t, archive, 0x00) =
            ((int(__fastcall*)(void*, void*, void*, void*))
                CSF_AT(void**, stream, 0)[84 / 4])(
                    stream, nullptr, fields + 0x2A8, archive) == -1;
    }

    if (AiAttackTasks_BeginArchiveField(archive))
    {
        void* stream = CSF_AT(void*, archive, 0x04);
        CSF_AT(uint8_t, archive, 0x00) =
            ((int(__fastcall*)(void*, void*, void*, void*))
                CSF_AT(void**, stream, 0)[84 / 4])(
                    stream, nullptr, fields + 0x2AC, archive) == -1;
    }

    if (AiAttackTasks_BeginArchiveField(archive))
    {
        void* stream = CSF_AT(void*, archive, 0x04);
        CSF_AT(uint8_t, archive, 0x00) =
            ((int(__fastcall*)(void*, void*, void*, void*))
                CSF_AT(void**, stream, 0)[84 / 4])(
                    stream, nullptr, fields + 0x2B0, archive) == -1;
    }

    if (AiAttackTasks_BeginArchiveField(archive))
    {
        Archive_SerializeString(
            reinterpret_cast<int>(fields + 0x2B4),
            reinterpret_cast<S_00691860*>(archive));
    }

    if (AiAttackTasks_BeginArchiveField(archive))
        Archive_SerializeFloatQuad(fields + 0x2D4, nullptr, archive);

    if (AiAttackTasks_BeginArchiveField(archive))
    {
        void* stream = CSF_AT(void*, archive, 0x04);
        CSF_AT(uint8_t, archive, 0x00) =
            ((int(__fastcall*)(void*, void*, void*, void*))
                CSF_AT(void**, stream, 0)[68 / 4])(
                    stream, nullptr, fields + 0x2E8, archive) == -1;
    }

    if (AiAttackTasks_BeginArchiveField(archive))
    {
        void* stream = CSF_AT(void*, archive, 0x04);
        CSF_AT(uint8_t, archive, 0x00) =
            ((int(__fastcall*)(void*, void*, void*, void*))
                CSF_AT(void**, stream, 0)[68 / 4])(
                    stream, nullptr, fields + 0x2E9, archive) == -1;
    }

    if (CSF_AT(int, archive, 0x18) > 50 || taskType == 0)
        AiAttackTasks_SerializeTaskSlot(fields + 0x20, archive);

    if (CSF_AT(int, archive, 0x18) > 50 || taskType == 4)
        AiAttackTasks_SerializeTaskSlot(fields + 0x160, archive);

    if (CSF_AT(int, archive, 0x18) > 50 || taskType == 5)
        AiAttackTasks_SerializeTaskSlot(fields + 0x1DC, archive);

    if (CSF_AT(int, archive, 0x18) > 50 || taskType == 3)
        AiAttackTasks_SerializeTaskSlot(fields + 0x110, archive);

    if (CSF_AT(int, archive, 0x18) > 50 || taskType == 2)
        AiAttackTasks_SerializeTaskSlot(fields + 0xFC, archive);

    if (CSF_AT(int, archive, 0x18) > 50 || taskType == 1)
        AiAttackTasks_SerializeTaskSlot(fields + 0x34, archive);

    AiAttackTasks_SerializeTaskSlot(fields + 0x48, archive);
    AiAttackTasks_SerializeTaskSlot(fields + 0x5C, archive);
    AiAttackTasks_SerializeTaskSlot(fields + 0x70, archive);

    CSF_AT(int, archive, 0x1C) = savedField;
    if (CSF_AT(uint8_t, archive, 0x00) != 0)
    {
        CSF_AT(uint8_t, archive, 0x00) = 1;
        return 1;
    }

    void* stream = CSF_AT(void*, archive, 0x04);
    const int result = ((int(__fastcall*)(void*, void*, void*))
        CSF_AT(void**, stream, 0)[124 / 4])(stream, nullptr, archive);

    if (static_cast<uint8_t>(result) == 0)
    {
        CSF_AT(uint8_t, archive, 0x00) = 1;
        return 1;
    }

    CSF_AT(uint8_t, archive, 0x00) = 0;
    return result;
}

HOOK(0x0047DBE0, S_0041f0a0_SerializeAttackTasks, "auto");
