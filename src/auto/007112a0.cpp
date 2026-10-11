#include <cstdint>

#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Virtual slot +0x10 of the object stored at this->field_0004 (slot 4).
// Stdcall: (this, int* out, uint32_t flag); callee pops its own 12 bytes.
using ChildGetCountFn = void(__stdcall*)(void* self, int* outCount, uint32_t flag);

static int __fastcall C_00863cd4_IsCountAtLeastHalfCapacity(C_00863cd4* self, void* /*edx*/) {
    void* childObject = reinterpret_cast<void*>(self->field_0004);
    void** childVtbl = CSF_AT(void**, childObject, 0);
    ChildGetCountFn getCount = reinterpret_cast<ChildGetCountFn>(childVtbl[0x10 / 4]);

    int currentCount = 0;
    getCount(childObject, &currentCount, 0);

    const int halfCapacity = static_cast<int>(self->field_0008) / 2;
    return currentCount >= halfCapacity ? 1 : 0;
}

HOOK(0x7112A0, C_00863cd4_IsCountAtLeastHalfCapacity, "auto");
