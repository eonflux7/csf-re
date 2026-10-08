#include <cstdint>
#include "hook.h"
#include "types/auto_classes.h"
#include "globals.h"

// Original callees, bound by address.
CSF_FUNC(int, __cdecl, GetGlobalValue, 0x00521BB0, ());
CSF_FUNC(void, __fastcall, MenuVideo_ApplyGlobalValue, 0x00521DE0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, CMenuVideoAdvanced_RefreshCachedSettings, 0x0053AD90, (void* self, void* edx));
CSF_FUNC(int, __fastcall, CMenuVideoAdvanced_ResetOptionState, 0x00548D20, (void* self, void* edx));
CSF_FUNC(int, __fastcall, CMenuVideoAdvanced_PushSettingsToWidgets, 0x00549040, (void* self, void* edx));

// Resets the advanced video state, stores the initial value, runs the virtual
// initialization chain, and returns the settings object at +0x1C.
static int __fastcall CMenuVideoAdvanced_InitializeAndLoadSettings(CMenuVideoAdvanced* self, void* /*edx*/, int initialValue, int /*unusedArg*/)
{
    self->field_0014 = 0;
    self->field_004c = 0;
    self->field_0018 = 0;
    self->field_0028 = 0;
    self->field_002c = 0;
    self->field_0030 = 0;
    self->field_0034 = 0;
    self->field_0038 = 0;
    self->field_003c = 0;
    self->field_0040 = 0;
    self->field_0044 = 0;

    // Virtual slot 0x40 (16), no arguments.
    reinterpret_cast<void(__fastcall*)(void*, void*)>(CSF_AT(void**, self, 0)[16])(self, nullptr);

    self->field_0014 = static_cast<uint32_t>(initialValue);

    void* settingsObject = reinterpret_cast<void*>(static_cast<std::uintptr_t>(self->field_001c));
    // Virtual slot 0x16C (91) on the settings object, no arguments.
    reinterpret_cast<void(__fastcall*)(void*, void*)>(CSF_AT(void**, settingsObject, 0)[91])(settingsObject, nullptr);

    CMenuVideoAdvanced_ResetOptionState(self, nullptr);

    int globalValue = GetGlobalValue();
    MenuVideo_ApplyGlobalValue(reinterpret_cast<void*>(static_cast<std::uintptr_t>(globalValue)), nullptr);

    // Virtual slot 0x44 (17) on self, with the settings object as its single stack argument.
    reinterpret_cast<void(__fastcall*)(void*, void*, int)>(CSF_AT(void**, self, 0)[17])(
        self, nullptr, static_cast<int>(self->field_001c));

    CMenuVideoAdvanced_RefreshCachedSettings(self, nullptr);
    CMenuVideoAdvanced_PushSettingsToWidgets(self, nullptr);

    return static_cast<int>(self->field_001c);
}

HOOK(0x005493D0, CMenuVideoAdvanced_InitializeAndLoadSettings, "auto");
