#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Menu label setter exposed through vtable slot 0x2C / 4 = 11 on the menu object.
// Stack args: (uint32_t value, const char* label); returns the object's result in eax.
using MenuLabelSetFn = uint8_t* (__fastcall*)(void* self, void* edx, uint32_t value, const char* label);

CSF_GLOBAL(const char*, g_pszMenu_P_MeJuMP, 0x899AF8);
CSF_GLOBAL(const char*, g_pszMenu_P_CharMP, 0x899AFC);
CSF_GLOBAL(const char*, g_pszMenu_P_EstaMP, 0x899B18);
CSF_GLOBAL(const char*, g_pszMenu_P_VictMP, 0x899B1C);
CSF_GLOBAL(const char*, g_pszMenu_P_TmPart, 0x899B20);

// Pushes the multiplayer menu labels to the menu object when its flag byte (+0xD0) is set.
// Returns the menu object pointer when the flag is clear, otherwise the result of the last label call.
static uint8_t* __fastcall C_00840754_SetMultiplayerMenuLabels(C_00840754* self, void* /*edx*/) {
    uint8_t* result = reinterpret_cast<uint8_t*>(self->field_0004);
    if (CSF_AT(uint8_t, result, 0xD0)) {
        reinterpret_cast<MenuLabelSetFn>(CSF_AT(void**, result, 0)[11])(
            result, nullptr, self->field_006c, g_pszMenu_P_MeJuMP);

        reinterpret_cast<MenuLabelSetFn>(CSF_AT(void**, reinterpret_cast<uint8_t*>(self->field_0004), 0)[11])(
            reinterpret_cast<void*>(self->field_0004), nullptr, self->field_0070, g_pszMenu_P_CharMP);

        reinterpret_cast<MenuLabelSetFn>(CSF_AT(void**, reinterpret_cast<uint8_t*>(self->field_0004), 0)[11])(
            reinterpret_cast<void*>(self->field_0004), nullptr, self->field_0048, g_pszMenu_P_TmPart);

        reinterpret_cast<MenuLabelSetFn>(CSF_AT(void**, reinterpret_cast<uint8_t*>(self->field_0004), 0)[11])(
            reinterpret_cast<void*>(self->field_0004), nullptr, self->field_004c, g_pszMenu_P_EstaMP);

        return reinterpret_cast<MenuLabelSetFn>(CSF_AT(void**, reinterpret_cast<uint8_t*>(self->field_0004), 0)[11])(
            reinterpret_cast<void*>(self->field_0004), nullptr, self->field_0050, g_pszMenu_P_VictMP);
    }
    return result;
}

HOOK(0x0050A010, C_00840754_SetMultiplayerMenuLabels, "auto");
