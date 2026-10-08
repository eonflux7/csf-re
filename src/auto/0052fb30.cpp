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

// Original callees (bound by address)
CSF_FUNC(int, __cdecl, GetGlobalValue_8BC1E0, 0x756A10, ());
CSF_FUNC(int, __cdecl, GetGlobalValue_8B0568, 0x4FDBB0, ());

// Global pointer to the "Menu_P_Campan" string (value read at 0x899AD8)
CSF_GLOBAL(const char*, g_pszMenu_P_Campan, 0x899AD8);

// Refreshes the campaign listbox selection of the scene selector.
static int __fastcall CMenuSceneSelect_RefreshListboxSelection(CMenuSceneSelect* self, void* /*edx*/)
{
    // First: virtual call (slot 0x60) on the scene listbox object, args (self, 0)
    void* listboxObj = reinterpret_cast<void*>(self->field_0034);
    void** listboxVtbl = CSF_AT(void**, listboxObj, 0);
    ((void(__fastcall*)(void*, void*, CMenuSceneSelect*, int))listboxVtbl[0x60 / 4])(
        listboxObj, nullptr, self, 0);

    int globalState = GetGlobalValue_8BC1E0();
    int result = globalState;

    if (!CSF_AT(uint8_t, reinterpret_cast<void*>(globalState), 0xA6C))
    {
        int otherState = GetGlobalValue_8B0568();
        if (CSF_AT(int, reinterpret_cast<void*>(otherState), 0x5C) != 1)
        {
            // Reload the listbox object pointer from self (original re-reads [esi+0x34])
            void* listboxObjAgain = reinterpret_cast<void*>(self->field_0034);
            void** vtblAgain = CSF_AT(void**, listboxObjAgain, 0);
            result = ((int(__fastcall*)(void*, void*, const char*))vtblAgain[0x58 / 4])(
                listboxObjAgain, nullptr, g_pszMenu_P_Campan);
        }
    }

    return result;
}

HOOK(0x52FB30, CMenuSceneSelect_RefreshListboxSelection, "auto");
