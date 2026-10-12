#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(char, __fastcall, C_0082e520_PrepareInputForBinding, 0x0041E050, (C_0082e520* self, void* /*edx*/, int a2, int a3));

// Prepares input for binding through the base class, then stores the dword at a3+4 into this->field_0094.
static char __fastcall C_0082e090_PrepareInputAndStoreSourceValue(C_0082e090* self, void* /*edx*/, int a2, int a3)
{
    char result = C_0082e520_PrepareInputForBinding(reinterpret_cast<C_0082e520*>(self), nullptr, a2, a3);
    self->field_0094 = CSF_AT(uint32_t, reinterpret_cast<uint8_t*>(a3), 4);
    return result;
}

HOOK(0x00415D70, C_0082e090_PrepareInputAndStoreSourceValue, "auto");
