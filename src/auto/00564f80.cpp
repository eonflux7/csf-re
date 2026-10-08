#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Forwards (a2, 0, a4 + 8, a3) to the virtual method at vtable slot 0x290 of the
// sub-object stored at this+0x14, and returns its result.
static int __fastcall CMenuAccounts_CallChildVtblMethod290(void* self, void* /*edx*/, int a2, int a3, int a4)
{
    void* child = CSF_AT(void*, self, 0x14);
    void** vtbl = CSF_AT(void**, child, 0);
    using Fn = int(__fastcall*)(void*, void*, int, int, int, int);
    return reinterpret_cast<Fn>(vtbl[0x290 / 4])(child, nullptr, a2, 0, a4 + 8, a3);
}

HOOK(0x00564F80, CMenuAccounts_CallChildVtblMethod290, "auto");
