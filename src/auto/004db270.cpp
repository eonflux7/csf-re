#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

static int __fastcall C_0082fbe8_ForwardMemberVirtualSlot9(
    C_0082fbe8* self, void* edx)
{
    auto* member = CSF_AT(C_0082fbe8*, self, 0x10);
    if (!member)
        return 0;

    // Matching register-argument types allow musttail to preserve the
    // caller's stack, including arguments whose signature is unknown.
    [[clang::musttail]]
    return ((int (__fastcall*)(C_0082fbe8*, void*))
        CSF_AT(void**, member, 0)[9])(member, edx);
}

HOOK(0x4DB270, C_0082fbe8_ForwardMemberVirtualSlot9, "auto");
