#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// 0x0041E200: writes the object's field at +0x18 into the caller-supplied
// output slot and returns that slot pointer.
static uint32_t* __fastcall C_0082e598_GetField0018(C_0082e598* self, void* /*edx*/, uint32_t* out)
{
    *out = self->field_0018;
    return out;
}

HOOK(0x41E200, C_0082e598_GetField0018, "auto");
