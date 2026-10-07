#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Dword read at 0x00863FF4 (mask applied to the boolean result: 0xFFFFFFFF & mask / 0 & mask).
CSF_GLOBAL(uint32_t, g_RowTextModeMask, 0x00863FF4);

// Queries the row text object (m_pRowText, +0x218) through its vtable slot +0x70 and maps the
// "non-zero" predicate back to either g_RowTextModeMask (query returned 0) or 0 (query != 0).
//   neg / sbb / not  =>  0xFFFFFFFF when the virtual call returned 0, else 0
//   and eax, [0x863FF4]  =>  mask & that value
static int __fastcall CGuiListBox_GetRowTextMode(CGuiListBox* self, void* /*edx*/)
{
    void* pRowText = reinterpret_cast<void*>(self->m_pRowText);
    void** vtbl = *reinterpret_cast<void***>(pRowText);
    int queryResult = (reinterpret_cast<int(__fastcall*)(void*, void*)>(vtbl[0x70 / 4]))(pRowText, nullptr);

    uint32_t mask = g_RowTextModeMask;
    uint32_t value = (queryResult == 0) ? 0xFFFFFFFFu : 0u;
    return static_cast<int>(value & mask);
}

HOOK(0x722CE0, CGuiListBox_GetRowTextMode, "auto");
