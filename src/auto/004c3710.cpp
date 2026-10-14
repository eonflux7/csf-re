#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __stdcall, AiStateFactory_ProcessFallbackTask, 0x004B9320,
         (int a1, int a2));

static int __fastcall S_00523cf0_ProcessCoverAttackTasks(
    S_00523cf0* self, void* /*edx*/, int taskAddress)
{
    void* task = reinterpret_cast<void*>(taskAddress);
    const int savedContext = CSF_AT(int, task, 0x1C);

    bool finished = false;
    if (CSF_AT(std::uint8_t, task, 0) == 0)
    {
        void* owner = CSF_AT(void*, task, 4);
        finished = !reinterpret_cast<bool(__fastcall*)(void*, void*, int, void*)>(
            CSF_AT(void**, owner, 0)[0x80 / 4])(owner, nullptr, 0, task);
    }

    CSF_AT(std::uint8_t, task, 0) = finished;
    CSF_AT(int, task, 0x1C) = 0;
    if (!finished)
    {
        void* owner = CSF_AT(void*, task, 4);
        CSF_AT(std::uint8_t, task, 0x15) = 0;
        CSF_AT(std::uint8_t, task, 0x16) = 0;
        CSF_AT(std::uint8_t, task, 0) =
            reinterpret_cast<int(__fastcall*)(void*, void*, S_00523cf0*, void*)>(
                CSF_AT(void**, owner, 0)[0x48 / 4])(owner, nullptr, self, task) == -1;
    }

    bool pending = CSF_AT(std::uint8_t, task, 0) == 0;
    CSF_AT(int, task, 0x1C) = 0;
    if (pending)
    {
        void* owner = CSF_AT(void*, task, 4);
        CSF_AT(std::uint8_t, task, 0x15) = 0;
        CSF_AT(std::uint8_t, task, 0x16) = 0;
        CSF_AT(std::uint8_t, task, 0) =
            reinterpret_cast<int(__fastcall*)(void*, void*, S_00523cf0*, void*)>(
                CSF_AT(void**, owner, 0)[0x54 / 4])(
                owner, nullptr,
                reinterpret_cast<S_00523cf0*>(
                    reinterpret_cast<std::uint8_t*>(self) + 4),
                task) == -1;
    }

    pending = CSF_AT(std::uint8_t, task, 0) == 0;
    CSF_AT(int, task, 0x1C) = 0;
    if (pending)
    {
        void* owner = CSF_AT(void*, task, 4);
        CSF_AT(std::uint8_t, task, 0x15) = 0;
        CSF_AT(std::uint8_t, task, 0x16) = 0;
        CSF_AT(std::uint8_t, task, 0) =
            reinterpret_cast<int(__fastcall*)(void*, void*, S_00523cf0*, void*)>(
                CSF_AT(void**, owner, 0)[0x38 / 4])(
                owner, nullptr,
                reinterpret_cast<S_00523cf0*>(
                    reinterpret_cast<std::uint8_t*>(self) + 8),
                task) == -1;
    }

    pending = CSF_AT(std::uint8_t, task, 0) == 0;
    CSF_AT(int, task, 0x1C) = 0;
    if (pending)
    {
        CSF_AT(std::uint8_t, task, 0x15) = 0;
        CSF_AT(std::uint8_t, task, 0x16) = 0;
        AiStateFactory_ProcessFallbackTask(
            reinterpret_cast<int>(
                reinterpret_cast<std::uint8_t*>(self) + 0x14),
            taskAddress);
    }

    pending = CSF_AT(std::uint8_t, task, 0) == 0;
    CSF_AT(int, task, 0x1C) = savedContext;
    if (pending)
    {
        void* owner = CSF_AT(void*, task, 4);
        const int result =
            reinterpret_cast<int(__fastcall*)(void*, void*, void*)>(
                CSF_AT(void**, owner, 0)[0x7C / 4])(owner, nullptr, task);
        if (static_cast<std::uint8_t>(result) != 0)
        {
            CSF_AT(std::uint8_t, task, 0) = 0;
            return result;
        }
    }

    CSF_AT(std::uint8_t, task, 0) = 1;
    return 1;
}

HOOK(0x004C3710, S_00523cf0_ProcessCoverAttackTasks, "auto");
