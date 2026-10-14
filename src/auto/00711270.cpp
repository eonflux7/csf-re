#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Forwards to the child object in field_0004 via its vtable slot 4 (byte offset 0x10),
// which takes (object, int* first, int* second) with __stdcall, and returns the first out value.
static int __fastcall C_00863cd4_GetFirstOutputValue(C_00863cd4* self, void* /*edx*/) {
    int v3 = 0;
    int v2 = 0;

    void* child = reinterpret_cast<void*>(self->field_0004);
    void** childVtbl = CSF_AT(void**, child, 0);
    // Original: mov ecx,[child] (ecx = child vtable), lea edx,[v3]; push edx; push child; call [ecx+0x10]
    // with push &v2 before it. Callee sees vtable in ecx, &v3 in edx, and (child, &v3, &v2) on the stack.
    using ChildGetTwoOutFn = void(__fastcall*)(void* vtblInEcx, int* v3InEdx, void* childArg, int* v3Arg, int* v2Arg);
    reinterpret_cast<ChildGetTwoOutFn>(childVtbl[4])(childVtbl, &v3, child, &v3, &v2);

    return v3;
}

HOOK(0x711270, C_00863cd4_GetFirstOutputValue, "auto");
