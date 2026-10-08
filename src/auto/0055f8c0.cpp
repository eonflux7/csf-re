#include <cstdint>
#include "hook.h"
#include "globals.h"

// 0x00899B90 : const char* g_pszMenu_P_AuUnPa
CSF_GLOBAL(const char*, g_pszMenu_P_AuUnPa, 0x00899B90);

// Calls the virtual handler at vtable slot 0x58/4 = 22 on the menu object stored
// at this+0x3c, passing g_pszMenu_P_AuUnPa, then stores state 4 at this+0x18.
// Returns the callee's result.
static int __fastcall CMenuMpOnline_OpenAuUnPaMenu(void* self, void* /*edx*/)
{
    void* menuObject = CSF_AT(void*, self, 0x3c);
    int result = ((int(__fastcall*)(void*, void*, const char*))CSF_AT(void**, menuObject, 0)[0x58 / 4])(
        menuObject, nullptr, g_pszMenu_P_AuUnPa);
    CSF_AT(int, self, 0x18) = 4;
    return result;
}

HOOK(0x0055F8C0, CMenuMpOnline_OpenAuUnPaMenu, "auto");
