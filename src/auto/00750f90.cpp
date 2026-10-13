#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_FUNC(void, __fastcall, NetPlayerDdl_ProcessBaseChunks, 0x007A39C0, (void* self, void* edx, void* a2));

// Forwards the second stack argument (a2) to NetPlayerDdl_ProcessBaseChunks.
// The disassembly loads ecx from [ebp+8] (a1) and pushes [ebp+0xc] (a2) as the
// callee's single argument, so a1 is the callee's `this`.
static void __stdcall NetChatDdl_ForwardBaseChunksToPlayer(C_008622e8* a1, C_008622e8* a2)
{
    NetPlayerDdl_ProcessBaseChunks(a1, nullptr, a2);
}

HOOK(0x750F90, NetChatDdl_ForwardBaseChunksToPlayer, "auto");
