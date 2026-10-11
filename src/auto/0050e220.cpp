#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_GLOBAL(const char*, g_pszMenu_P_ProgBr, 0x899B24);

// Returns the cached control handle at +0x100. On first use, asks the object at +0x14
// (through its vtable slot 0x58) for the control named by g_pszMenu_P_ProgBr and caches it.
static int __fastcall GetCachedProgressBarControl(void* self, void* /*edx*/)
{
    int result = CSF_AT(int, self, 0x100);
    if (result == 0) {
        void* lookupObject = CSF_AT(void*, self, 0x14);
        result = ((int(__fastcall*)(void*, void*, const char*))CSF_AT(void**, lookupObject, 0)[0x58 / 4])(
            lookupObject, nullptr, g_pszMenu_P_ProgBr);
        CSF_AT(int, self, 0x100) = result;
    }
    return result;
}

HOOK(0x0050E220, GetCachedProgressBarControl, "auto");
