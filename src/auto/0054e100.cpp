#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

static int __fastcall C_008443c8_ValidateByContext(
    C_008443c8* self, void* /*edx*/, uint32_t* value, int context)
{
    const uint8_t useIndirect = CSF_AT(uint8_t, reinterpret_cast<void*>(context), 0x14);
    void* validator = reinterpret_cast<void*>(self->field_0004);
    void** vtable = CSF_AT(void**, validator, 0);

    // The original leaves the validator object in ecx and pops its own stack
    // argument (__stdcall-style), so the callee is bound as __thiscall.
    uint8_t valid;
    if (useIndirect)
    {
        valid = reinterpret_cast<uint8_t (__thiscall*)(void*, uint32_t)>(
            vtable[0x3c / 4])(validator, *value);
    }
    else
    {
        valid = reinterpret_cast<uint8_t (__thiscall*)(void*, uint32_t*)>(
            vtable[0x1c / 4])(validator, value);
    }
    return (valid != 0) - 1;
}

HOOK(0x0054E100, C_008443c8_ValidateByContext, "auto");
