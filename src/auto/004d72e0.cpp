#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "core/memory.h"

// Calls slot 35 (+0x8C) of the vtable of the object at owner+0x14, where owner = this+0x08.
// Arguments passed: 0, 0, -1, 0. Returns the callee's result.
static int __fastcall Autonomo_InvokeInnerQueryWithFlags(void* self, void* /*edx*/)
{
    void* owner = CSF_AT(void*, self, 0x08);
    void* innerObject = CSF_AT(void*, owner, 0x14);
    void** innerVtbl = CSF_AT(void**, innerObject, 0);
    using InnerQueryFn = int(__fastcall*)(void* self, void* edx, int a, int b, int c, int d);
    InnerQueryFn queryFn = reinterpret_cast<InnerQueryFn>(innerVtbl[0x8C / 4]);
    return queryFn(innerObject, nullptr, 0, 0, -1, 0);
}

HOOK(0x004D72E0, Autonomo_InvokeInnerQueryWithFlags, "auto");
