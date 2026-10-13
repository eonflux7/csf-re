#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Inner object's vtable slot 2 (byte offset 0x8), __thiscall: (this, a2, 2).
static int __fastcall C_0085e760_IncrementCounterAndCheckInnerResult(C_0085e760* self, void* /*edx*/, int a2) {
    self->field_0008 += 2;

    void* innerObject = reinterpret_cast<void*>(self->field_0004);
    int innerResult = ((int(__fastcall*)(void*, void*, int, int))CSF_AT(void**, innerObject, 0)[2])(innerObject, nullptr, a2, 2);

    return innerResult == 2;
}

HOOK(0x0069A270, C_0085e760_IncrementCounterAndCheckInnerResult, "auto");
