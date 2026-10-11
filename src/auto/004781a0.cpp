#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(float*, __fastcall, NormalizeVector3, 0x006AB8D0, (void* self, void* edx));

static float* __fastcall C_00833b88_CopyAndNormalizeVector3(
    C_00833b88* self, void* /*edx*/, const uint32_t* source)
{
    self->vtbl = reinterpret_cast<void**>(source[0]);
    self->field_0004 = source[1];
    CSF_AT(uint32_t, self, 0x008) = source[2];
    return NormalizeVector3(self, self);
}

HOOK(0x004781A0, C_00833b88_CopyAndNormalizeVector3, "auto");
