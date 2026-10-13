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

CSF_FUNC(uint32_t*, __fastcall, sub_747A30, 0x00747A30, (void* self, void* edx, void* arg0));

// Forwards the fourth stack argument (a4) to the object passed as the second
// stack argument (a2). The disassembly loads ecx from [ebp+0xc] (a2), so a2 is
// the callee's `this`, and pushes [ebp+0x14] (a4) as its single argument.
// a1, a3 and a5 are never read.
static uint32_t* __stdcall NetProyectilDdl_ForwardFourthArgToSecondObject(int a1, C_008622e8* a2, int a3, C_008622e8* a4, int a5)
{
    (void)a1;
    (void)a3;
    (void)a5;
    return sub_747A30(a2, nullptr, a4);
}

HOOK(0x747A10, NetProyectilDdl_ForwardFourthArgToSecondObject, "auto");
