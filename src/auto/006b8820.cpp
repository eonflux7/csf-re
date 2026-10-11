#include <cstdint>
#include "globals.h"
#include "hook.h"

// Reads the int stored at (base + a1 + 4), where base is the dword at 0x8BB488.
// The original reloads the global on every call and adds the signed argument
// as a plain integer offset, with the constant +4 applied after it.
CSF_GLOBAL(uint32_t, g_ObjectPropsTableBase, 0x8BB488);

static int __cdecl GfxObjectProps_ReadTableEntryField4(int a1) {
    uint32_t address = g_ObjectPropsTableBase + static_cast<uint32_t>(a1) + 4u;
    return *reinterpret_cast<int*>(address);
}

HOOK(0x006B8820, GfxObjectProps_ReadTableEntryField4, "auto");
