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

CSF_FUNC(int, __fastcall, sub_743010, 0x00743010, (void* self, void* edx, void* a2));

// Forwards its two stack arguments to 0x743010. The original loads a1 into ecx
// right before the call, so a1 becomes the callee's `this`; a2 is pushed as the
// callee's single stack argument.
static int __stdcall NetCogibleDdl_ForwardToInnerHandler(char* a1, C_008622e8* a2)
{
    return sub_743010(reinterpret_cast<void*>(a1), nullptr, reinterpret_cast<void*>(a2));
}

HOOK(0x743410, NetCogibleDdl_ForwardToInnerHandler, "auto");
