#include <cstdint>

#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Hands out the two stored values (field_0004 and field_0010) through the caller's
// out pointers, then clears the object's four slots. Always returns 0.
static int __fastcall C_0085e7b8_TakeHandlesAndReset(C_0085e7b8* self, void* /*edx*/,
                                                     uint32_t* outFirst, uint32_t* outSecond) {
    *outFirst = self->field_0004;
    *outSecond = self->field_0010;
    self->field_0004 = 0;
    self->field_0010 = 0;
    self->field_0008 = 0;
    self->field_000c = 0;
    return 0;
}

HOOK(0x0069A560, C_0085e7b8_TakeHandlesAndReset, "auto");
