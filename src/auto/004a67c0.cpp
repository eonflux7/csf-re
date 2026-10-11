#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

// Reads the object stored at self+8, asks it for a sub-object (vtbl slot 10),
// asks that for another sub-object (vtbl slot 26), then queries the last one
// (vtbl slot 16) with argument 20 and returns the result.
static int __fastcall GetNestedValueFor20(void* self, void* /*edx*/) {
    void* firstObject = CSF_AT(void*, self, 0x8);
    void* secondObject = ((void* (__fastcall*)(void*, void*))CSF_AT(void**, firstObject, 0)[40 / 4])(firstObject, nullptr);
    void* thirdObject = ((void* (__fastcall*)(void*, void*))CSF_AT(void**, secondObject, 0)[104 / 4])(secondObject, nullptr);
    return ((int (__fastcall*)(void*, void*, int))CSF_AT(void**, thirdObject, 0)[64 / 4])(thirdObject, nullptr, 20);
}

HOOK(0x4A67C0, GetNestedValueFor20, "auto");
