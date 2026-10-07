#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// Returns the DWORD stored at offset 0x84 of the object.
static int __fastcall C_00843230_GetField0084(C_00843230* self, void* /*edx*/)
{
    return static_cast<int>(self->field_0084);
}

HOOK(0x00541960, C_00843230_GetField0084, "auto");
