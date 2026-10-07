#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(std::uint32_t*, __cdecl, GuiEffect_Create, 0x00713B20, (int type));

// Creates a type-6 GUI effect, binds it to this control, configures its two
// integer parameters and its resource name (empty string when null) and finally
// registers the effect on the control through its virtual slot 0x234.
static std::uint32_t* __fastcall CGuiControl_CreateType6Effect(
    CGuiControl* self, void* /*edx*/,
    int firstParameter, int secondParameter, const char* resourceName)
{
    std::uint32_t* effect = GuiEffect_Create(6);
    if (!effect)
        return effect;

    ((void (__fastcall*)(void*, void*, CGuiControl*))
        CSF_AT(void**, effect, 0)[0x14 / 4])(effect, nullptr, self);

    ((void (__fastcall*)(void*, void*, int, int))
        CSF_AT(void**, effect, 0)[0x44 / 4])(
            effect, nullptr, firstParameter, secondParameter);

    const char* effectiveName = resourceName;
    if (!effectiveName)
        effectiveName = CSF_STR(0x0082E65C, "");

    ((void (__fastcall*)(void*, void*, const char*))
        CSF_AT(void**, effect, 0)[0x24 / 4])(
            effect, nullptr, effectiveName);

    return ((std::uint32_t* (__fastcall*)(CGuiControl*, void*, std::uint32_t*))
        self->vtbl[0x234 / 4])(self, nullptr, effect);
}

HOOK(0x00718B80, CGuiControl_CreateType6Effect, "auto");
