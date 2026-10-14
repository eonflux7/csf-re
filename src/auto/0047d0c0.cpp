#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, TaskState_ResetSlot, 0x0047BCB0, (int a1, void* a2));
CSF_FUNC(int, __fastcall, Archive_SerializeBaseFields, 0x004DB4F0, (void* self, void* edx, int a2));

static int __fastcall S_0041f0a0_SerializeAttackTaskState(
    S_0041f0a0* self, void* /*edx*/, S_0041f0a0* archive)
{
    const uint32_t savedContext = CSF_AT(uint32_t, archive, 0x1C);

    bool failed = false;
    if (CSF_AT(uint8_t, archive, 0x00) == 0)
    {
        void* stream = CSF_AT(void*, archive, 0x04);
        failed = !reinterpret_cast<bool(__fastcall*)(void*, void*, int, S_0041f0a0*)>(
            CSF_AT(void**, stream, 0)[0x80 / 4])(stream, nullptr, 0, archive);
    }
    CSF_AT(uint8_t, archive, 0x00) = failed;

    Archive_SerializeBaseFields(self, nullptr, reinterpret_cast<int>(archive));

    bool resetSlot = CSF_AT(uint8_t, archive, 0x00) == 0;
    CSF_AT(uint32_t, archive, 0x1C) = 0;
    if (resetSlot)
    {
        CSF_AT(uint8_t, archive, 0x15) = 0;
        CSF_AT(uint8_t, archive, 0x16) = 0;
        TaskState_ResetSlot(
            reinterpret_cast<int>(reinterpret_cast<uint8_t*>(self) + 0x10), archive);
    }

    resetSlot = CSF_AT(uint8_t, archive, 0x00) == 0;
    CSF_AT(uint32_t, archive, 0x1C) = 0;
    if (resetSlot)
    {
        CSF_AT(uint8_t, archive, 0x15) = 0;
        CSF_AT(uint8_t, archive, 0x16) = 0;
        TaskState_ResetSlot(
            reinterpret_cast<int>(reinterpret_cast<uint8_t*>(self) + 0x64), archive);
    }

    resetSlot = CSF_AT(uint8_t, archive, 0x00) == 0;
    CSF_AT(uint32_t, archive, 0x1C) = 0;
    if (resetSlot)
    {
        CSF_AT(uint8_t, archive, 0x15) = 0;
        CSF_AT(uint8_t, archive, 0x16) = 0;
        TaskState_ResetSlot(
            reinterpret_cast<int>(reinterpret_cast<uint8_t*>(self) + 0x24), archive);
    }

    resetSlot = CSF_AT(uint8_t, archive, 0x00) == 0;
    CSF_AT(uint32_t, archive, 0x1C) = 0;
    if (resetSlot)
    {
        CSF_AT(uint8_t, archive, 0x15) = 0;
        CSF_AT(uint8_t, archive, 0x16) = 0;
        TaskState_ResetSlot(
            reinterpret_cast<int>(reinterpret_cast<uint8_t*>(self) + 0x38), archive);
    }

    resetSlot = CSF_AT(uint8_t, archive, 0x00) == 0;
    CSF_AT(uint32_t, archive, 0x1C) = 0;
    if (resetSlot)
    {
        CSF_AT(uint8_t, archive, 0x15) = 0;
        CSF_AT(uint8_t, archive, 0x16) = 0;
        TaskState_ResetSlot(
            reinterpret_cast<int>(reinterpret_cast<uint8_t*>(self) + 0x50), archive);
    }

    const bool canFinish = CSF_AT(uint8_t, archive, 0x00) == 0;
    CSF_AT(uint32_t, archive, 0x1C) = savedContext;
    if (canFinish)
    {
        void* stream = CSF_AT(void*, archive, 0x04);
        const int result =
            reinterpret_cast<int(__fastcall*)(void*, void*, S_0041f0a0*)>(
                CSF_AT(void**, stream, 0)[0x7C / 4])(stream, nullptr, archive);
        if (static_cast<uint8_t>(result) != 0)
        {
            CSF_AT(uint8_t, archive, 0x00) = 0;
            return result;
        }
    }

    CSF_AT(uint8_t, archive, 0x00) = 1;
    return 1;
}

HOOK(0x0047D0C0, S_0041f0a0_SerializeAttackTaskState, "auto");
