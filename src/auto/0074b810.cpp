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

CSF_FUNC(void*, __fastcall, NetStream_ReadIntoSlot, 0x00655FC0, (void* self, void* edx, char* a2));

// Forwards the second stack argument (a2) to the object passed as the fourth
// stack argument (a4). The disassembly loads ecx from [ebp+0x14] (a4), so a4 is
// the callee's `this`, and pushes [ebp+0xc] (a2) as its single argument. The
// result is returned unchanged. a1, a3 and a5 are never read.
static void* __stdcall NetGhostDdl_ReadIntoSlotViaFourthArg(int a1, char* a2, int a3, C_008622e8* a4, int a5)
{
    (void)a1;
    (void)a3;
    (void)a5;
    return NetStream_ReadIntoSlot(a4, nullptr, a2);
}

HOOK(0x74B810, NetGhostDdl_ReadIntoSlotViaFourthArg, "auto");
