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

CSF_FUNC(void*, __fastcall, sub_655E20, 0x00655E20, (void* self, void* edx, char* a2));

// Forwards the name string to the object passed in the fourth argument.
// The disassembly loads ecx from [ebp+0x14] (a4: a1@+8, a2@+0xc, a3@+0x10,
// a4@+0x14, a5@+0x18), so a4 is the callee's `this`. a1, a3 and a5 are never read.
static void* __stdcall NetCogibleDdl_ForwardNameToTarget(int a1, char* a2, int a3, C_008622e8* a4, int a5)
{
    (void)a1;
    (void)a3;
    (void)a5;
    void* target = reinterpret_cast<void*>(a4);
    return sub_655E20(target, nullptr, a2);
}

HOOK(0x742A50, NetCogibleDdl_ForwardNameToTarget, "auto");
