#include <cstdint>

#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

static int __fastcall C_00863cd4_GetInnerFlagBit0(C_00863cd4* self, void* /*edx*/) {
    void* innerObject = reinterpret_cast<void*>(self->field_0004);
    int outFlags = 0;
    using InnerGetFlagsFn = void(__stdcall*)(void* innerObject, int* outFlags);
    InnerGetFlagsFn getFlags = reinterpret_cast<InnerGetFlagsFn>(CSF_AT(void**, innerObject, 0)[9]);
    getFlags(innerObject, &outFlags);
    return outFlags & 1;
}

HOOK(0x7111C0, C_00863cd4_GetInnerFlagBit0, "auto");
