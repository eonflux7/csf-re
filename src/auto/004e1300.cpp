#include "hook.h"
#include "types/auto_classes.h"

// Reads the linked value via the object's virtual getter (slot +0x84), stores it
// in field_0008, sets field_0014 to -1 and clears field_0010. Returns the value.
static int __fastcall C_0083e160_CaptureLinkedValueAndReset(C_0083e160* self, void* /*edx*/) {
    int linkedValue = self->vGetLinkedValueDC();
    self->field_0008 = linkedValue;
    self->field_0014 = -1;
    self->field_0010 = 0;
    return linkedValue;
}

HOOK(0x4E1300, C_0083e160_CaptureLinkedValueAndReset, "auto");
