#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, AiAttackTasks_ResetSlot, 0x0047BCB0, (int owner, S_00691860* slot));
CSF_FUNC(int, __fastcall, Archive_SerializeBaseFields, 0x004DB4F0, (void* self, void* edx, int a2));

static int __fastcall S_0041f0a0_SerializeAttackTaskSlots(
    S_0041f0a0* self, void* /*edx*/, S_0041f0a0* archive)
{
    const uint32_t savedContext = CSF_AT(uint32_t, archive, 0x1C);

    bool failed = false;
    if (CSF_AT(uint8_t, archive, 0x00) == 0)
    {
        void* backend = CSF_AT(void*, archive, 0x04);
        failed = !((bool(__fastcall*)(void*, void*, int, S_0041f0a0*))
            CSF_AT(void**, backend, 0)[0x80 / 4])(
                backend, nullptr, 0, archive);
    }
    CSF_AT(uint8_t, archive, 0x00) = failed;

    Archive_SerializeBaseFields(
        self, nullptr, reinterpret_cast<int>(archive));

    bool processSlot = CSF_AT(uint8_t, archive, 0x00) == 0;
    CSF_AT(uint32_t, archive, 0x1C) = 0;
    if (processSlot)
    {
        CSF_AT(uint8_t, archive, 0x15) = 0;
        CSF_AT(uint8_t, archive, 0x16) = 0;
        AiAttackTasks_ResetSlot(
            reinterpret_cast<int>(reinterpret_cast<uint8_t*>(self) + 0x64),
            reinterpret_cast<S_00691860*>(archive));
    }

    processSlot = CSF_AT(uint8_t, archive, 0x00) == 0;
    CSF_AT(uint32_t, archive, 0x1C) = 0;
    if (processSlot)
    {
        CSF_AT(uint8_t, archive, 0x15) = 0;
        CSF_AT(uint8_t, archive, 0x16) = 0;
        AiAttackTasks_ResetSlot(
            reinterpret_cast<int>(reinterpret_cast<uint8_t*>(self) + 0x10),
            reinterpret_cast<S_00691860*>(archive));
    }

    processSlot = CSF_AT(uint8_t, archive, 0x00) == 0;
    CSF_AT(uint32_t, archive, 0x1C) = 0;
    if (processSlot)
    {
        CSF_AT(uint8_t, archive, 0x15) = 0;
        CSF_AT(uint8_t, archive, 0x16) = 0;
        AiAttackTasks_ResetSlot(
            reinterpret_cast<int>(reinterpret_cast<uint8_t*>(self) + 0x24),
            reinterpret_cast<S_00691860*>(archive));
    }

    processSlot = CSF_AT(uint8_t, archive, 0x00) == 0;
    CSF_AT(uint32_t, archive, 0x1C) = 0;
    if (processSlot)
    {
        CSF_AT(uint8_t, archive, 0x15) = 0;
        CSF_AT(uint8_t, archive, 0x16) = 0;
        AiAttackTasks_ResetSlot(
            reinterpret_cast<int>(reinterpret_cast<uint8_t*>(self) + 0x38),
            reinterpret_cast<S_00691860*>(archive));
    }

    processSlot = CSF_AT(uint8_t, archive, 0x00) == 0;
    CSF_AT(uint32_t, archive, 0x1C) = 0;
    if (processSlot)
    {
        CSF_AT(uint8_t, archive, 0x15) = 0;
        CSF_AT(uint8_t, archive, 0x16) = 0;
        AiAttackTasks_ResetSlot(
            reinterpret_cast<int>(reinterpret_cast<uint8_t*>(self) + 0x50),
            reinterpret_cast<S_00691860*>(archive));
    }

    processSlot = CSF_AT(uint8_t, archive, 0x00) == 0;
    CSF_AT(uint32_t, archive, 0x1C) = 0;
    if (processSlot)
    {
        void* backend = CSF_AT(void*, archive, 0x04);
        CSF_AT(uint8_t, archive, 0x15) = 0;
        CSF_AT(uint8_t, archive, 0x16) = 0;
        const int result =
            ((int(__fastcall*)(void*, void*, uint8_t*, S_0041f0a0*))
                CSF_AT(void**, backend, 0)[0x44 / 4])(
                    backend, nullptr,
                    reinterpret_cast<uint8_t*>(self) + 0x78, archive);
        CSF_AT(uint8_t, archive, 0x00) = result == -1;
    }

    CSF_AT(uint32_t, archive, 0x1C) = savedContext;

    if (CSF_AT(uint8_t, archive, 0x00) == 0)
    {
        void* backend = CSF_AT(void*, archive, 0x04);
        const bool result =
            ((bool(__fastcall*)(void*, void*, S_0041f0a0*))
                CSF_AT(void**, backend, 0)[0x7C / 4])(
                    backend, nullptr, archive);
        if (result)
        {
            CSF_AT(uint8_t, archive, 0x00) = 0;
            return result;
        }
    }

    CSF_AT(uint8_t, archive, 0x00) = 1;
    return 1;
}

HOOK(0x00423E20, S_0041f0a0_SerializeAttackTaskSlots, "auto");
