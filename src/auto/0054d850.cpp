#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// 0x0054D850: installs the class vtable pointer into this.
static void __fastcall C_008443c4_Constructor(C_008443c4* self, void* /*edx*/)
{
    self->vtbl = reinterpret_cast<void**>(0x008443C4);
}

HOOK(0x54D850, C_008443c4_Constructor, "auto");
