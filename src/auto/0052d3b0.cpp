#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());
CSF_FUNC(void, __fastcall, MenuManager_SetMenuState, 0x0050DC80, (void* self, void* edx, char flag));
CSF_FUNC(int, __cdecl, GetGlobalValue_521BB0, 0x00521BB0, ());
CSF_FUNC(void, __fastcall, MenuProgressInput_UpdateGlobalState, 0x00521DE0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, C_008414e8_TryLayoutFullProgressPanel, 0x0052D330, (C_008414e8* self, void*));
CSF_FUNC(int, __fastcall, CMenuOptions_ApplyTargetOffset, 0x0053AD90, (CMenuOptions* self, void*));

static int __fastcall C_008414e8_InitializeProgressPanel(
    C_008414e8* self, void* /*edx*/, int initialValue, int /*unused*/)
{
    self->field_0014 = 0;
    self->field_0040 = 0;
    self->field_0018 = 0;
    self->vClearField0010();

    self->field_0014 = initialValue;
    MenuProgressInput_UpdateGlobalState(
        reinterpret_cast<void*>(GetGlobalValue_521BB0()), nullptr);

    void* backButton = reinterpret_cast<void*>(self->field_001c);
    reinterpret_cast<void(__fastcall*)(void*, void*)>(
        CSF_AT(void**, backButton, 0)[0x16C / 4])(backButton, nullptr);

    self->vSetBackButton(static_cast<int>(self->field_001c));
    CMenuOptions_ApplyTargetOffset(reinterpret_cast<CMenuOptions*>(self), nullptr);

    void* manager = reinterpret_cast<void*>(GetGlobalState_50D190());
    MenuManager_SetMenuState(manager, nullptr, 0);
    C_008414e8_TryLayoutFullProgressPanel(self, nullptr);

    uint32_t* buttons = &self->field_0024;
    for (int remaining = 4; remaining; --remaining, ++buttons)
    {
        void* button = reinterpret_cast<void*>(*buttons);
        if (button)
        {
            reinterpret_cast<void(__fastcall*)(void*, void*, int)>(
                CSF_AT(void**, button, 0)[0xA8 / 4])(button, nullptr, 1);
        }
    }

    void* state = reinterpret_cast<void*>(GetGlobalState_50D190());
    void* activeObject = CSF_AT(void*, state, 0x74);
    if (activeObject)
    {
        reinterpret_cast<void(__fastcall*)(void*, void*)>(
            CSF_AT(void**, activeObject, 0)[0xC / 4])(activeObject, nullptr);
    }

    return static_cast<int>(self->field_001c);
}

HOOK(0x0052D3B0, C_008414e8_InitializeProgressPanel, "auto");
