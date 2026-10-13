#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, GetGlobalValue_521BB0, 0x00521BB0, ());
CSF_FUNC(void, __fastcall, MenuProgressInput_UpdateGlobalState, 0x00521DE0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, CMenuOptions_ApplyTargetOffset, 0x0053AD90, (CMenuOptions* self, void*));

static int __fastcall CMenuSoundOptions_InitializeControls(
    CMenuSoundOptions* self, void* /*edx*/, int owner, int /*unused*/)
{
    self->vResetAndInitialize();
    self->field_0014 = static_cast<uint32_t>(owner);

    void* controls = reinterpret_cast<void*>(self->field_001c);
    reinterpret_cast<void(__fastcall*)(void*, void*)>(
        CSF_AT(void**, controls, 0)[0x16c / 4])(controls, nullptr);

    void* globalState = reinterpret_cast<void*>(GetGlobalValue_521BB0());
    MenuProgressInput_UpdateGlobalState(globalState, nullptr);

    void* settings = reinterpret_cast<void*>(self->field_002c);
    if (settings)
    {
        self->field_0030 = static_cast<float>(
            reinterpret_cast<double(__fastcall*)(void*, void*)>(
                CSF_AT(void**, settings, 0)[0x18 / 4])(settings, nullptr));

        settings = reinterpret_cast<void*>(self->field_002c);
        self->field_0034 = static_cast<float>(
            reinterpret_cast<double(__fastcall*)(void*, void*)>(
                CSF_AT(void**, settings, 0)[0x20 / 4])(settings, nullptr));

        self->vStoreControlValue();
        self->vUpdateToggleFlagFromControl();
        reinterpret_cast<void(__fastcall*)(CMenuSoundOptions*, void*)>(
            self->vtbl[0x68 / 4])(self, nullptr);
    }

    void* effectsSlider = reinterpret_cast<void*>(self->field_0040);
    if (effectsSlider)
    {
        reinterpret_cast<void(__fastcall*)(void*, void*, int, int, int)>(
            CSF_AT(void**, effectsSlider, 0)[0x260 / 4])(
                effectsSlider, nullptr, 30, 1, 0);

        effectsSlider = reinterpret_cast<void*>(self->field_0040);
        void** sliderVtable = CSF_AT(void**, effectsSlider, 0);
        int itemCount = reinterpret_cast<int(__fastcall*)(void*, void*, int)>(
            sliderVtable[0x264 / 4])(effectsSlider, nullptr, 0);

        int selection = static_cast<int>(
            static_cast<int64_t>(static_cast<double>(itemCount - 1) * static_cast<double>(self->field_0030)));

        effectsSlider = reinterpret_cast<void*>(self->field_0040);
        reinterpret_cast<void(__fastcall*)(void*, void*, int)>(
            sliderVtable[0x26c / 4])(effectsSlider, nullptr, selection);
    }

    void* musicSlider = reinterpret_cast<void*>(self->field_0044);
    if (musicSlider)
    {
        reinterpret_cast<void(__fastcall*)(void*, void*, int, int, int)>(
            CSF_AT(void**, musicSlider, 0)[0x260 / 4])(
                musicSlider, nullptr, 30, 1, 0);

        musicSlider = reinterpret_cast<void*>(self->field_0044);
        void** sliderVtable = CSF_AT(void**, musicSlider, 0);
        int itemCount = reinterpret_cast<int(__fastcall*)(void*, void*, int)>(
            sliderVtable[0x264 / 4])(musicSlider, nullptr, 0);

        int selection = static_cast<int>(
            static_cast<int64_t>(static_cast<double>(itemCount - 1) * static_cast<double>(self->field_0034)));

        musicSlider = reinterpret_cast<void*>(self->field_0044);
        reinterpret_cast<void(__fastcall*)(void*, void*, int)>(
            sliderVtable[0x26c / 4])(musicSlider, nullptr, selection);
    }

    self->vSendOkQuestionGE03();
    self->vSetBackButton(static_cast<int>(self->field_001c));
    self->vInitializeAcousticRadioGroup(static_cast<int>(self->field_001c));
    self->vInitializeSoundSwitch(reinterpret_cast<void*>(self->field_001c));

    reinterpret_cast<void(__fastcall*)(CMenuSoundOptions*, void*, uint32_t)>(
        self->vtbl[0x8c / 4])(self, nullptr, self->field_001c);

    CMenuOptions_ApplyTargetOffset(reinterpret_cast<CMenuOptions*>(self), nullptr);

    void* ownerObject = reinterpret_cast<void*>(self->field_0014);
    reinterpret_cast<void(__fastcall*)(void*, void*, int)>(
        CSF_AT(void**, ownerObject, 0)[0x9c / 4])(ownerObject, nullptr, 0);

    ownerObject = reinterpret_cast<void*>(self->field_0014);
    reinterpret_cast<void(__fastcall*)(void*, void*)>(
        CSF_AT(void**, ownerObject, 0)[0xa8 / 4])(ownerObject, nullptr);

    return static_cast<int>(self->field_001c);
}

HOOK(0x0053CDA0, CMenuSoundOptions_InitializeControls, "auto");
