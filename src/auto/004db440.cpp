#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

static int __fastcall C_00834fa8_SetLinkedTask(
    C_00834fa8* self, void* /*edx*/, int taskAddress)
{
    void* context = CSF_AT(void*, self, 0x0C);
    if (!context)
        return 0;

    void* newTask = reinterpret_cast<void*>(
        static_cast<uintptr_t>(taskAddress));
    void* oldTask = CSF_AT(void*, context, 0x10);

    if (newTask != oldTask)
    {
        if (oldTask)
        {
            reinterpret_cast<void (__fastcall*)(void*, void*)>(
                CSF_AT(void**, oldTask, 0)[0x94 / 4])(
                    oldTask, nullptr);
        }

        CSF_AT(void*, context, 0x10) = newTask;
        if (newTask)
        {
            CSF_AT(uint32_t, newTask, 0x08) =
                CSF_AT(uint32_t, context, 0x08);

            CSF_AT(void*, CSF_AT(void*, context, 0x10), 0x0C) = context;

            void* linkedTask = CSF_AT(void*, context, 0x10);
            reinterpret_cast<void (__fastcall*)(void*, void*)>(
                CSF_AT(void**, linkedTask, 0)[0x8C / 4])(
                    linkedTask, nullptr);
        }
    }

    return taskAddress;
}

HOOK(0x004DB440, C_00834fa8_SetLinkedTask, "auto");
