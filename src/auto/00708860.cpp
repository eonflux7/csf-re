#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// The vtable object itself lives at 0x008634C0; the original stores its address
// into the instance's vtbl field.
CSF_GLOBAL(void*, g_vtbl_C_008634c0, 0x008634C0);

// C_008634c0::SetVtable (0x00708860)
// Overwrites the object's vtable pointer with the vtable at 0x008634C0.
static void __fastcall C_008634c0_SetVtable(C_008634c0* self, void* /*edx*/)
{
    self->vtbl = &g_vtbl_C_008634c0;
}

HOOK(0x00708860, C_008634c0_SetVtable, "auto");
