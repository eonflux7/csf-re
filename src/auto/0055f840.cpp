// CMenuMpOnline::SetLabelText (0x0055F840)
// Forwards the menu label string to a sub-object's virtual method (vtbl+0x58),
// then tail-calls 0x54E780 and returns its result.
#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Pointer to the current menu label string (read as a pointer load from 0x899B30).
CSF_GLOBAL(const char*, g_pszMenu_P_LbSlCu, 0x899B30);

// Shared routine tail-jumped to at the end of the function.
CSF_FUNC(int, __cdecl, MenuMpOnline_Sub_54E780, 0x54E780, ());

static int __fastcall CMenuMpOnline_SetLabelText(void* self, void* /*edx*/)
{
    // ecx = this->field_003C (pointer to a sub-object that owns the vtable)
    void* pSub = *reinterpret_cast<void**>(static_cast<uint8_t*>(self) + 0x3C);

    // edx = g_pszMenu_P_LbSlCu
    const char* pszLabel = g_pszMenu_P_LbSlCu;

    // eax = pSub->vtbl ; call vtbl[0x58 / 4](pSub, pszLabel)
    void** vtbl = *reinterpret_cast<void***>(pSub);
    reinterpret_cast<void(__fastcall*)(void*, void*, const char*)>(
        vtbl[0x58 / 4])(pSub, nullptr, pszLabel);

    return MenuMpOnline_Sub_54E780();
}

HOOK(0x55F840, CMenuMpOnline_SetLabelText, "auto");
