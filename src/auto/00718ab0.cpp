#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "legacy/string.h"
#include "types/auto_classes.h"

CSF_FUNC(std::uint32_t*, __cdecl, GuiEffect_Create, 0x00713B20, (int type));

static std::uint32_t* __fastcall CGuiControl_CreateConfiguredEffect(
    CGuiControl* self, void* /*edx*/, int configurationValue, const char* text)
{
    std::uint32_t* effect = GuiEffect_Create(7);
    if (!effect)
        return effect;

    ((void(__fastcall*)(void*, void*, CGuiControl*))
        CSF_AT(void**, effect, 0)[0x14 / 4])(effect, nullptr, self);

    ((void(__fastcall*)(void*, void*, int))
        CSF_AT(void**, effect, 0)[0x48 / 4])(
            effect, nullptr, configurationValue);

    const char* effectiveText = text;
    if (!effectiveText)
        effectiveText = CSF_STR(0x0082E65C, "");

    ((void(__fastcall*)(void*, void*, const char*))
        CSF_AT(void**, effect, 0)[0x24 / 4])(
            effect, nullptr, effectiveText);

    return ((std::uint32_t*(__fastcall*)(void*, void*, std::uint32_t*))
        self->vtbl[0x234 / 4])(self, nullptr, effect);
}

HOOK(0x00718AB0, CGuiControl_CreateConfiguredEffect, "auto");
