#include "hook.h"
#include "globals.h"

// Returns the byte stored at +0x30 of the object, or 0 when the handle is null.
static char __cdecl GfxSkin_ProbeEntryHandle(int entryHandle) {
    if (entryHandle) {
        return CSF_AT(char, reinterpret_cast<void*>(entryHandle), 0x30);
    }
    return 0;
}

HOOK(0x006E2F10, GfxSkin_ProbeEntryHandle, "auto");
