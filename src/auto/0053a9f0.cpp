#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, GetGlobalValue_8B056C, 0x00506BD0, ());
CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());
CSF_FUNC(int, __cdecl, GetGlobalValue_521BB0, 0x00521BB0, ());
CSF_FUNC(void, __fastcall, MenuProgressInput_UpdateGlobalState, 0x00521DE0, (void* self, void* edx));
CSF_FUNC(int, __stdcall, OnlineMenu_SetSharedText, 0x00521E30, (int a1));
CSF_FUNC(int, __fastcall, CMenuMain_UpdateLayoutAlignment, 0x0053A7D0, (CMenuMain* self, void*));
CSF_FUNC(int, __cdecl, GetGlobalValue_8BC1E0, 0x00756A10, ());
CSF_FUNC(int, __cdecl, GetGlobalValue_75FDF0, 0x0075FDF0, ());

static int __fastcall CMenuMain_InitializeScreen(
    CMenuMain* self, void* /*edx*/, int owner, int /*unused*/)
{
    if (!self->field_0018)
    {
        void* state = reinterpret_cast<void*>(GetGlobalState_50D190());
        reinterpret_cast<void(__fastcall*)(void*, void*, CMenuMain*)>(
            CSF_AT(void**, state, 0)[0x18 / 4])(state, nullptr, self);
    }

    self->vResetFields();
    self->field_0010 = owner;

    void* progress = reinterpret_cast<void*>(GetGlobalValue_521BB0());
    MenuProgressInput_UpdateGlobalState(progress, nullptr);

    void* screen = reinterpret_cast<void*>(self->field_0018);
    reinterpret_cast<void(__fastcall*)(void*, void*)>(
        CSF_AT(void**, screen, 0)[0x16C / 4])(screen, nullptr);

    screen = reinterpret_cast<void*>(self->field_0018);
    self->field_001c =
        reinterpret_cast<uint32_t(__fastcall*)(void*, void*, const char*)>(
            CSF_AT(void**, screen, 0)[0xFC / 4])(
            screen, nullptr, CSF_STR(0x008425C8, "BotonCampanya"));

    void* settings = reinterpret_cast<void*>(GetGlobalValue_75FDF0());
    if (!CSF_AT(uint8_t, settings, 0xC0))
    {
        void* campaignButton = reinterpret_cast<void*>(self->field_001c);
        if (campaignButton)
        {
            reinterpret_cast<void(__fastcall*)(void*, void*, int, int, int)>(
                CSF_AT(void**, campaignButton, 0)[0x68 / 4])(
                campaignButton, nullptr, 0, 1, 1);
        }
    }

    screen = reinterpret_cast<void*>(self->field_0018);
    self->field_0020 =
        reinterpret_cast<uint32_t(__fastcall*)(void*, void*, const char*)>(
            CSF_AT(void**, screen, 0)[0xFC / 4])(
            screen, nullptr, CSF_STR(0x008425B4, "BotonMultiPlayer"));

    if (self->field_0020)
    {
        settings = reinterpret_cast<void*>(GetGlobalValue_75FDF0());
        int multiplayerEnabled = CSF_AT(uint8_t, settings, 0xCC);
        void* multiplayerButton = reinterpret_cast<void*>(self->field_0020);
        reinterpret_cast<void(__fastcall*)(void*, void*, int, int, int)>(
            CSF_AT(void**, multiplayerButton, 0)[0x68 / 4])(
            multiplayerButton, nullptr, multiplayerEnabled, 1, 1);

        settings = reinterpret_cast<void*>(GetGlobalValue_75FDF0());
        if (!CSF_AT(uint8_t, settings, 0xC1))
        {
            multiplayerButton = reinterpret_cast<void*>(self->field_0020);
            reinterpret_cast<void(__fastcall*)(void*, void*, int, int, int)>(
                CSF_AT(void**, multiplayerButton, 0)[0x68 / 4])(
                multiplayerButton, nullptr, 0, 1, 1);
        }
    }

    screen = reinterpret_cast<void*>(self->field_0018);
    uint32_t optionsButton =
        reinterpret_cast<uint32_t(__fastcall*)(void*, void*, const char*)>(
            CSF_AT(void**, screen, 0)[0xFC / 4])(
            screen, nullptr, CSF_STR(0x00841DEC, "BotonOpciones"));
    screen = reinterpret_cast<void*>(self->field_0018);
    self->field_0024 = optionsButton;

    self->field_0028 =
        reinterpret_cast<uint32_t(__fastcall*)(void*, void*, const char*)>(
            CSF_AT(void**, screen, 0)[0xFC / 4])(
            screen, nullptr, CSF_STR(0x008425A4, "BotonCreditos"));

    if (self->field_0028)
    {
        int settingsAddress = GetGlobalValue_75FDF0();
        bool creditsEnabled =
            CSF_AT(uint8_t, reinterpret_cast<void*>(settingsAddress), 0xF0) == 0;
        // The original replaces only the low byte of this argument.
        int creditsArgument = static_cast<int>(
            (static_cast<uint32_t>(settingsAddress) & ~uint32_t(0xFF)) |
            static_cast<uint32_t>(creditsEnabled));

        void* creditsButton = reinterpret_cast<void*>(self->field_0028);
        reinterpret_cast<void(__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, creditsButton, 0)[0xA8 / 4])(
            creditsButton, nullptr, creditsArgument);
    }

    reinterpret_cast<void(__fastcall*)(CMenuMain*, void*)>(
        CSF_AT(void**, self, 0)[0x48 / 4])(self, nullptr);
    CMenuMain_UpdateLayoutAlignment(self, nullptr);

    void* state = reinterpret_cast<void*>(GetGlobalState_50D190());
    void* stateComponent = CSF_AT(void*, state, 0x10);
    reinterpret_cast<void(__fastcall*)(void*, void*)>(
        CSF_AT(void**, stateComponent, 0)[0x8 / 4])(stateComponent, nullptr);

    state = reinterpret_cast<void*>(GetGlobalState_50D190());
    CSF_AT(uint8_t, state, 0xD0) = 0;

    void* globalManager = reinterpret_cast<void*>(GetGlobalValue_8B056C());
    void* managerComponent = CSF_AT(void*, globalManager, 0x14);
    reinterpret_cast<void(__fastcall*)(void*, void*)>(
        CSF_AT(void**, managerComponent, 0)[0x14 / 4])(
        managerComponent, nullptr);

    void* onlineState = reinterpret_cast<void*>(GetGlobalValue_8BC1E0());
    uint32_t textCapacity = CSF_AT(uint32_t, onlineState, 0xF9C);
    uint8_t* sharedString = static_cast<uint8_t*>(onlineState) + 0xF84;
    char* text;
    if (textCapacity < 0x10)
        text = reinterpret_cast<char*>(sharedString + 4);
    else
        text = CSF_AT(char*, sharedString, 0x4);

    GetGlobalValue_521BB0();
    OnlineMenu_SetSharedText(reinterpret_cast<int>(text));

    void* ownerObject = reinterpret_cast<void*>(self->field_0010);
    reinterpret_cast<void(__fastcall*)(void*, void*, int)>(
        CSF_AT(void**, ownerObject, 0)[0x9C / 4])(ownerObject, nullptr, 1);

    ownerObject = reinterpret_cast<void*>(self->field_0010);
    reinterpret_cast<void(__fastcall*)(void*, void*)>(
        CSF_AT(void**, ownerObject, 0)[0xA8 / 4])(ownerObject, nullptr);

    reinterpret_cast<void(__fastcall*)(CMenuMain*, void*)>(
        CSF_AT(void**, self, 0)[0x50 / 4])(self, nullptr);

    return static_cast<int>(self->field_0018);
}

HOOK(0x0053A9F0, CMenuMain_InitializeScreen, "auto");
