#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

// Tail-jumps to SceneLoader_GetSectorRecord, forwarding ecx (this) and the stack argument.
CSF_FUNC(int, __fastcall, SceneLoader_GetSectorRecord, 0x00762470, (void* self, void* /*edx*/, int sectorIndex));

static int __fastcall C_00840e0c_GetSectorRecord(void* self, void* /*edx*/, int sectorIndex)
{
    return SceneLoader_GetSectorRecord(self, nullptr, sectorIndex);
}

HOOK(0x0051B8E0, C_00840e0c_GetSectorRecord, "auto");
