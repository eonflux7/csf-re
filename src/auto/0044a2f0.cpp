#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(int, __stdcall, DispatchToEventHandlers, 0x00449E30,
         (uint32_t* a1, int a2));

static int __fastcall ProcessEventThroughHandlers(
    uint32_t* self, void* /*edx*/, int eventAddress)
{
    void* event = reinterpret_cast<void*>(eventAddress);
    const uint32_t savedState = CSF_AT(uint32_t, event, 0x1C);

    bool stopped = false;
    if (CSF_AT(uint8_t, event, 0x00) == 0)
    {
        void* receiver = CSF_AT(void*, event, 0x04);
        stopped = !reinterpret_cast<bool(__fastcall*)(void*, void*, int, int)>(
            CSF_AT(void**, receiver, 0)[0x80 / 4])(
                receiver, nullptr, 0, eventAddress);
    }

    CSF_AT(uint8_t, event, 0x00) = stopped;
    CSF_AT(uint32_t, event, 0x1C) = 0;

    if (!stopped)
    {
        void* receiver = CSF_AT(void*, event, 0x04);
        CSF_AT(uint8_t, event, 0x15) = 0;
        CSF_AT(uint8_t, event, 0x16) = 0;

        const int result =
            reinterpret_cast<int(__fastcall*)(void*, void*, uint32_t*, int)>(
                CSF_AT(void**, receiver, 0)[0x48 / 4])(
                    receiver, nullptr, self, eventAddress);
        CSF_AT(uint8_t, event, 0x00) = (result == -1);
    }

    const bool dispatchFurther = CSF_AT(uint8_t, event, 0x00) == 0;
    CSF_AT(uint32_t, event, 0x1C) = 0;
    if (dispatchFurther)
    {
        CSF_AT(uint8_t, event, 0x15) = 0;
        CSF_AT(uint8_t, event, 0x16) = 0;
        DispatchToEventHandlers(self + 1, eventAddress);
    }

    const bool finalize = CSF_AT(uint8_t, event, 0x00) == 0;
    CSF_AT(uint32_t, event, 0x1C) = savedState;
    if (finalize)
    {
        void* receiver = CSF_AT(void*, event, 0x04);
        const int result =
            reinterpret_cast<int(__fastcall*)(void*, void*, int)>(
                CSF_AT(void**, receiver, 0)[0x7C / 4])(
                    receiver, nullptr, eventAddress);
        if (static_cast<uint8_t>(result) != 0)
        {
            CSF_AT(uint8_t, event, 0x00) = 0;
            return result;
        }
    }

    CSF_AT(uint8_t, event, 0x00) = 1;
    return 1;
}

HOOK(0x0044A2F0, ProcessEventThroughHandlers, "auto");
