#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// 0x0052AFC0: returns the dword at offset 0x894 of CMenuMessageBox.
static int __fastcall CMenuMessageBox_GetField0894(CMenuMessageBox* self, void* /*edx*/)
{
    return static_cast<int>(CSF_AT(uint32_t, self, 0x894));
}

HOOK(0x0052AFC0, CMenuMessageBox_GetField0894, "auto");
