#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

static int __fastcall C_0082fbe8_ForwardMemberVirtualCall(
    C_0082fbe8* self, void* edx)
{
    void* member = CSF_AT(void*, self, 0x10);
    if (!member)
        return 0;

    // Matching register-only signatures allow musttail to preserve the
    // caller's stack, including any arguments unknown to this wrapper.
    [[clang::musttail]]
    return reinterpret_cast<int(__fastcall*)(C_0082fbe8*, void*)>(
        CSF_AT(void**, member, 0)[0x30 / 4])(
            reinterpret_cast<C_0082fbe8*>(member), edx);
}

HOOK(0x004DB2C0, C_0082fbe8_ForwardMemberVirtualCall, "auto");
