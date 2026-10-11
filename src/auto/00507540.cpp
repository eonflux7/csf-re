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

// Shared value block released at shutdown (global at 0x8B056C).
CSF_GLOBAL(int, g_value_8B056C, 0x008B056C);
CSF_GLOBAL(uint8_t, g_unk_8BAD98, 0x008BAD98);

CSF_FUNC(int*, __fastcall, ReleaseSharedValueBlock, 0x00507480, (void* self, void* edx));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));
CSF_FUNC(int, __fastcall, ResetUnk8BAD98State, 0x0069DE90, (void* self, void* edx));

// Releases the global value block and resets the auxiliary state object.
static int __cdecl AppMain_ReleaseValueBlock()
{
    if (g_value_8B056C == 0)
        return 0;

    int blockAddress = g_value_8B056C;
    ReleaseSharedValueBlock(reinterpret_cast<void*>(static_cast<uintptr_t>(blockAddress)), nullptr);
    Mem_Free(reinterpret_cast<void*>(static_cast<uintptr_t>(blockAddress)));
    g_value_8B056C = 0;

    // Original tail-jumps through the 0x690D90 thunk, which loads ecx = 0x8BAD98 and jumps to 0x69DE90.
    return ResetUnk8BAD98State(&g_unk_8BAD98, nullptr);
}

HOOK(0x00507540, AppMain_ReleaseValueBlock, "auto");
