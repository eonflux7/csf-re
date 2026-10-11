#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, TraverseVisitorOverSubobjects, 0x00464380, (void* self, void* , uint8_t* state));
CSF_FUNC(int, __cdecl, AiAttackTasks_ResetSlot, 0x0047BCB0, (int owner, S_00691860* slot));
CSF_FUNC(char, __cdecl, Archive_SerializeObjectRef, 0x004ADF10, (void* a3, bool* a2));
CSF_FUNC(int, __fastcall, AiAutonomous_SerializeByteAndPair, 0x004D18F0, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, AiAutonomous_SerializeBaseState, 0x004D19C0, (void* self, void* edx, void* a2));

static bool AiAutonomous_BeginField(uint8_t* state)
{
    const bool active = CSF_AT(uint8_t, state, 0x00) == 0;
    CSF_AT(uint32_t, state, 0x1C) = 0;
    return active;
}

static void AiAutonomous_SerializeInteger(uint8_t* state, int* value)
{
    if (!AiAutonomous_BeginField(state))
        return;

    void* archive = CSF_AT(void*, state, 0x04);
    CSF_AT(uint8_t, state, 0x15) = 0;
    CSF_AT(uint8_t, state, 0x16) = 0;
    CSF_AT(uint8_t, state, 0x00) =
        reinterpret_cast<int (__fastcall*)(void*, void*, int*, uint8_t*)>(
            CSF_AT(void**, archive, 0)[88 / 4])(
                archive, nullptr, value, state) == -1;
}

static void AiAutonomous_SerializeByte(uint8_t* state, uint8_t* value)
{
    if (!AiAutonomous_BeginField(state))
        return;

    void* archive = CSF_AT(void*, state, 0x04);
    CSF_AT(uint8_t, state, 0x15) = 0;
    CSF_AT(uint8_t, state, 0x16) = 0;
    CSF_AT(uint8_t, state, 0x00) =
        reinterpret_cast<int (__fastcall*)(void*, void*, uint8_t*, uint8_t*)>(
            CSF_AT(void**, archive, 0)[68 / 4])(
                archive, nullptr, value, state) == -1;
}

static void AiAutonomous_SerializeReference(uint8_t* state, void* reference)
{
    if (!AiAutonomous_BeginField(state))
        return;

    CSF_AT(uint8_t, state, 0x15) = 1;
    CSF_AT(uint8_t, state, 0x16) = 0;
    Archive_SerializeObjectRef(reference, reinterpret_cast<bool*>(state));
}

static void AiAutonomous_SerializeTask(uint8_t* state, void* task)
{
    if (!AiAutonomous_BeginField(state))
        return;

    CSF_AT(uint8_t, state, 0x15) = 0;
    CSF_AT(uint8_t, state, 0x16) = 0;
    AiAttackTasks_ResetSlot(
        reinterpret_cast<int>(task), reinterpret_cast<S_00691860*>(state));
}

