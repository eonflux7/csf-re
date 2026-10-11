#include "hook.h"
#include "globals.h"

// Bound by address. The original pushes three cdecl arguments per call:
// (id, a2, a3), with id being the constant. See notes in the metadata.
CSF_FUNC(void*, __cdecl, Cheats_ApplyCheatEntry, 0x0042AD00, (int cheatId, int a2, int a3));

static void* __cdecl Cheats_ApplyCheatSet(int a2, int a3)
{
    Cheats_ApplyCheatEntry(4, a2, a3);
    Cheats_ApplyCheatEntry(5, a2, a3);
    Cheats_ApplyCheatEntry(6, a2, a3);
    Cheats_ApplyCheatEntry(7, a2, a3);
    Cheats_ApplyCheatEntry(16, a2, a3);
    return Cheats_ApplyCheatEntry(9, a2, a3);
}

HOOK(0x0042AE60, Cheats_ApplyCheatSet, "auto");
