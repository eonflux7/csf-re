#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Increments the counter field by 4, then calls the inner object's vtable slot 2
// (byte offset 0x8), __thiscall: (this, a2, 4). Returns whether the inner result equals 4.
static int __fastcall C_0085e760_IncrementCounterByFourAndCheckInnerResult(C_0085e760* self, void* /*edx*/, int a2) {
    self->field_0008 += 4;

    void* innerObject = reinterpret_cast<void*>(self->field_0004);
    int innerResult = ((int(__fastcall*)(void*, void*, int, int))CSF_AT(void**, innerObject, 0)[2])(innerObject, nullptr, a2, 4);

    return innerResult == 4;
}

HOOK(0x0069A2D0, C_0085e760_IncrementCounterByFourAndCheckInnerResult, "auto");
