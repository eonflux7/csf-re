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

// Dispatcher at 0x81C750: __cdecl, 3 args, caller pops (add esp, 0xC).
CSF_FUNC(int, __cdecl, Im3d_InvokeWithCallback, 0x81C750, (uint32_t owner, void* callback, void* argBlock));

static int __fastcall C_00861b40_InvokeWithArgs(C_00861b40* self, void* /*edx*/, int a2, int a3)
{
    // Pack the two caller arguments into a three-dword block: [a3, a2, 0].
    alignas(4) uint32_t argBlock[3];
    argBlock[0] = static_cast<uint32_t>(a3);
    argBlock[1] = static_cast<uint32_t>(a2);
    argBlock[2] = 0;

    uint32_t owner = CSF_AT(uint32_t, self, 0x4);

    return Im3d_InvokeWithCallback(
        owner,
        reinterpret_cast<void*>(CSF_ADDR_FN(0x006E3F60)),
        argBlock);
}

HOOK(0x006E4030, C_00861b40_InvokeWithArgs, "auto");
