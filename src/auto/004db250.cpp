#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

static int __fastcall TCharTask_ForwardLinkedObjectMethod(
    C_0082fbe8* self, void* edx)
{
    auto* linkedObject = reinterpret_cast<C_0082fbe8*>(
        CSF_AT(std::uint32_t, self, 0x10));

    if (!linkedObject)
        return 0;

    // Force a tail jump, preserving the caller's stack and incoming EDX.
    [[clang::musttail]]
    return reinterpret_cast<int(__fastcall*)(C_0082fbe8*, void*)>(
        CSF_AT(void**, linkedObject, 0)[7])(linkedObject, edx);
}

HOOK(0x004DB250, TCharTask_ForwardLinkedObjectMethod, "auto");
