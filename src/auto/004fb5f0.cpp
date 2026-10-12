#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(char, __fastcall, C_00830578_ResetMenuController, 0x0042CAB0, (C_00830578* self, void*));
CSF_FUNC(char, __fastcall, S_004faea0_OpenMenu, 0x004FAEA0, (void* self, void* edx, int a2, wchar_t* Source));
CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());
CSF_FUNC(int, __cdecl, Text_ConvertToDisplayEncoding, 0x00692580, (int a1, int a2));
CSF_FUNC(int, __fastcall, DevConsole_SetMenuState, 0x00712CE0, (void* self, void* edx, int a2));

CSF_GLOBAL(int, g_globalValue_8B0560, 0x008B0560);

// The packet labels the direct OpenMenu callee as a vtable reference.
static const auto kMenuEntryBinding = CSF_VTBL(0x004FAEA0);

static char __fastcall S_004faea0_ApplyFlagReset(
    S_004faea0* self, void* /*edx*/, char enabled)
{
    self->field_0010 = enabled;

    if (enabled)
    {
        self->field_0018 = 0;
        self->field_0144 = 0;

        void* state = reinterpret_cast<void*>(GetGlobalState_50D190());
        C_00830578_ResetMenuController(
            CSF_AT(C_00830578*, state, 0x18), nullptr);

        state = reinterpret_cast<void*>(GetGlobalState_50D190());
        void* controller = CSF_AT(void*, state, 0x18);
        reinterpret_cast<void (__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, controller, 0)[0x10 / 4])(
                controller, nullptr, 0);

        state = reinterpret_cast<void*>(GetGlobalState_50D190());
        self->field_000c =
            CSF_AT(uint32_t, CSF_AT(void*, state, 0x10), 0x18);

        state = reinterpret_cast<void*>(GetGlobalState_50D190());
        DevConsole_SetMenuState(
            CSF_AT(void*, state, 0x10), nullptr, g_globalValue_8B0560);

        const uint32_t menu = self->field_0174;
        if (menu)
            return static_cast<char>(menu);

        wchar_t* displayName = reinterpret_cast<wchar_t*>(
            Text_ConvertToDisplayEncoding(
                reinterpret_cast<int>(CSF_STR(0x0083F340, "CS01")), 16));
        return S_004faea0_OpenMenu(self, nullptr, 2, displayName);
    }

    void* state = reinterpret_cast<void*>(GetGlobalState_50D190());
    const int savedState = static_cast<int>(self->field_000c);
    return static_cast<char>(DevConsole_SetMenuState(
        CSF_AT(void*, state, 0x10), nullptr, savedState));
}

HOOK(0x004FB5F0, S_004faea0_ApplyFlagReset, "auto");
