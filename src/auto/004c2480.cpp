#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

static int __fastcall S_004c2380_CheckTaskConditions(
    S_004c2380* self, void* /*edx*/, void* task)
{
    const uint32_t savedContext = CSF_AT(uint32_t, task, 0x1C);

    bool stopped = false;
    if (CSF_AT(uint8_t, task, 0x00) == 0)
    {
        void* evaluator = CSF_AT(void*, task, 0x04);
        stopped = !((bool(__fastcall*)(void*, void*, int, void*))
            CSF_AT(void**, evaluator, 0)[0x80 / 4])(
                evaluator, nullptr, 0, task);
    }

    CSF_AT(uint8_t, task, 0x00) = stopped;
    CSF_AT(uint32_t, task, 0x1C) = 0;
    if (!stopped)
    {
        void* evaluator = CSF_AT(void*, task, 0x04);
        CSF_AT(uint8_t, task, 0x15) = 0;
        CSF_AT(uint8_t, task, 0x16) = 0;
        CSF_AT(uint8_t, task, 0x00) =
            ((int(__fastcall*)(void*, void*, void*, void*))
                CSF_AT(void**, evaluator, 0)[0x44 / 4])(
                    evaluator, nullptr, self, task) == -1;
    }

    bool continueChecking = CSF_AT(uint8_t, task, 0x00) == 0;
    CSF_AT(uint32_t, task, 0x1C) = 0;
    if (continueChecking)
    {
        void* evaluator = CSF_AT(void*, task, 0x04);
        CSF_AT(uint8_t, task, 0x15) = 0;
        CSF_AT(uint8_t, task, 0x16) = 0;
        CSF_AT(uint8_t, task, 0x00) =
            ((int(__fastcall*)(void*, void*, void*, void*))
                CSF_AT(void**, evaluator, 0)[0x44 / 4])(
                    evaluator, nullptr,
                    reinterpret_cast<uint8_t*>(self) + 1, task) == -1;
    }

    continueChecking = CSF_AT(uint8_t, task, 0x00) == 0;
    CSF_AT(uint32_t, task, 0x1C) = 0;
    if (continueChecking)
    {
        void* evaluator = CSF_AT(void*, task, 0x04);
        CSF_AT(uint8_t, task, 0x15) = 0;
        CSF_AT(uint8_t, task, 0x16) = 0;
        CSF_AT(uint8_t, task, 0x00) =
            ((int(__fastcall*)(void*, void*, void*, void*))
                CSF_AT(void**, evaluator, 0)[0x58 / 4])(
                    evaluator, nullptr,
                    reinterpret_cast<uint8_t*>(self) + 4, task) == -1;
    }

    continueChecking = CSF_AT(uint8_t, task, 0x00) == 0;
    CSF_AT(uint32_t, task, 0x1C) = 0;
    if (continueChecking)
    {
        void* evaluator = CSF_AT(void*, task, 0x04);
        CSF_AT(uint8_t, task, 0x15) = 0;
        CSF_AT(uint8_t, task, 0x16) = 0;
        CSF_AT(uint8_t, task, 0x00) =
            ((int(__fastcall*)(void*, void*, void*, void*))
                CSF_AT(void**, evaluator, 0)[0x54 / 4])(
                    evaluator, nullptr,
                    reinterpret_cast<uint8_t*>(self) + 8, task) == -1;
    }

    continueChecking = CSF_AT(uint8_t, task, 0x00) == 0;
    CSF_AT(uint32_t, task, 0x1C) = 0;
    if (continueChecking)
    {
        void* evaluator = CSF_AT(void*, task, 0x04);
        CSF_AT(uint8_t, task, 0x15) = 0;
        CSF_AT(uint8_t, task, 0x16) = 0;
        CSF_AT(uint8_t, task, 0x00) =
            ((int(__fastcall*)(void*, void*, void*, void*))
                CSF_AT(void**, evaluator, 0)[0x54 / 4])(
                    evaluator, nullptr,
                    reinterpret_cast<uint8_t*>(self) + 0xC, task) == -1;
    }

    CSF_AT(uint32_t, task, 0x1C) = savedContext;
    if (CSF_AT(uint8_t, task, 0x00) == 0)
    {
        void* evaluator = CSF_AT(void*, task, 0x04);
        const int result =
            ((int(__fastcall*)(void*, void*, void*))
                CSF_AT(void**, evaluator, 0)[0x7C / 4])(
                    evaluator, nullptr, task);
        if (static_cast<uint8_t>(result) != 0)
        {
            CSF_AT(uint8_t, task, 0x00) = 0;
            return result;
        }
    }

    CSF_AT(uint8_t, task, 0x00) = 1;
    return 1;
}

HOOK(0x004C2480, S_004c2380_CheckTaskConditions, "auto");
