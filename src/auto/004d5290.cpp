#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, AiAttackTasks_ResetSlot, 0x0047BCB0, (int owner, S_00691860* slot));
CSF_FUNC(int, __cdecl, Archive_SerializeString, 0x00496080, (int archiveAddress, S_00691860* string));
CSF_FUNC(int, __fastcall, S_0041f0a0_SerializeAutonomousState_4DA030, 0x004DA030, (S_0041f0a0* self, void* , uint8_t* state));

static int __fastcall S_0041f0a0_SerializeGiveObjectTask(
    S_0041f0a0* self, void* /*edx*/, S_0041f0a0* state)
{
    auto* taskBytes = reinterpret_cast<uint8_t*>(self);
    bool failed = false;
    if (CSF_AT(uint8_t, state, 0) == 0)
    {
        void* archive = CSF_AT(void*, state, 0x04);
        failed = !((bool(__fastcall*)(void*, void*, int, S_0041f0a0*))
            CSF_AT(void**, archive, 0)[0x80 / 4])(
                archive, nullptr, 0, state);
    }

    const uint32_t savedContext = CSF_AT(uint32_t, state, 0x1C);
    CSF_AT(uint8_t, state, 0) = failed;
    S_0041f0a0_SerializeAutonomousState_4DA030(
        self, nullptr, reinterpret_cast<uint8_t*>(state));

    bool canSerialize = CSF_AT(uint8_t, state, 0) == 0;
    CSF_AT(uint32_t, state, 0x1C) = 0;
    if (canSerialize)
    {
        CSF_AT(uint8_t, state, 0x15) = 0;
        CSF_AT(uint8_t, state, 0x16) = 0;
        Archive_SerializeString(
            reinterpret_cast<int>(taskBytes + 952),
            reinterpret_cast<S_00691860*>(state));
    }

    canSerialize = CSF_AT(uint8_t, state, 0) == 0;
    CSF_AT(uint32_t, state, 0x1C) = 0;
    if (canSerialize)
    {
        void* archive = CSF_AT(void*, state, 0x04);
        CSF_AT(uint8_t, state, 0x15) = 0;
        CSF_AT(uint8_t, state, 0x16) = 0;
        CSF_AT(uint8_t, state, 0) =
            ((int(__fastcall*)(void*, void*, uint8_t*, S_0041f0a0*))
                CSF_AT(void**, archive, 0)[0x38 / 4])(
                    archive, nullptr, taskBytes + 964, state) == -1;
    }

    canSerialize = CSF_AT(uint8_t, state, 0) == 0;
    CSF_AT(uint32_t, state, 0x1C) = 0;
    if (canSerialize)
    {
        void* archive = CSF_AT(void*, state, 0x04);
        CSF_AT(uint8_t, state, 0x15) = 0;
        CSF_AT(uint8_t, state, 0x16) = 0;
        CSF_AT(uint8_t, state, 0) =
            ((int(__fastcall*)(void*, void*, uint8_t*, S_0041f0a0*))
                CSF_AT(void**, archive, 0)[0x38 / 4])(
                    archive, nullptr, taskBytes + 980, state) == -1;
    }

    canSerialize = CSF_AT(uint8_t, state, 0) == 0;
    CSF_AT(uint32_t, state, 0x1C) = 0;
    if (canSerialize)
    {
        void* archive = CSF_AT(void*, state, 0x04);
        CSF_AT(uint8_t, state, 0x15) = 0;
        CSF_AT(uint8_t, state, 0x16) = 0;
        CSF_AT(uint8_t, state, 0) =
            ((int(__fastcall*)(void*, void*, uint8_t*, S_0041f0a0*))
                CSF_AT(void**, archive, 0)[0x44 / 4])(
                    archive, nullptr, taskBytes + 992, state) == -1;
    }

    canSerialize = CSF_AT(uint8_t, state, 0) == 0;
    CSF_AT(uint32_t, state, 0x1C) = 0;
    if (canSerialize)
    {
        void* archive = CSF_AT(void*, state, 0x04);
        CSF_AT(uint8_t, state, 0x15) = 0;
        CSF_AT(uint8_t, state, 0x16) = 0;
        CSF_AT(uint8_t, state, 0) =
            ((int(__fastcall*)(void*, void*, uint8_t*, S_0041f0a0*))
                CSF_AT(void**, archive, 0)[0x44 / 4])(
                    archive, nullptr, taskBytes + 976, state) == -1;
    }

    canSerialize = CSF_AT(uint8_t, state, 0) == 0;
    CSF_AT(uint32_t, state, 0x1C) = 0;
    if (canSerialize)
    {
        CSF_AT(uint8_t, state, 0x15) = 0;
        CSF_AT(uint8_t, state, 0x16) = 0;
        AiAttackTasks_ResetSlot(
            reinterpret_cast<int>(taskBytes + 828),
            reinterpret_cast<S_00691860*>(state));
    }

    canSerialize = CSF_AT(uint8_t, state, 0) == 0;
    CSF_AT(uint32_t, state, 0x1C) = 0;
    if (canSerialize)
    {
        void* archive = CSF_AT(void*, state, 0x04);
        CSF_AT(uint8_t, state, 0x15) = 0;
        CSF_AT(uint8_t, state, 0x16) = 0;
        CSF_AT(uint8_t, state, 0) =
            ((int(__fastcall*)(void*, void*, uint8_t*, S_0041f0a0*))
                CSF_AT(void**, archive, 0)[0x54 / 4])(
                    archive, nullptr, taskBytes + 996, state) == -1;
    }

    CSF_AT(uint32_t, state, 0x1C) = savedContext;
    if (CSF_AT(uint8_t, state, 0) != 0)
    {
        CSF_AT(uint8_t, state, 0) = 1;
        return 1;
    }

    void* archive = CSF_AT(void*, state, 0x04);
    // Only AL is tested, but the successful path returns the entire EAX.
    const int result =
        ((int(__fastcall*)(void*, void*, S_0041f0a0*))
            CSF_AT(void**, archive, 0)[0x7C / 4])(
                archive, nullptr, state);
    if (static_cast<uint8_t>(result) == 0)
    {
        CSF_AT(uint8_t, state, 0) = 1;
        return 1;
    }

    CSF_AT(uint8_t, state, 0) = 0;
    return result;
}

HOOK(0x004D5290, S_0041f0a0_SerializeGiveObjectTask, "auto");
