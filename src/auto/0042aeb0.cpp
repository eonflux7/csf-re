#include "hook.h"
#include "globals.h"

// Bound by address. The original pushes three cdecl arguments per call:
// (id, a2, a3), with id being the constant. See notes in the metadata.
CSF_FUNC(void*, __cdecl, Cheats_ApplyCheatEntry, 0x0042AD00, (int cheatId, int a2, int a3));

static void* __cdecl Cheats_ApplyCheatSetGroupB(int a2, int a3)
{
    Cheats_ApplyCheatEntry(13, a2, a3);
    Cheats_ApplyCheatEntry(18, a2, a3);
    Cheats_ApplyCheatEntry(19, a2, a3);
    Cheats_ApplyCheatEntry(24, a2, a3);
    Cheats_ApplyCheatEntry(27, a2, a3);
    return Cheats_ApplyCheatEntry(17, a2, a3);
}

HOOK(0x0042AEB0, Cheats_ApplyCheatSetGroupB, "auto");
