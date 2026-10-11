#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

// Table base pointer loaded from the game's global. Its value is used as a base address.
CSF_GLOBAL(uint32_t, g_ObjectAttachmentTable, 0x008BB478);

// Returns the attachment word stored at (table base + index + 4), or 0 when index is 0.
static int __cdecl GfxObjectProps_GetObjectAttachment(int objectIndex) {
    if (objectIndex == 0) {
        return 0;
    }
    uintptr_t tableBase = static_cast<uintptr_t>(g_ObjectAttachmentTable);
    void* entryAddress = reinterpret_cast<void*>(tableBase + static_cast<uintptr_t>(objectIndex));
    return CSF_AT(int, entryAddress, 4);
}

HOOK(0x006B87A0, GfxObjectProps_GetObjectAttachment, "auto");
