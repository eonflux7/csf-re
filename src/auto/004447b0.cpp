#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(unsigned int, __stdcall, C_0083109c_ProcessPrimaryHandlers,
         0x004445B0, (int a1, char* a2));
CSF_FUNC(int, __stdcall, C_0083109c_ProcessSecondaryHandlers,
         0x004443B0, (uint32_t* a1, int a2));

static int __fastcall C_0083109c_ProcessEvent(
    C_0083109c* self, void* /*edx*/, int eventAddress)
{
    void* event = reinterpret_cast<void*>(eventAddress);

    bool handled = false;
    if (CSF_AT(uint8_t, event, 0x00) == 0)
    {
        void* receiver = CSF_AT(void*, event, 0x04);
        handled = !reinterpret_cast<bool(__fastcall*)(void*, void*, int, int)>(
            CSF_AT(void**, receiver, 0)[0x80 / 4])(
                receiver, nullptr, 0, eventAddress);
    }

    const uint32_t savedContext = CSF_AT(uint32_t, event, 0x1C);
    CSF_AT(uint8_t, event, 0x00) = handled;
    CSF_AT(uint32_t, event, 0x1C) = 0;

    if (!handled)
    {
        void* receiver = CSF_AT(void*, event, 0x04);
        CSF_AT(uint8_t, event, 0x15) = 0;
        CSF_AT(uint8_t, event, 0x16) = 0;
        const int result =
            reinterpret_cast<int(__fastcall*)(void*, void*, uint32_t*, int)>(
                CSF_AT(void**, receiver, 0)[0x54 / 4])(
                    receiver, nullptr, &self->field_0008, eventAddress);
        CSF_AT(uint8_t, event, 0x00) = result == -1;
    }

    bool pending = CSF_AT(uint8_t, event, 0x00) == 0;
    CSF_AT(uint32_t, event, 0x1C) = 0;
    if (pending)
    {
        CSF_AT(uint8_t, event, 0x15) = 0;
        CSF_AT(uint8_t, event, 0x16) = 0;
        C_0083109c_ProcessPrimaryHandlers(
            reinterpret_cast<int>(&self->field_000c),
            reinterpret_cast<char*>(event));
    }

    pending = CSF_AT(uint8_t, event, 0x00) == 0;
    CSF_AT(uint32_t, event, 0x1C) = 0;
    if (pending)
    {
        CSF_AT(uint8_t, event, 0x15) = 0;
        CSF_AT(uint8_t, event, 0x16) = 0;
        C_0083109c_ProcessSecondaryHandlers(
            &self->field_001c, eventAddress);
    }

    pending = CSF_AT(uint8_t, event, 0x00) == 0;
    CSF_AT(uint32_t, event, 0x1C) = savedContext;
    if (pending)
    {
        void* receiver = CSF_AT(void*, event, 0x04);
        const bool result =
            reinterpret_cast<bool(__fastcall*)(void*, void*, int)>(
                CSF_AT(void**, receiver, 0)[0x7C / 4])(
                    receiver, nullptr, eventAddress);
        if (result)
        {
            CSF_AT(uint8_t, event, 0x00) = 0;
            return result;
        }
    }

    CSF_AT(uint8_t, event, 0x00) = 1;
    return 1;
}

HOOK(0x004447B0, C_0083109c_ProcessEvent, "auto");
