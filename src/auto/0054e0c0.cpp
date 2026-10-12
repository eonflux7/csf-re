#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

static int __fastcall C_008443c8_ValidateValue(
    C_008443c8* self, void* /*edx*/, uint32_t* value, int context)
{
    const uint8_t indirect = CSF_AT(uint8_t, reinterpret_cast<void*>(context), 0x14);
    void* validator = reinterpret_cast<void*>(self->field_0004);
    void** vtable = CSF_AT(void**, validator, 0);

    uint8_t valid;
    if (indirect)
    {
        valid = reinterpret_cast<uint8_t (__fastcall*)(void*, void*, uint32_t)>(
            vtable[0x44 / 4])(validator, nullptr, *value);
    }
    else
    {
        valid = reinterpret_cast<uint8_t (__fastcall*)(void*, void*, uint32_t*)>(
            vtable[0x24 / 4])(validator, nullptr, value);
    }
    return (valid != 0) - 1;
}

HOOK(0x0054E0C0, C_008443c8_ValidateValue, "auto");
