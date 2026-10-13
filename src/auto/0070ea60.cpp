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

// Original 0x0070E970: __fastcall, self in ecx, edx unused, pops nothing.
CSF_FUNC(char, __fastcall, Audio_ProcessEffectEntry, 0x0070E970, (void* self, void* edx));

// Original 0x0070EA60: __cdecl(a1). Loads a1 into ecx and tail-jumps to
// 0x70E970, so the callee's result in al is returned to our caller unchanged.
static char __cdecl Audio_ForwardToEntryHandler(S_0070e170* a1)
{
    return Audio_ProcessEffectEntry(a1, nullptr);
}

HOOK(0x0070EA60, Audio_ForwardToEntryHandler, "auto");
