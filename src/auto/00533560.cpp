#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, S_0041e5f0_GrowArrayCapacity, 0x0041E5F0, (S_0041e5f0* self, void* , int additionalCapacity));
CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());
CSF_FUNC(int, __fastcall, CMenuEula_RefreshTextControls, 0x005333B0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, CGuiSlider_ForwardSelfToParent, 0x00718660, (CGuiSlider* self, void*));

static int __fastcall CMenuEula_InitializeControls(
    CMenuEula* self, void* /*edx*/, int context, int /*unused*/)
{
    // Address classified as a vtable reference by the packet.
    (void)CSF_VTBL(0x0041E5F0);

    if (!self->field_001c)
    {
        void* state = reinterpret_cast<void*>(GetGlobalState_50D190());
        reinterpret_cast<void(__fastcall*)(void*, void*, CMenuEula*)>(
            CSF_AT(void**, state, 0)[0x18 / 4])(state, nullptr, self);
    }

    self->field_0038 = 0;
    self->field_0160 = 0;
    if (self->field_0160 >= CSF_AT(uint32_t, self, 0x15C))
    {
        S_0041e5f0_GrowArrayCapacity(
            reinterpret_cast<S_0041e5f0*>(
                reinterpret_cast<uint8_t*>(self) + 0x158),
            nullptr, CSF_AT(int, self, 0x164));
    }

    uint32_t index = self->field_0160;
    uint32_t* entry = reinterpret_cast<uint32_t*>(self->field_0158) + index;
    self->field_0160 = index + 1;
    *entry = 0;

    void* window = reinterpret_cast<void*>(self->field_001c);
    self->field_0018 = 0;
    self->field_0014 = static_cast<uint32_t>(context);
    reinterpret_cast<void(__fastcall*)(void*, void*, int)>(
        CSF_AT(void**, window, 0)[0xEC / 4])(window, nullptr, 0);

    window = reinterpret_cast<void*>(self->field_001c);
    reinterpret_cast<void(__fastcall*)(void*, void*)>(
        CSF_AT(void**, window, 0)[0x16C / 4])(window, nullptr);

    window = reinterpret_cast<void*>(self->field_001c);
    uint32_t control = reinterpret_cast<uint32_t(__fastcall*)(void*, void*, const char*)>(
        CSF_AT(void**, window, 0)[0xFC / 4])(
            window, nullptr, CSF_STR(0x00841C58, "BotonAccept"));
    window = reinterpret_cast<void*>(self->field_001c);
    self->field_0024 = control;

    control = reinterpret_cast<uint32_t(__fastcall*)(void*, void*, const char*)>(
        CSF_AT(void**, window, 0)[0xFC / 4])(
            window, nullptr, CSF_STR(0x00841C48, "BotonDecline"));
    self->field_0028 = control;
    if (control)
        CGuiSlider_ForwardSelfToParent(
            reinterpret_cast<CGuiSlider*>(control), nullptr);

    window = reinterpret_cast<void*>(self->field_001c);
    control = reinterpret_cast<uint32_t(__fastcall*)(void*, void*, const char*)>(
        CSF_AT(void**, window, 0)[0xFC / 4])(
            window, nullptr, CSF_STR(0x00841C3C, "BotonPrev"));
    window = reinterpret_cast<void*>(self->field_001c);
    self->field_002c = control;

    control = reinterpret_cast<uint32_t(__fastcall*)(void*, void*, const char*)>(
        CSF_AT(void**, window, 0)[0xFC / 4])(
            window, nullptr, CSF_STR(0x00841C30, "BotonNext"));
    window = reinterpret_cast<void*>(self->field_001c);
    self->field_0030 = control;

    self->m_textControl =
        reinterpret_cast<uint32_t(__fastcall*)(void*, void*, const char*)>(
            CSF_AT(void**, window, 0)[0xFC / 4])(
                window, nullptr, CSF_STR(0x00841C1C, "StaticChorongoEIDOS"));

    CMenuEula_RefreshTextControls(self, nullptr);
    return static_cast<int>(self->field_001c);
}

HOOK(0x00533560, CMenuEula_InitializeControls, "auto");
