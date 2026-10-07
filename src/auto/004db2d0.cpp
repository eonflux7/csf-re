#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

static int __fastcall C_0082fbe8_ForwardToMemberVirtual(
    C_0082fbe8* self, void* edx)
{
    void* target = CSF_AT(void*, self, 0x10);
    if (!target)
        return 0;

    void** vtable = CSF_AT(void**, target, 0);

    // Matching ABI signatures permit a mandatory tail jump with no stack arguments.
    [[clang::musttail]] return
        (reinterpret_cast<int(__fastcall*)(C_0082fbe8*, void*)>(vtable[13]))(
            reinterpret_cast<C_0082fbe8*>(target), edx);
}

HOOK(0x004DB2D0, C_0082fbe8_ForwardToMemberVirtual, "auto");
