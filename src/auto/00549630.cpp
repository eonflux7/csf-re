#include <cstdint>
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "globals.h"

CSF_FUNC(void*, __cdecl, GetGlobalValue, 0x0075FDF0, ());
CSF_GLOBAL(const char*, g_MenuOptionsText, 0x00899ACC);
CSF_GLOBAL(const char*, g_MenuInGameOptionsText, 0x00899AD4);

static int __fastcall CMenuVideoOptions_UpdateOptionsMenu(
    CMenuVideoOptions* self, void* /*edx*/)
{
    void* globalState = GetGlobalValue();
    if (CSF_AT(std::uint8_t, globalState, 0xCC) != 0)
    {
        void* subordinate = reinterpret_cast<void*>(self->field_0020);
        if (subordinate)
        {
            ((void (__fastcall*)(void*, void*))
                CSF_AT(void**, subordinate, 0)[0x08 / 4])(
                    subordinate, nullptr);
        }
    }

    void* menu = reinterpret_cast<void*>(self->field_0014);
    ((void (__fastcall*)(void*, void*, CMenuVideoOptions*, int))
        CSF_AT(void**, menu, 0)[0x60 / 4])(menu, nullptr, self, 0);

    const int result = static_cast<int>(self->field_0018);
    if (result != 0)
        return result;

    const std::uint8_t useInGameText =
        ((std::uint8_t (__fastcall*)(void*, void*))
            CSF_AT(void**, self, 0)[0x30 / 4])(self, nullptr);

    const char* text = g_MenuOptionsText;
    if (useInGameText != 0)
        text = g_MenuInGameOptionsText;

    menu = reinterpret_cast<void*>(self->field_0014);
    return ((int (__fastcall*)(void*, void*, const char*))
        CSF_AT(void**, menu, 0)[0x58 / 4])(menu, nullptr, text);
}

HOOK(0x00549630, CMenuVideoOptions_UpdateOptionsMenu, "auto");
