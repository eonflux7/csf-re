#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, AiAttackTasks_ResetSlot, 0x0047BCB0, (int owner, S_00691860* slot));
CSF_FUNC(int, __cdecl, Archive_SerializeString, 0x00496080, (int archiveAddress, S_00691860* string));
CSF_FUNC(int, __fastcall, Archive_SerializeBaseFields, 0x004DB4F0, (void* self, void* edx, int a2));

static bool Archive_PrepareField(S_0041f0a0* archive)
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

static void Archive_SerializeIntegerField(void* field, S_0041f0a0* archive)
{
    const bool ready = CSF_AT(uint8_t, archive, 0x00) == 0;
    CSF_AT(uint32_t, archive, 0x1C) = 0;
    if (ready)
    {
        void* stream = CSF_AT(void*, archive, 0x04);
        CSF_AT(uint8_t, archive, 0x15) = 0;
        CSF_AT(uint8_t, archive, 0x16) = 0;
        const int result =
            reinterpret_cast<int(__fastcall*)(void*, void*, void*, S_0041f0a0*)>(
                CSF_AT(void**, stream, 0)[0x48 / 4])(
                    stream, nullptr, field, archive);
        CSF_AT(uint8_t, archive, 0x00) = result == -1;
    }
}

static int __fastcall S_0041f0a0_SerializeAttackTasks(
    S_0041f0a0* self, void* /*edx*/, S_0041f0a0* archive)
{
    bool failed = false;
    if (CSF_AT(uint8_t, archive, 0x00) == 0)
    {
        void* stream = CSF_AT(void*, archive, 0x04);
        failed =
            !reinterpret_cast<bool(__fastcall*)(void*, void*, int, S_0041f0a0*)>(
                CSF_AT(void**, stream, 0)[0x80 / 4])(
                    stream, nullptr, 0, archive);
    }

    const uint32_t savedContext = CSF_AT(uint32_t, archive, 0x1C);
    CSF_AT(uint8_t, archive, 0x00) = failed;
    Archive_SerializeBaseFields(self, nullptr, reinterpret_cast<int>(archive));

    auto* bytes = reinterpret_cast<uint8_t*>(self);
    Archive_SerializeIntegerField(bytes + 0x108, archive);
    Archive_SerializeIntegerField(bytes + 0x10C, archive);
    Archive_SerializeIntegerField(bytes + 0x110, archive);

    if (Archive_PrepareField(archive))
        Archive_SerializeString(reinterpret_cast<int>(bytes + 0xFC),
                                reinterpret_cast<S_00691860*>(archive));

    if (Archive_PrepareField(archive))
        AiAttackTasks_ResetSlot(reinterpret_cast<int>(bytes + 0x10),
                                reinterpret_cast<S_00691860*>(archive));
    if (Archive_PrepareField(archive))
        AiAttackTasks_ResetSlot(reinterpret_cast<int>(bytes + 0x24),
                                reinterpret_cast<S_00691860*>(archive));
    if (Archive_PrepareField(archive))
        AiAttackTasks_ResetSlot(reinterpret_cast<int>(bytes + 0xD4),
                                reinterpret_cast<S_00691860*>(archive));
    if (Archive_PrepareField(archive))
        AiAttackTasks_ResetSlot(reinterpret_cast<int>(bytes + 0xE8),
                                reinterpret_cast<S_00691860*>(archive));
    if (Archive_PrepareField(archive))
        AiAttackTasks_ResetSlot(reinterpret_cast<int>(bytes + 0x60),
                                reinterpret_cast<S_00691860*>(archive));
    if (Archive_PrepareField(archive))
        AiAttackTasks_ResetSlot(reinterpret_cast<int>(bytes + 0xC0),
                                reinterpret_cast<S_00691860*>(archive));
    if (Archive_PrepareField(archive))
        AiAttackTasks_ResetSlot(reinterpret_cast<int>(bytes + 0xAC),
                                reinterpret_cast<S_00691860*>(archive));
    if (Archive_PrepareField(archive))
        AiAttackTasks_ResetSlot(reinterpret_cast<int>(bytes + 0x74),
                                reinterpret_cast<S_00691860*>(archive));
    if (Archive_PrepareField(archive))
        AiAttackTasks_ResetSlot(reinterpret_cast<int>(bytes + 0x98),
                                reinterpret_cast<S_00691860*>(archive));
    if (Archive_PrepareField(archive))
        AiAttackTasks_ResetSlot(reinterpret_cast<int>(bytes + 0x3C),
                                reinterpret_cast<S_00691860*>(archive));

    const bool readyToFinish = CSF_AT(uint8_t, archive, 0x00) == 0;
    CSF_AT(uint32_t, archive, 0x1C) = savedContext;
    if (readyToFinish)
    {
        void* stream = CSF_AT(void*, archive, 0x04);
        // The original tests AL but forwards the entire EAX on success.
        const int result =
            reinterpret_cast<int(__fastcall*)(void*, void*, S_0041f0a0*)>(
                CSF_AT(void**, stream, 0)[0x7C / 4])(
                    stream, nullptr, archive);
        if (static_cast<uint8_t>(result) != 0)
        {
            CSF_AT(uint8_t, archive, 0x00) = 0;
            return result;
        }
    }

    CSF_AT(uint8_t, archive, 0x00) = 1;
    return 1;
}

HOOK(0x004A87D0, S_0041f0a0_SerializeAttackTasks, "auto");
