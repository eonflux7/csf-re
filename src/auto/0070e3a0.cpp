#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// mov al, [ecx+45h]; ret: a byte getter, with no stores.
static uint8_t __fastcall Audio_GetFlag45(C_008639b0* self, void*) {
    return self->field_0045;
}
HOOK(0x0070E3A0, Audio_GetFlag45, "m70DDE0");
