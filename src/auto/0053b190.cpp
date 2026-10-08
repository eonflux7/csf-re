#include <cstdint>
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "globals.h"

CSF_FUNC(int, __cdecl, GetGlobalState, 0x0050D190, ());
CSF_FUNC(int, __cdecl, GetGlobalValue, 0x0075FDF0, ());

// The original replaces only AL before passing EAX to the control.
// Preserve the upper bytes, including those returned by GetGlobalState.
static uint32_t __fastcall CMenuGameOptions_GetRawDisableArgument(
    CMenuGameOptions* self, void* /*edx*/)
{
    uint32_t raw =
        ((uint32_t(__fastcall*)(void*, void*))CSF_AT(void**, self, 0)[0x30 / 4])
        (self, nullptr);

    if (static_cast<uint8_t>(raw) != 0)
    {
        raw = static_cast<uint32_t>(GetGlobalState());
        if (CSF_AT(uint8_t, reinterpret_cast<void*>(raw), 0xD0) != 0)
            return raw & 0xFFFFFF00u;
    }

    raw =
        ((uint32_t(__fastcall*)(void*, void*))CSF_AT(void**, self, 0)[0x30 / 4])
        (self, nullptr);

    return (raw & 0xFFFFFF00u) | (static_cast<uint8_t>(raw) != 0 ? 1u : 0u);
}

static int __fastcall CMenuGameOptions_RefreshOptionControls(
    CMenuGameOptions* self, void* /*edx*/)
{
    uint32_t result = self->field_0024;
    if (result == 0)
        return static_cast<int>(result);

    void* control = reinterpret_cast<void*>(self->field_0034);
    if (control != nullptr)
    {
        void* settings = reinterpret_cast<void*>(self->field_0024);
        void** settingsTable = CSF_AT(void**, settings, 0);
        void** controlTable = CSF_AT(void**, control, 0);
        uint32_t state =
            ((uint32_t(__fastcall*)(void*, void*))settingsTable[0x20 / 4])
            (settings, nullptr);

        control = reinterpret_cast<void*>(self->field_0034);
        uint32_t selection = 0x534F3131u - (static_cast<uint8_t>(state) != 0);
        ((void(__fastcall*)(void*, void*, uint32_t))controlTable[0x2A4 / 4])
            (control, nullptr, selection);
    }

    control = reinterpret_cast<void*>(self->field_0038);
    if (control != nullptr)
    {
        void* settings = reinterpret_cast<void*>(self->field_0024);
        void** settingsTable = CSF_AT(void**, settings, 0);
        void** controlTable = CSF_AT(void**, control, 0);
        uint32_t state =
            ((uint32_t(__fastcall*)(void*, void*))settingsTable[0x28 / 4])
            (settings, nullptr);

        control = reinterpret_cast<void*>(self->field_0038);
        uint32_t selection = 0x534F3131u - (static_cast<uint8_t>(state) != 0);
        ((void(__fastcall*)(void*, void*, uint32_t))controlTable[0x2A4 / 4])
            (control, nullptr, selection);
    }

    control = reinterpret_cast<void*>(self->field_003c);
    if (control != nullptr)
    {
        void* settings = reinterpret_cast<void*>(self->field_0024);
        void** settingsTable = CSF_AT(void**, settings, 0);
        void** controlTable = CSF_AT(void**, control, 0);
        uint32_t state =
            ((uint32_t(__fastcall*)(void*, void*))settingsTable[0x60 / 4])
            (settings, nullptr);

        control = reinterpret_cast<void*>(self->field_003c);
        uint32_t selection = 0x47453135u - (static_cast<uint8_t>(state) != 0);
        ((void(__fastcall*)(void*, void*, uint32_t))controlTable[0x2A4 / 4])
            (control, nullptr, selection);

        uint32_t disableArgument =
            CMenuGameOptions_GetRawDisableArgument(self, nullptr);
        control = reinterpret_cast<void*>(self->field_003c);
        ((int(__fastcall*)(void*, void*, uint32_t))
            CSF_AT(void**, control, 0)[0xA8 / 4])
            (control, nullptr, disableArgument);
    }

    if (self->field_0048 != 0)
    {
        uint32_t disableArgument =
            CMenuGameOptions_GetRawDisableArgument(self, nullptr);
        control = reinterpret_cast<void*>(self->field_0048);
        ((int(__fastcall*)(void*, void*, uint32_t))
            CSF_AT(void**, control, 0)[0xA8 / 4])
            (control, nullptr, disableArgument);
    }

    control = reinterpret_cast<void*>(self->field_0040);
    if (control != nullptr)
    {
        ((int(__fastcall*)(void*, void*, uint32_t))
            CSF_AT(void**, control, 0)[0xA8 / 4])
            (control, nullptr, 1);
    }

    control = reinterpret_cast<void*>(self->field_004c);
    if (control != nullptr)
    {
        ((int(__fastcall*)(void*, void*, uint32_t))
            CSF_AT(void**, control, 0)[0xA8 / 4])
            (control, nullptr, 1);
    }

    result = static_cast<uint32_t>(GetGlobalValue());
    if (CSF_AT(uint8_t, reinterpret_cast<void*>(result), 0xC1) == 0)
    {
        control = reinterpret_cast<void*>(self->field_0048);
        ((int(__fastcall*)(void*, void*, uint32_t))
            CSF_AT(void**, control, 0)[0xA8 / 4])
            (control, nullptr, 1);

        control = reinterpret_cast<void*>(self->field_003c);
        return ((int(__fastcall*)(void*, void*, uint32_t))
            CSF_AT(void**, control, 0)[0xA8 / 4])
            (control, nullptr, 1);
    }

    return static_cast<int>(result);
}

HOOK(0x0053B190, CMenuGameOptions_RefreshOptionControls, "auto");
