#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

static int __fastcall C_0082dd78_DispatchReturnedObject(
    C_0082dd78* self, void* /*edx*/)
{
    void* inner = CSF_AT(void*, self, 0x04);

    using GetObjectMethod = void* (__fastcall*)(void*, void*);
    void* target =
        reinterpret_cast<GetObjectMethod>(
            CSF_AT(void**, inner, 0)[0x28 / 4])(inner, nullptr);

    void** targetVtable = CSF_AT(void**, target, 0);

    using TailMethod = int (__fastcall*)(C_0082dd78*, void*);
    [[clang::musttail]]
    return reinterpret_cast<TailMethod>(targetVtable[0x70 / 4])(
        reinterpret_cast<C_0082dd78*>(target), targetVtable);
}

HOOK(0x0041E0C0, C_0082dd78_DispatchReturnedObject, "auto");
