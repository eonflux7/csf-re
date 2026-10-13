#include "hook.h"
#include "globals.h"

// Returns the byte stored at +0x31 of the object, or 0 when the handle is null.
static char __cdecl GfxObject_CheckObjectState(int objectHandle) {
    if (objectHandle) {
        return CSF_AT(char, reinterpret_cast<void*>(objectHandle), 0x31);
    }
    return 0;
}

HOOK(0x006E2F30, GfxObject_CheckObjectState, "auto");
