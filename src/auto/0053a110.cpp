#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Menu-pause button-id globals (pointer values stored at these addresses are passed as-is).
CSF_GLOBAL(const char*, g_pszMenu_P_OptIng, 0x00899AD4);
CSF_GLOBAL(const char*, g_pszMenu_P_LoGame, 0x00899B08);
CSF_GLOBAL(const char*, g_pszMenu_P_LbFrI2, 0x00899B60);

// Original 0x00539D10 (sub_539D10): __thiscall, one stack argument, ret 4.
CSF_FUNC(int, __fastcall, CMenuPause_CloseAndReturnResult, 0x00539D10, (CMenuPause* self, void* edx, int actionCode));

// Dispatches a button event (identified by the id stored at +4 of a2) to the matching
// CMenuPause action. Every comparison re-reads both the menu field and a2+4, and every
// host-object access re-reads field_0010, as the original does.
static int __fastcall CMenuPause_DispatchButtonEvent(CMenuPause* self, void* /*edx*/, int a2)
{
    if (self->field_0010 == 0)
        return 0;

    // Continue button: base handler with action code 1.
    if (self->field_0020 != 0 && self->field_0020 == CSF_AT(uint32_t, a2, 4))
        return CMenuPause_CloseAndReturnResult(self, nullptr, 1);

    // Virtual slot 19 (+0x4C) on self, no arguments.
    if (self->field_0024 != 0 && self->field_0024 == CSF_AT(uint32_t, a2, 4))
        return ((int(__fastcall*)(void*, void*))CSF_AT(void**, self, 0)[19])(self, nullptr);

    // Host: vtbl[24](self, 0), then vtbl[22](LoGame).
    if (self->field_0028 != 0 && self->field_0028 == CSF_AT(uint32_t, a2, 4))
    {
        void* host = reinterpret_cast<void*>(self->field_0010);
        ((void(__fastcall*)(void*, void*, CMenuPause*, int))CSF_AT(void**, host, 0)[24])(host, nullptr, self, 0);
        host = reinterpret_cast<void*>(self->field_0010);
        return ((int(__fastcall*)(void*, void*, const char*))CSF_AT(void**, host, 0)[22])(host, nullptr, g_pszMenu_P_LoGame);
    }

    // Virtual slot 17 (+0x44) on self, no arguments.
    if (self->field_002c != 0 && self->field_002c == CSF_AT(uint32_t, a2, 4))
        return ((int(__fastcall*)(void*, void*))CSF_AT(void**, self, 0)[17])(self, nullptr);

    // Host: vtbl[24](self, 0), then vtbl[22](OptIng).
    if (self->field_0030 != 0 && self->field_0030 == CSF_AT(uint32_t, a2, 4))
    {
        void* host = reinterpret_cast<void*>(self->field_0010);
        ((void(__fastcall*)(void*, void*, CMenuPause*, int))CSF_AT(void**, host, 0)[24])(host, nullptr, self, 0);
        host = reinterpret_cast<void*>(self->field_0010);
        return ((int(__fastcall*)(void*, void*, const char*))CSF_AT(void**, host, 0)[22])(host, nullptr, g_pszMenu_P_OptIng);
    }

    // Host: vtbl[24](self, 0); child = vtbl[23](LbFrI2); child->byte[+0x188] = 1; then vtbl[22](LbFrI2).
    if (self->field_001c != 0 && self->field_001c == CSF_AT(uint32_t, a2, 4))
    {
        void* host = reinterpret_cast<void*>(self->field_0010);
        ((void(__fastcall*)(void*, void*, CMenuPause*, int))CSF_AT(void**, host, 0)[24])(host, nullptr, self, 0);
        host = reinterpret_cast<void*>(self->field_0010);
        void* childObject = ((void*(__fastcall*)(void*, void*, const char*))CSF_AT(void**, host, 0)[23])(host, nullptr, g_pszMenu_P_LbFrI2);
        CSF_AT(uint8_t, childObject, 0x188) = 1;
        host = reinterpret_cast<void*>(self->field_0010);
        return ((int(__fastcall*)(void*, void*, const char*))CSF_AT(void**, host, 0)[22])(host, nullptr, g_pszMenu_P_LbFrI2);
    }

    // Exit-style button: virtual slot 18 (+0x48) on self, no arguments.
    // Falls through to returning the field_0034 value when it does not match.
    int result = static_cast<int>(self->field_0034);
    if (result != 0 && static_cast<uint32_t>(result) == CSF_AT(uint32_t, a2, 4))
        return ((int(__fastcall*)(void*, void*))CSF_AT(void**, self, 0)[18])(self, nullptr);

    return result;
}

HOOK(0x0053A110, CMenuPause_DispatchButtonEvent, "auto");
