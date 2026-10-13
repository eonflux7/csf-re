#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Callee at 0x81C750: cdecl, 3 stack args, returns int. Unknown prototype, inferred from the call site.
CSF_FUNC(int, __cdecl, Im3d_DispatchCall, 0x81C750, (uint32_t handle, void* callback, void* args));

// Callback at 0x6E41D0, passed as a function value.

static int __fastcall C_00861b58_ForwardToDispatch(C_00861b58* self, void* /*edx*/, int a2, int a3)
{
    uint32_t handle = self->field_0004;

    // Two-dword argument block on the stack: [0] = a3, [1] = a2.
    alignas(8) uint32_t argBlock[2];
    argBlock[0] = static_cast<uint32_t>(a3);
    argBlock[1] = static_cast<uint32_t>(a2);

    return Im3d_DispatchCall(handle,
                             reinterpret_cast<void*>(CSF_ADDR_FN(0x006E41D0)),
                             argBlock);
}

HOOK(0x006E4270, C_00861b58_ForwardToDispatch, "auto");
