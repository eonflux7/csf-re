#include <cstdint>

#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

static int __fastcall C_00863cd4_GetInnerFlagBit0(C_00863cd4* self, void* /*edx*/) {
    void* innerObject = reinterpret_cast<void*>(self->field_0004);
    void** innerVtbl = CSF_AT(void**, innerObject, 0);
    int outFlags = 0;
    // Original: mov ecx,[inner] (ecx = inner vtable), lea edx,[outFlags], push edx, push inner,
    // call [ecx+0x24] (slot 9). The callee sees the vtable in ecx, &outFlags in edx, and
    // (inner, &outFlags) on the stack. A __fastcall pointer with two register args gives that layout.
    using InnerGetFlagsFn = void(__fastcall*)(void* vtblInEcx, int* outFlagsInEdx, void* innerArg, int* outFlagsArg);
    reinterpret_cast<InnerGetFlagsFn>(innerVtbl[9])(innerVtbl, &outFlags, innerObject, &outFlags);
    return outFlags & 1;
}

HOOK(0x7111C0, C_00863cd4_GetInnerFlagBit0, "auto");
