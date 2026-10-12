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

CSF_FUNC(void, __fastcall, NetGameDdl_HandleEntry, 0x0074DD10, (void* self, void* edx, void* a2));

// Forwards the second stack argument (a2) to the object passed as the first
// stack argument (a1), which is loaded into ecx and used as `this` by the callee.
// The original's return value is whatever the callee left in eax; the bound
// callee signature is void, so that value is not recoverable here.
static int __stdcall NetGameDdl_ForwardEntryToHandler(char* a1, C_008622e8* a2)
{
    NetGameDdl_HandleEntry(a1, nullptr, a2);
    return 0;
}

HOOK(0x74E5F0, NetGameDdl_ForwardEntryToHandler, "auto");
