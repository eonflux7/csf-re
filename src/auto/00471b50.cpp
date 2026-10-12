#include "hook.h"
#include "globals.h"
#include "types/auto_classes.h"

static CSF_FUNC(int, __cdecl, MissionScript_ReleaseBlock, 0x004714E0, (S_0069b1c0* Block));

static S_0069b1c0* __cdecl MissionScript_ReleaseBlockIfPresent(S_0069b1c0* block)
{
    if (block)
        return reinterpret_cast<S_0069b1c0*>(MissionScript_ReleaseBlock(block));
    return block;
}

HOOK(0x00471B50, MissionScript_ReleaseBlockIfPresent, "auto");
