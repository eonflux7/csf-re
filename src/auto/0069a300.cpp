#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Advances this object's byte counter by 8, forwards a2 and 8 to the inner
// object's vtable slot 2 (byte offset 0x8), and reports whether it returned 8.
static int __fastcall C_0085e760_AdvanceCounterAndCheckInnerResult(C_0085e760* self, void* /*edx*/, int a2) {
    self->field_0008 += 8;

    void* innerObject = reinterpret_cast<void*>(self->field_0004);
    int innerResult = ((int(__fastcall*)(void*, void*, int, int))CSF_AT(void**, innerObject, 0)[2])(innerObject, nullptr, a2, 8);

    return innerResult == 8;
}

HOOK(0x0069A300, C_0085e760_AdvanceCounterAndCheckInnerResult, "auto");
