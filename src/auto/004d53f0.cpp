#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, AiAttackTasks_ResetSlot, 0x0047BCB0, (int owner, S_00691860* slot));
CSF_FUNC(int, __fastcall, Archive_SerializeBaseFields, 0x004DB4F0, (void* self, void* edx, int a2));

static int __fastcall S_0041f0a0_SerializeGiveObjectTask(
    S_0041f0a0* self, void* /*edx*/, S_0041f0a0* archive)
{
    const uint32_t savedContext = CSF_AT(uint32_t, archive, 0x1C);

    bool beginFailed = false;
    if (CSF_AT(uint8_t, archive, 0) == 0)
    {
        void* serializer = CSF_AT(void*, archive, 0x04);
        beginFailed =
            !reinterpret_cast<bool(__fastcall*)(void*, void*, int, S_0041f0a0*)>(
                CSF_AT(void**, serializer, 0)[0x80 / 4])(
                    serializer, nullptr, 0, archive);
    }
    CSF_AT(uint8_t, archive, 0) = beginFailed;

    Archive_SerializeBaseFields(self, nullptr, reinterpret_cast<int>(archive));

    bool canSerialize = CSF_AT(uint8_t, archive, 0) == 0;
    CSF_AT(uint32_t, archive, 0x1C) = 0;
    if (canSerialize)
    {
        CSF_AT(uint8_t, archive, 0x15) = 0;
        CSF_AT(uint8_t, archive, 0x16) = 0;
        AiAttackTasks_ResetSlot(
            reinterpret_cast<int>(reinterpret_cast<uint8_t*>(self) + 0x24),
            reinterpret_cast<S_00691860*>(archive));
    }

    canSerialize = CSF_AT(uint8_t, archive, 0) == 0;
    CSF_AT(uint32_t, archive, 0x1C) = 0;
    if (canSerialize)
    {
        CSF_AT(uint8_t, archive, 0x15) = 0;
        CSF_AT(uint8_t, archive, 0x16) = 0;
        AiAttackTasks_ResetSlot(
            reinterpret_cast<int>(reinterpret_cast<uint8_t*>(self) + 0x38),
            reinterpret_cast<S_00691860*>(archive));
    }

    canSerialize = CSF_AT(uint8_t, archive, 0) == 0;
    CSF_AT(uint32_t, archive, 0x1C) = 0;
    if (canSerialize)
    {
        CSF_AT(uint8_t, archive, 0x15) = 0;
        CSF_AT(uint8_t, archive, 0x16) = 0;
        AiAttackTasks_ResetSlot(
            reinterpret_cast<int>(reinterpret_cast<uint8_t*>(self) + 0x4C),
            reinterpret_cast<S_00691860*>(archive));
    }

    canSerialize = CSF_AT(uint8_t, archive, 0) == 0;
    CSF_AT(uint32_t, archive, 0x1C) = 0;
    if (canSerialize)
    {
        CSF_AT(uint8_t, archive, 0x15) = 0;
        CSF_AT(uint8_t, archive, 0x16) = 0;
        AiAttackTasks_ResetSlot(
            reinterpret_cast<int>(reinterpret_cast<uint8_t*>(self) + 0x60),
            reinterpret_cast<S_00691860*>(archive));
    }

    canSerialize = CSF_AT(uint8_t, archive, 0) == 0;
    CSF_AT(uint32_t, archive, 0x1C) = 0;
    if (canSerialize)
    {
        void* serializer = CSF_AT(void*, archive, 0x04);
        CSF_AT(uint8_t, archive, 0x15) = 0;
        CSF_AT(uint8_t, archive, 0x16) = 0;
        const int result =
            reinterpret_cast<int(__fastcall*)(void*, void*, uint8_t*, S_0041f0a0*)>(
                CSF_AT(void**, serializer, 0)[0x58 / 4])(
                    serializer, nullptr,
                    reinterpret_cast<uint8_t*>(self) + 0x74, archive);
        CSF_AT(uint8_t, archive, 0) = result == -1;
    }

    canSerialize = CSF_AT(uint8_t, archive, 0) == 0;
    CSF_AT(uint32_t, archive, 0x1C) = 0;
    if (canSerialize)
    {
        CSF_AT(uint8_t, archive, 0x15) = 0;
        CSF_AT(uint8_t, archive, 0x16) = 0;
        AiAttackTasks_ResetSlot(
            reinterpret_cast<int>(reinterpret_cast<uint8_t*>(self) + 0x10),
            reinterpret_cast<S_00691860*>(archive));
    }

    canSerialize = CSF_AT(uint8_t, archive, 0) == 0;
    CSF_AT(uint32_t, archive, 0x1C) = savedContext;
    if (canSerialize)
    {
        void* serializer = CSF_AT(void*, archive, 0x04);
        // Only AL is tested, but the successful path returns the full EAX.
        const int result =
            reinterpret_cast<int(__fastcall*)(void*, void*, S_0041f0a0*)>(
                CSF_AT(void**, serializer, 0)[0x7C / 4])(
                    serializer, nullptr, archive);
        if (static_cast<uint8_t>(result) != 0)
        {
            CSF_AT(uint8_t, archive, 0) = 0;
            return result;
        }
    }

    CSF_AT(uint8_t, archive, 0) = 1;
    return 1;
}

HOOK(0x004D53F0, S_0041f0a0_SerializeGiveObjectTask, "auto");
