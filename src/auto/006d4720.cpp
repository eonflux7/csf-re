#include <cstdint>

#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Returns the address of sector record `sectorIndex` in the sector table whose base
// pointer is stored at this+0x08. Each sector record is 0x3C (60) bytes.
static int __fastcall SceneLoader_GetSectorEntry(S_006d4450* self, void* /*edx*/, int sectorIndex)
{
    int tableBase = static_cast<int>(self->field_0008);
    return tableBase + sectorIndex * 60;
}

HOOK(0x006D4720, SceneLoader_GetSectorEntry, "auto");
