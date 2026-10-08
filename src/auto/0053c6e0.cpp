#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_GLOBAL(uint32_t, g_ScreenWidth, 0x00899B9C);
CSF_GLOBAL(uint32_t, g_ScreenHeight, 0x00899BA0);

CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(void*, __fastcall, LookupObjectById, 0x00652160,
         (int handle, void* edx));
CSF_FUNC(uint8_t*, __fastcall, LookupNamedElement, 0x00693360,
         (void* table, void* edx, const char* name));

static int __fastcall CMenuSoundOptions_InitPanelAndSliders(
    CMenuSoundOptions* self, void* /*edx*/,
    void* ownerObject, void* tableHolder)
{
    const uint32_t cachedPanel = self->field_001c;
    if (cachedPanel != 0) {
        return static_cast<int>(cachedPanel);
    }

    void* rootObject = reinterpret_cast<void* (__fastcall*)(void*, void*)>(
        CSF_AT(void**, ownerObject, 0)[0x54 / 4])(ownerObject, nullptr);

    // This vtable is loaded before the name lookup in the original.
    void** rootVtable = CSF_AT(void**, rootObject, 0);
    uint8_t* panelEntry = LookupNamedElement(
        *static_cast<void**>(tableHolder), nullptr,
        CSF_STR(0x00841304, ".PANEL_PRINCIPAL"));

    void* panel = reinterpret_cast<void* (__fastcall*)(void*, void*, void*)>(
        rootVtable[0x18 / 4])(rootObject, nullptr, panelEntry);
    self->field_001c = reinterpret_cast<uint32_t>(panel);
    if (panel == nullptr) {
        return 0;
    }

    void* xHandleObject = LookupObjectById(GetGlobalInt(), nullptr);
    void* xResult = reinterpret_cast<void* (__fastcall*)(void*, void*)>(
        CSF_AT(void**, xHandleObject, 0)[0x18 / 4])(xHandleObject, nullptr);
    const uint32_t xRawBits = CSF_AT(uint32_t, xResult, 0);

    void* yHandleObject = LookupObjectById(GetGlobalInt(), nullptr);
    void* yResult = reinterpret_cast<void* (__fastcall*)(void*, void*)>(
        CSF_AT(void**, yHandleObject, 0)[0x18 / 4])(yHandleObject, nullptr);
    const uint32_t yRawBits = CSF_AT(uint32_t, yResult, 0x04);

    // panel position as a fraction of the screen (unsigned ints divided as floats)
    const float yRatio = static_cast<float>(yRawBits) / static_cast<float>(g_ScreenHeight);
    const float xRatio = static_cast<float>(xRawBits) / static_cast<float>(g_ScreenWidth);

    void* panelForPlacement = reinterpret_cast<void*>(self->field_001c);
    reinterpret_cast<void (__fastcall*)(void*, void*, float, float)>(
        CSF_AT(void**, panelForPlacement, 0)[0x150 / 4])(
            panelForPlacement, nullptr, xRatio, yRatio);

    void* panelForMusic = reinterpret_cast<void*>(self->field_001c);
    void* musicSlider =
        reinterpret_cast<void* (__fastcall*)(void*, void*, const char*)>(
            CSF_AT(void**, panelForMusic, 0)[0xFC / 4])(
                panelForMusic, nullptr, CSF_STR(0x00842A58, "SliderMusic"));
    self->field_0040 = reinterpret_cast<uint32_t>(musicSlider);
    if (musicSlider != nullptr) {
        int musicValue = reinterpret_cast<int (__fastcall*)(void*, void*)>(
            CSF_AT(void**, musicSlider, 0)[0x244 / 4])(musicSlider, nullptr);
        void* musicSliderNow = reinterpret_cast<void*>(self->field_0040);
        reinterpret_cast<void (__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, musicSliderNow, 0)[0x158 / 4])(
                musicSliderNow, nullptr, musicValue);
    }

    void* panelForFx = reinterpret_cast<void*>(self->field_001c);
    void* fxSlider =
        reinterpret_cast<void* (__fastcall*)(void*, void*, const char*)>(
            CSF_AT(void**, panelForFx, 0)[0xFC / 4])(
                panelForFx, nullptr, CSF_STR(0x00842A4C, "SliderFX"));
    self->field_0044 = reinterpret_cast<uint32_t>(fxSlider);
    if (fxSlider == nullptr) {
        return 0;
    }

    int fxValue = reinterpret_cast<int (__fastcall*)(void*, void*)>(
        CSF_AT(void**, fxSlider, 0)[0x244 / 4])(fxSlider, nullptr);
    void* fxSliderNow = reinterpret_cast<void*>(self->field_0044);
    return reinterpret_cast<int (__fastcall*)(void*, void*, int)>(
        CSF_AT(void**, fxSliderNow, 0)[0x158 / 4])(
            fxSliderNow, nullptr, fxValue);
}

HOOK(0x0053C6E0, CMenuSoundOptions_InitPanelAndSliders, "auto");
