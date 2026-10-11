#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, AiAttackTasks_ResetSlot, 0x0047BCB0, (int owner, S_00691860* slot));
CSF_FUNC(int, __fastcall, Archive_SerializeBaseFields, 0x004DB4F0, (void* self, void* edx, int a2));

static bool AiAutonomous_BeginArchiveField(S_0041f0a0* archive)
{
    const bool ready = CSF_AT(uint8_t, archive, 0x00) == 0;
    CSF_AT(uint32_t, archive, 0x1C) = 0;
    if (ready)
    {
        CSF_AT(uint8_t, archive, 0x15) = 0;
        CSF_AT(uint8_t, archive, 0x16) = 0;
    }
    return ready;
}

static int __fastcall S_0041f0a0_SerializeAutonomousTask(
    S_0041f0a0* self, void* /*edx*/, S_0041f0a0* archive)
{
    const uint32_t savedContext = CSF_AT(uint32_t, archive, 0x1C);
    bool failed = false;
    if (CSF_AT(uint8_t, archive, 0x00) == 0)
    {
        void* serializer = CSF_AT(void*, archive, 0x04);
        failed = !reinterpret_cast<bool(__fastcall*)(
            void*, void*, int, S_0041f0a0*)>(
                CSF_AT(void**, serializer, 0)[0x80 / 4])(
                    serializer, nullptr, 0, archive);
    }
    CSF_AT(uint8_t, archive, 0x00) = failed;

    Archive_SerializeBaseFields(self, nullptr, reinterpret_cast<int>(archive));

    if (AiAutonomous_BeginArchiveField(archive))
    {
        void* serializer = CSF_AT(void*, archive, 0x04);
        CSF_AT(uint8_t, archive, 0x00) =
            reinterpret_cast<int(__fastcall*)(
                void*, void*, uint8_t*, S_0041f0a0*)>(
                    CSF_AT(void**, serializer, 0)[0x44 / 4])(
                        serializer, nullptr,
                        reinterpret_cast<uint8_t*>(self) + 0x60, archive) == -1;
    }

    if (AiAutonomous_BeginArchiveField(archive))
    {
        void* serializer = CSF_AT(void*, archive, 0x04);
        CSF_AT(uint8_t, archive, 0x00) =
            reinterpret_cast<int(__fastcall*)(
                void*, void*, uint8_t*, S_0041f0a0*)>(
                    CSF_AT(void**, serializer, 0)[0x38 / 4])(
                        serializer, nullptr,
                        reinterpret_cast<uint8_t*>(self) + 0x64, archive) == -1;
    }

    if (AiAutonomous_BeginArchiveField(archive))
    {
        void* serializer = CSF_AT(void*, archive, 0x04);
        CSF_AT(uint8_t, archive, 0x00) =
            reinterpret_cast<int(__fastcall*)(
                void*, void*, uint8_t*, S_0041f0a0*)>(
                    CSF_AT(void**, serializer, 0)[0x58 / 4])(
                        serializer, nullptr,
                        reinterpret_cast<uint8_t*>(self) + 0x74, archive) == -1;
    }

    if (AiAutonomous_BeginArchiveField(archive))
    {
        void* serializer = CSF_AT(void*, archive, 0x04);
        CSF_AT(uint8_t, archive, 0x00) =
            reinterpret_cast<int(__fastcall*)(
                void*, void*, uint8_t*, S_0041f0a0*)>(
                    CSF_AT(void**, serializer, 0)[0x58 / 4])(
                        serializer, nullptr,
                        reinterpret_cast<uint8_t*>(self) + 0x78, archive) == -1;
    }

    if (AiAutonomous_BeginArchiveField(archive))
    {
        void* serializer = CSF_AT(void*, archive, 0x04);
        CSF_AT(uint8_t, archive, 0x00) =
            reinterpret_cast<int(__fastcall*)(
                void*, void*, uint8_t*, S_0041f0a0*)>(
                    CSF_AT(void**, serializer, 0)[0x58 / 4])(
                        serializer, nullptr,
                        reinterpret_cast<uint8_t*>(self) + 0x7C, archive) == -1;
    }

    if (AiAutonomous_BeginArchiveField(archive))
    {
        void* serializer = CSF_AT(void*, archive, 0x04);
        CSF_AT(uint8_t, archive, 0x00) =
            reinterpret_cast<int(__fastcall*)(
                void*, void*, uint8_t*, S_0041f0a0*)>(
                    CSF_AT(void**, serializer, 0)[0x48 / 4])(
                        serializer, nullptr,
                        reinterpret_cast<uint8_t*>(self) + 0x80, archive) == -1;
    }

    if (AiAutonomous_BeginArchiveField(archive))
    {
        void* serializer = CSF_AT(void*, archive, 0x04);
        CSF_AT(uint8_t, archive, 0x00) =
            reinterpret_cast<int(__fastcall*)(
                void*, void*, uint8_t*, S_0041f0a0*)>(
                    CSF_AT(void**, serializer, 0)[0x54 / 4])(
                        serializer, nullptr,
                        reinterpret_cast<uint8_t*>(self) + 0x70, archive) == -1;
    }

    if (AiAutonomous_BeginArchiveField(archive))
    {
        AiAttackTasks_ResetSlot(
            reinterpret_cast<int>(reinterpret_cast<uint8_t*>(self) + 0x10),
            reinterpret_cast<S_00691860*>(archive));
    }

    if (AiAutonomous_BeginArchiveField(archive))
    {
        AiAttackTasks_ResetSlot(
            reinterpret_cast<int>(reinterpret_cast<uint8_t*>(self) + 0x24),
            reinterpret_cast<S_00691860*>(archive));
    }

    if (AiAutonomous_BeginArchiveField(archive))
    {
        AiAttackTasks_ResetSlot(
            reinterpret_cast<int>(reinterpret_cast<uint8_t*>(self) + 0x38),
            reinterpret_cast<S_00691860*>(archive));
    }

    if (AiAutonomous_BeginArchiveField(archive))
    {
        AiAttackTasks_ResetSlot(
            reinterpret_cast<int>(reinterpret_cast<uint8_t*>(self) + 0x4C),
            reinterpret_cast<S_00691860*>(archive));
    }

    CSF_AT(uint32_t, archive, 0x1C) = savedContext;

    if (CSF_AT(uint8_t, archive, 0x00) == 0)
    {
        void* serializer = CSF_AT(void*, archive, 0x04);
        const bool completed = reinterpret_cast<bool(__fastcall*)(
            void*, void*, S_0041f0a0*)>(
                CSF_AT(void**, serializer, 0)[0x7C / 4])(
                    serializer, nullptr, archive);
        if (completed)
        {
            CSF_AT(uint8_t, archive, 0x00) = 0;
            return completed;
        }
    }

    CSF_AT(uint8_t, archive, 0x00) = 1;
    return 1;
}

HOOK(0x004DA530, S_0041f0a0_SerializeAutonomousTask, "auto");
