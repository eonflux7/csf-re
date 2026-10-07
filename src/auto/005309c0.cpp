#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// 0x005309C0 — returns the DWORD stored at offset 0x1EC of the object.
static int __fastcall CMenuMpServerInfo_GetField01ec(CMenuMpServerInfo* self, void* /*edx*/)
{
    return self->field_01ec;
}

HOOK(0x5309C0, CMenuMpServerInfo_GetField01ec, "auto");
