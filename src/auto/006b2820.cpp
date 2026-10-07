#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// Vtable of C_00860178 (data referenced at 0x00860178).
CSF_GLOBAL(void*, g_C_00860178_vtable, 0x00860178);

// Sets the object's vtable pointer to C_00860178's vtable and returns the object.
static C_00860178* __fastcall C_00860178_Construct(C_00860178* self, void* /*edx*/)
{
    self->vtbl = &g_C_00860178_vtable;
    return self;
}

HOOK(0x006B2820, C_00860178_Construct, "auto");
