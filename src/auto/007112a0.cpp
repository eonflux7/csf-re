#include <cstdint>

#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Virtual slot +0x10 of the object stored at this->field_0004 (slot 4).
// Original: mov ecx,[child] (ecx = child vtable), lea edx,[count]; push 0; push edx; push child; call [ecx+0x10].
// Callee sees the vtable in ecx, &count in edx, and (child, &count, 0) on the stack; it pops its 12 stack bytes.
using ChildGetCountFn = void(__fastcall*)(void* vtblInEcx, int* outCountInEdx, void* childArg, int* outCountArg, uint32_t flag);

static int __fastcall C_00863cd4_IsCountAtLeastHalfCapacity(C_00863cd4* self, void* /*edx*/) {
    void* childObject = reinterpret_cast<void*>(self->field_0004);
    void** childVtbl = CSF_AT(void**, childObject, 0);
    ChildGetCountFn getCount = reinterpret_cast<ChildGetCountFn>(childVtbl[0x10 / 4]);

    int currentCount = 0;
    getCount(childVtbl, &currentCount, childObject, &currentCount, 0);

    const int halfCapacity = static_cast<int>(self->field_0008) / 2;
    return currentCount >= halfCapacity ? 1 : 0;
}

HOOK(0x7112A0, C_00863cd4_IsCountAtLeastHalfCapacity, "auto");
