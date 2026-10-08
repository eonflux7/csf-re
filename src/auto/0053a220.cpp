#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_GLOBAL(const char*, g_pszMenu_P_SaGame, 0x00899B10);

// The child control stored at CMenuPause::field_0010 gets its owner/context set
// (virtual slot 0x60, called with (this, 0)) and then receives the caption
// g_pszMenu_P_SaGame (virtual slot 0x58). Returns the caption call's result.
static int __fastcall CMenuPause_ApplyLabelToChild(CMenuPause* self, void* /*edx*/)
{
    void* child = reinterpret_cast<void*>(self->field_0010);
    ((void (__fastcall*)(void*, void*, CMenuPause*, int))CSF_AT(void**, child, 0)[0x60 / 4])(child, nullptr, self, 0);

    // Re-read the child pointer: the previous virtual call may replace it.
    child = reinterpret_cast<void*>(self->field_0010);
    const char* label = g_pszMenu_P_SaGame;
    return ((int (__fastcall*)(void*, void*, const char*))CSF_AT(void**, child, 0)[0x58 / 4])(child, nullptr, label);
}

HOOK(0x0053A220, CMenuPause_ApplyLabelToChild, "auto");
