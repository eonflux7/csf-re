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

CSF_FUNC(int, __fastcall, Ddf20Handler_Process, 0x004DE0D0, (void* self, void* edx, int a2, int a3, int a4));

// Tail-forwards the request to the inner object stored at +0x18 with the same arguments.
static int __fastcall C_00834050_ForwardToInnerHandler(C_00834050* self, void* /*edx*/, int a2, int a3, int a4)
{
    void* innerObject = CSF_AT(void*, self, 0x18);
    return Ddf20Handler_Process(innerObject, nullptr, a2, a3, a4);
}

HOOK(0x004DE7E0, C_00834050_ForwardToInnerHandler, "auto");
