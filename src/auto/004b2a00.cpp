#include <cstdint>
#include "hook.h"
#include "types/auto_classes.h"

// Returns the constant 30. No arguments, no side effects.
static int __fastcall TFncInteractivo_GetThirty(TFncInteractivo* self, void* /*edx*/)
{
    (void)self;
    return 30;
}

HOOK(0x4B2A00, TFncInteractivo_GetThirty, "auto");
