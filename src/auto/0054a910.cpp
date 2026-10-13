#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, GetGlobalValue_521BB0, 0x00521BB0, ());
CSF_FUNC(void, __fastcall, MenuProgressInput_UpdateGlobalState, 0x00521DE0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, CMenuOptions_ApplyTargetOffset, 0x0053AD90, (CMenuOptions* self, void*));
CSF_FUNC(int, __fastcall, CMenuVideoOptions_PopulateResolutionChoices, 0x00549580, (CMenuVideoOptions* self, void*));
CSF_FUNC(int, __fastcall, CMenuVideoOptions_FillSettingsSnapshot, 0x00549760, (CMenuVideoOptions* self, void*, uint8_t* outSnapshot));
CSF_FUNC(int, __fastcall, CMenuVideoOptions_PopulateDisplayModes, 0x0054A690, (void* self, void* edx));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));
CSF_FUNC(void*, __cdecl, InitStringFromLiteral, 0x00692720, (void* returnSlot, const char* text, int count));
CSF_FUNC(int, __cdecl, GetGlobalValue_75FDF0, 0x0075FDF0, ());

static int __fastcall CMenuVideoOptions_InitializeControls(
    CMenuVideoOptions* self, void* /*edx*/, int parent, int /*unused*/)
{
    CMenuVideoOptions_FillSettingsSnapshot(
        self, nullptr, reinterpret_cast<uint8_t*>(&self->field_0028));

    self->field_0014 = 0;
    self->field_0078 = 0;
    self->field_0018 = 0;
    self->field_0058 = 0;
    self->field_005c = 0;
    self->field_006c = 0;
    self->field_0060 = 0;
    self->field_0064 = 0;
    self->field_0068 = 0;
    self->field_007c = 0;
    self->field_0070 = 0;
    self->vClearField0010();

    void* control = reinterpret_cast<void*>(self->field_001c);
    self->field_0014 = parent;
    reinterpret_cast<void(__fastcall*)(void*, void*)>(
        CSF_AT(void**, control, 0)[364 / 4])(control, nullptr);

    MenuProgressInput_UpdateGlobalState(
        reinterpret_cast<void*>(GetGlobalValue_521BB0()), nullptr);

    control = reinterpret_cast<void*>(self->field_001c);
    self->field_006c = reinterpret_cast<uint32_t(__fastcall*)(void*, void*, const char*)>(
        CSF_AT(void**, control, 0)[252 / 4])(
            control, nullptr, CSF_STR(0x008441E8, "Noise"));
    if (self->field_006c)
    {
        control = reinterpret_cast<void*>(self->field_006c);
        reinterpret_cast<void(__fastcall*)(void*, void*)>(
            CSF_AT(void**, control, 0)[696 / 4])(control, nullptr);

        control = reinterpret_cast<void*>(self->field_006c);
        reinterpret_cast<void(__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, control, 0)[668 / 4])(control, nullptr, 1397698864);

        control = reinterpret_cast<void*>(self->field_006c);
        reinterpret_cast<void(__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, control, 0)[668 / 4])(control, nullptr, 1397698865);

        control = reinterpret_cast<void*>(self->field_006c);
        reinterpret_cast<void(__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, control, 0)[676 / 4])(
                control, nullptr, 1397698865 - (self->field_0038 != 0));
    }

    control = reinterpret_cast<void*>(self->field_001c);
    self->field_0058 = reinterpret_cast<uint32_t(__fastcall*)(void*, void*, const char*)>(
        CSF_AT(void**, control, 0)[252 / 4])(
            control, nullptr, CSF_STR(0x008441E0, "Quality"));
    if (self->field_0058)
    {
        void* settings = reinterpret_cast<void*>(GetGlobalValue_75FDF0());
        control = reinterpret_cast<void*>(self->field_0058);
        reinterpret_cast<void(__fastcall*)(void*, void*, int, int, int)>(
            CSF_AT(void**, control, 0)[104 / 4])(
                control, nullptr, CSF_AT(uint8_t, settings, 0xCC), 1, 1);
        CMenuVideoOptions_PopulateResolutionChoices(self, nullptr);
    }

    control = reinterpret_cast<void*>(self->field_001c);
    self->field_005c = reinterpret_cast<uint32_t(__fastcall*)(void*, void*, const char*)>(
        CSF_AT(void**, control, 0)[252 / 4])(
            control, nullptr, CSF_STR(0x0083F8EC, "Scan"));
    if (self->field_005c)
    {
        void* settings = reinterpret_cast<void*>(GetGlobalValue_75FDF0());
        control = reinterpret_cast<void*>(self->field_005c);
        reinterpret_cast<void(__fastcall*)(void*, void*, int, int, int)>(
            CSF_AT(void**, control, 0)[104 / 4])(
                control, nullptr, CSF_AT(uint8_t, settings, 0xCC), 1, 1);
    }

    control = reinterpret_cast<void*>(self->field_001c);
    self->field_0060 = reinterpret_cast<uint32_t(__fastcall*)(void*, void*, const char*)>(
        CSF_AT(void**, control, 0)[252 / 4])(
            control, nullptr, CSF_STR(0x008441D8, "Mode"));
    if (self->field_0060)
    {
        CMenuVideoOptions_PopulateDisplayModes(self, nullptr);
        void* settings = reinterpret_cast<void*>(GetGlobalValue_75FDF0());
        control = reinterpret_cast<void*>(self->field_0060);
        reinterpret_cast<void(__fastcall*)(void*, void*, int, int, int)>(
            CSF_AT(void**, control, 0)[104 / 4])(
                control, nullptr, CSF_AT(uint8_t, settings, 0xCC), 1, 1);
    }

    control = reinterpret_cast<void*>(self->field_001c);
    self->field_0064 = reinterpret_cast<uint32_t(__fastcall*)(void*, void*, const char*)>(
        CSF_AT(void**, control, 0)[252 / 4])(
            control, nullptr, CSF_STR(0x008441CC, "Brightness"));
    if (self->field_0064)
    {
        control = reinterpret_cast<void*>(self->field_0064);
        reinterpret_cast<void(__fastcall*)(void*, void*, int, int, int)>(
            CSF_AT(void**, control, 0)[608 / 4])(control, nullptr, 30, 1, 0);

        control = reinterpret_cast<void*>(self->field_0064);
        void** sliderVtable = CSF_AT(void**, control, 0);
        int steps = reinterpret_cast<int(__fastcall*)(void*, void*, int)>(
            sliderVtable[612 / 4])(control, nullptr, 0);
        int64_t position = static_cast<int64_t>((steps - 1) * self->field_0034);
        control = reinterpret_cast<void*>(self->field_0064);
        reinterpret_cast<void(__fastcall*)(void*, void*, int)>(
            sliderVtable[620 / 4])(control, nullptr, static_cast<int32_t>(position));

        control = reinterpret_cast<void*>(self->field_0064);
        int value = reinterpret_cast<int(__fastcall*)(void*, void*)>(
            CSF_AT(void**, control, 0)[580 / 4])(control, nullptr);
        control = reinterpret_cast<void*>(self->field_0064);
        reinterpret_cast<void(__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, control, 0)[344 / 4])(control, nullptr, value);
    }

    if (!reinterpret_cast<bool(__fastcall*)(void*, void*)>(
            CSF_AT(void**, self, 0)[0x30 / 4])(self, nullptr))
    {
        control = reinterpret_cast<void*>(self->field_001c);
        self->field_0068 = reinterpret_cast<uint32_t(__fastcall*)(void*, void*, const char*)>(
            CSF_AT(void**, control, 0)[252 / 4])(
                control, nullptr, CSF_STR(0x008441C0, "Advanced"));
    }

    control = reinterpret_cast<void*>(self->field_001c);
    self->field_0070 = reinterpret_cast<uint32_t(__fastcall*)(void*, void*, const char*)>(
        CSF_AT(void**, control, 0)[252 / 4])(
            control, nullptr, CSF_STR(0x008413D4, "BotonOkQuestion"));
    if (self->field_0070 &&
        !CSF_AT(uint8_t, reinterpret_cast<void*>(GetGlobalValue_75FDF0()), 0xCC))
    {
        control = reinterpret_cast<void*>(self->field_0070);
        void* label = reinterpret_cast<void*(__fastcall*)(void*, void*)>(
            CSF_AT(void**, control, 0)[272 / 4])(control, nullptr);

        // The string occupies the entire 0x1C-byte local region, including
        // its leading allocator word and its 0x18-byte storage/size/capacity.
        alignas(8) uint8_t textBytes[0x1C] = {};
        CSF_AT(uint32_t, textBytes, 0x18) = 7;
        CSF_AT(uint32_t, textBytes, 0x14) = 0;
        CSF_AT(uint16_t, textBytes, 0x4) = 0;
        InitStringFromLiteral(textBytes, CSF_STR(0x008427FC, "GE03"), 1);

        void* text = CSF_AT(uint32_t, textBytes, 0x18) < 8
            ? static_cast<void*>(textBytes + 0x4)
            : CSF_AT(void*, textBytes, 0x4);
        reinterpret_cast<void(__fastcall*)(void*, void*, void*)>(
            CSF_AT(void**, label, 0)[64 / 4])(label, nullptr, text);

        if (CSF_AT(uint32_t, textBytes, 0x18) >= 8 &&
            CSF_AT(void*, textBytes, 0x4))
        {
            Mem_FreeDebug(CSF_AT(void*, textBytes, 0x4));
        }
    }

    self->vSetBackButton(static_cast<int>(self->field_001c));
    CMenuOptions_ApplyTargetOffset(reinterpret_cast<CMenuOptions*>(self), nullptr);

    control = reinterpret_cast<void*>(self->field_0014);
    reinterpret_cast<void(__fastcall*)(void*, void*, int)>(
        CSF_AT(void**, control, 0)[156 / 4])(control, nullptr, 0);
    control = reinterpret_cast<void*>(self->field_0014);
    reinterpret_cast<void(__fastcall*)(void*, void*)>(
        CSF_AT(void**, control, 0)[168 / 4])(control, nullptr);

    return static_cast<int>(self->field_001c);
}

HOOK(0x0054A910, CMenuVideoOptions_InitializeControls, "auto");
