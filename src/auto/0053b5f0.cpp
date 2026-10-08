#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// Menu label strings: the globals hold pointers to the menu key names.
CSF_GLOBAL(const char*, g_pszMenu_P_Option, 0x00899ACC);
CSF_GLOBAL(const char*, g_pszMenu_P_OptIng, 0x00899AD4);

// Refreshes the toggle button label on a game options menu. The label is chosen by
// the boolean returned from the screen's virtual slot 0x30; the result of the final
// virtual call is returned unchanged.
static int __fastcall CMenuGameOptions_UpdateToggleButtonLabel(CMenuGameOptions* self, void* /*edx*/)
{
    // self->vtbl[0x54]()  -- refresh / prepare step, no arguments beyond this.
    ((void(__fastcall*)(void*, void*))CSF_AT(void**, self, 0)[0x54 / 4])(self, nullptr);

    // Sub-object at +0x18: obj->vtbl[0x60](obj, self, 0)
    void* subObject = reinterpret_cast<void*>(self->field_0018);
    ((void(__fastcall*)(void*, void*, void*, int))CSF_AT(void**, subObject, 0)[0x60 / 4])(
        subObject, nullptr, static_cast<void*>(self), 0);

    // self->vtbl[0x30]() -> bool; nonzero selects the "OptIng" label.
    const int isActive = ((int(__fastcall*)(void*, void*))CSF_AT(void**, self, 0)[0x30 / 4])(self, nullptr) & 0xFF;

    const char* label = isActive != 0 ? g_pszMenu_P_OptIng : g_pszMenu_P_Option;

    // Sub-object at +0x18: obj->vtbl[0x58](obj, label)
    void* subObjectAgain = reinterpret_cast<void*>(self->field_0018);
    return ((int(__fastcall*)(void*, void*, const char*))CSF_AT(void**, subObjectAgain, 0)[0x58 / 4])(
        subObjectAgain, nullptr, label);
}

HOOK(0x0053B5F0, CMenuGameOptions_UpdateToggleButtonLabel, "auto");
