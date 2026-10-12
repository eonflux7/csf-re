#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(char, __fastcall, C_0082e520_PrepareInputForBinding, 0x0041E050, (C_0082e520* self, void* /*edx*/, int a2, int a3));

static char __fastcall C_0082dc88_InitInputBindingState(C_0082dc88* self, void* /*edx*/, int a2, int a3)
{
    char result = C_0082e520_PrepareInputForBinding(reinterpret_cast<C_0082e520*>(self), nullptr, a2, a3);
    self->field_0039 = 1;
    self->field_0034 = 0;
    self->field_0034 |= 1u;
    return result;
}

HOOK(0x0040D140, C_0082dc88_InitInputBindingState, "auto");
