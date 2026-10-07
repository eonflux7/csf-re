#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// 0x005309C0 — returns the DWORD stored at offset 0x1EC of the object.
static int __fastcall C_00841ae8_GetField01ec(C_00841ae8* self, void* /*edx*/)
{
    return self->field_01ec;
}

HOOK(0x5309C0, C_00841ae8_GetField01ec, "auto");