static int __fastcall S_0041f0a0_SerializeAutonomousState(
    S_0041f0a0* self, void* /*edx*/, uint8_t* state)
{
    bool failed = false;
    if (CSF_AT(uint8_t, state, 0x00) == 0)
    {
        void* archive = CSF_AT(void*, state, 0x04);
        failed =
            !reinterpret_cast<bool (__fastcall*)(void*, void*, int, uint8_t*)>(
                CSF_AT(void**, archive, 0)[128 / 4])(
                    archive, nullptr, 0, state);
    }

    const uint32_t savedContext = CSF_AT(uint32_t, state, 0x1C);
    CSF_AT(uint8_t, state, 0x00) = failed;
    AiAutonomous_SerializeBaseState(self, nullptr, state);

    int taskType = 0;
    if (CSF_AT(uint8_t, state, 0x14) != 0)
    {
        void* task = reinterpret_cast<void*>(self->field_000c);
        if (task)
        {
            taskType = reinterpret_cast<int (__fastcall*)(void*, void*)>(
                CSF_AT(void**, task, 0)[128 / 4])(task, nullptr);
        }
    }

    AiAutonomous_SerializeInteger(state, &taskType);
    AiAutonomous_SerializeInteger(state, &CSF_AT(int, self, 0x2F0));
    AiAutonomous_SerializeInteger(state, &CSF_AT(int, self, 0x2F4));
    AiAutonomous_SerializeInteger(state, &CSF_AT(int, self, 0x2F8));
    AiAutonomous_SerializeInteger(state, &CSF_AT(int, self, 0x2FC));
    AiAutonomous_SerializeInteger(state, &CSF_AT(int, self, 0x300));

    AiAutonomous_SerializeReference(state, &CSF_AT(uint32_t, self, 0x304));
    AiAutonomous_SerializeReference(state, &CSF_AT(uint32_t, self, 0x308));
    AiAutonomous_SerializeReference(state, &CSF_AT(uint32_t, self, 0x30C));

    AiAutonomous_SerializeTask(state, reinterpret_cast<uint8_t*>(self) + 0x3C);
    AiAutonomous_SerializeTask(state, reinterpret_cast<uint8_t*>(self) + 0x220);

    if (CSF_AT(int, state, 0x18) > 50 || !taskType)
        AiAutonomous_SerializeTask(state, reinterpret_cast<uint8_t*>(self) + 0x50);
    if (CSF_AT(int, state, 0x18) > 50 || taskType == 6)
        AiAutonomous_SerializeTask(state, reinterpret_cast<uint8_t*>(self) + 0x108);
    if (CSF_AT(int, state, 0x18) > 50 || taskType == 7)
        AiAutonomous_SerializeTask(state, reinterpret_cast<uint8_t*>(self) + 0x64);
    if (CSF_AT(int, state, 0x18) > 50 || taskType == 16)
        AiAutonomous_SerializeTask(state, reinterpret_cast<uint8_t*>(self) + 0x130);
    if (CSF_AT(int, state, 0x18) > 50 || taskType == 5)
        AiAutonomous_SerializeTask(state, reinterpret_cast<uint8_t*>(self) + 0xE0);
    if (CSF_AT(int, state, 0x18) > 50 || taskType == 14)
        AiAutonomous_SerializeTask(state, reinterpret_cast<uint8_t*>(self) + 0x160);
    if (CSF_AT(int, state, 0x18) > 50 || taskType == 15)
        AiAutonomous_SerializeTask(state, reinterpret_cast<uint8_t*>(self) + 0x19C);
    if (CSF_AT(int, state, 0x18) > 50 || taskType == 17)
        AiAutonomous_SerializeTask(state, reinterpret_cast<uint8_t*>(self) + 0x2A8);
    if (CSF_AT(int, state, 0x18) > 50 || taskType == 13)
        AiAutonomous_SerializeTask(state, reinterpret_cast<uint8_t*>(self) + 0x144);
    if (CSF_AT(int, state, 0x18) > 50 || taskType == 11)
        AiAutonomous_SerializeTask(state, reinterpret_cast<uint8_t*>(self) + 0xB4);
    if (CSF_AT(int, state, 0x18) > 50 || taskType == 8)
        AiAutonomous_SerializeTask(state, reinterpret_cast<uint8_t*>(self) + 0x78);
    if (CSF_AT(int, state, 0x18) > 50 || taskType == 1)
        AiAutonomous_SerializeTask(state, reinterpret_cast<uint8_t*>(self) + 0x8C);
    if (CSF_AT(int, state, 0x18) > 50 || taskType == 9)
        AiAutonomous_SerializeTask(state, reinterpret_cast<uint8_t*>(self) + 0xA0);
    if (CSF_AT(int, state, 0x18) > 50 || taskType == 4)
        AiAutonomous_SerializeTask(state, reinterpret_cast<uint8_t*>(self) + 0xF4);
    if (CSF_AT(int, state, 0x18) > 50 || taskType == 3)
        AiAutonomous_SerializeTask(state, reinterpret_cast<uint8_t*>(self) + 0xC8);
    if (CSF_AT(int, state, 0x18) > 50 || taskType == 10)
        AiAutonomous_SerializeTask(state, reinterpret_cast<uint8_t*>(self) + 0x11C);

    AiAutonomous_SerializeByte(state, &self->field_0310);
    if (self->field_0310)
    {
        if (AiAutonomous_BeginField(state))
        {
            CSF_AT(uint8_t, state, 0x15) = 0;
            CSF_AT(uint8_t, state, 0x16) = 0;
            TraverseVisitorOverSubobjects(
                reinterpret_cast<uint8_t*>(self) + 0x314, nullptr, state);
        }
    }

    AiAutonomous_SerializeByte(state, &CSF_AT(uint8_t, self, 0x338));
    if (AiAutonomous_BeginField(state))
    {
        CSF_AT(uint8_t, state, 0x15) = 0;
        CSF_AT(uint8_t, state, 0x16) = 0;
        AiAutonomous_SerializeByteAndPair(
            reinterpret_cast<uint8_t*>(self) + 0x32C,  // lea ecx, [edi + 0x32C]
            nullptr, reinterpret_cast<int>(state));
    }

    CSF_AT(uint32_t, state, 0x1C) = savedContext;
    if (CSF_AT(uint8_t, state, 0x00) != 0)
    {
        CSF_AT(uint8_t, state, 0x00) = 1;
        return 1;
    }

    void* archive = CSF_AT(void*, state, 0x04);
    const int result =
        reinterpret_cast<int (__fastcall*)(void*, void*, uint8_t*)>(
            CSF_AT(void**, archive, 0)[124 / 4])(archive, nullptr, state);
    if (static_cast<uint8_t>(result) == 0)
    {
        CSF_AT(uint8_t, state, 0x00) = 1;
        return 1;
    }

    CSF_AT(uint8_t, state, 0x00) = 0;
    return result;
}

HOOK(0x004DA030, S_0041f0a0_SerializeAutonomousState, "auto");

