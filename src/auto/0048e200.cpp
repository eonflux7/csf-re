#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

static int __fastcall C_00833908_TailDispatchRelatedObject(
    C_00833908* self, void* /*edx*/)
{
    auto* relatedObject = reinterpret_cast<C_00833908*>(
        reinterpret_cast<int (__fastcall*)(C_00833908*, void*)>(
            self->vtbl[10])(self, nullptr));

    void** relatedVtable = CSF_AT(void**, relatedObject, 0);

    [[clang::musttail]]
    return reinterpret_cast<int (__fastcall*)(C_00833908*, void*)>(
        relatedVtable[41])(relatedObject, relatedVtable);
}

HOOK(0x0048E200, C_00833908_TailDispatchRelatedObject, "auto");
