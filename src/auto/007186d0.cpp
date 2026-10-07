#include <cstdint>
#include "globals.h"
#include "hook.h"

// CGuiControl::SetSelectable(char)
//
// Stores the flag at +0x115 and, depending on it, either leaves the value at
// +0xe0 alone (rewriting it with itself) or forces it to 2 (returning 2).
static int __fastcall CGuiControl_SetSelectable(void* self, void* /*edx*/, char a2)
{
    CSF_AT(char, self, 0x115) = a2;

    if (a2)
    {
        // Original reloads and re-stores the same dword at +0xe0 (no side effect),
        // then returns it.
        int state = CSF_AT(int, self, 0xe0);
        CSF_AT(int, self, 0xe0) = state;
        return state;
    }

    CSF_AT(int, self, 0xe0) = 2;
    return 2;
}

HOOK(0x7186D0, CGuiControl_SetSelectable, "auto");
