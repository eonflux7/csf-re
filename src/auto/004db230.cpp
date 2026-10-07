#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

static int __fastcall C_0082fbe8_ForwardInnerObjectVirtualCall(
    C_0082fbe8* self, void* edx)
{
    auto* inner = reinterpret_cast<C_0082fbe8*>(
        CSF_AT(uint32_t, self, 0x10));

    if (!inner)
        return 0;

    // Force a tail jump, retaining the caller's stack and forwarding EDX.
    [[clang::musttail]] return
        ((int(__fastcall*)(C_0082fbe8*, void*))
            CSF_AT(void**, inner, 0)[4])(inner, edx);
}

HOOK(0x004DB230, C_0082fbe8_ForwardInnerObjectVirtualCall, "auto");
