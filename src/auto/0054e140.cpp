#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

static int __fastcall C_008443c8_SerializeInt16(
    C_008443c8* self, void* /*edx*/, int16_t* value, int context)
{
    const uint8_t writeMode = CSF_AT(uint8_t, context, 0x14);
    void* serializer = reinterpret_cast<void*>(self->field_0004);
    void** vtable = CSF_AT(void**, serializer, 0);

    uint8_t success;
    if (writeMode)
    {
        success = reinterpret_cast<uint8_t(__fastcall*)(void*, void*, int)>(
            vtable[0x4C / 4])(serializer, nullptr, static_cast<int>(*value));
    }
    else
    {
        success = reinterpret_cast<uint8_t(__fastcall*)(void*, void*, int16_t*)>(
            vtable[0x2C / 4])(serializer, nullptr, value);
    }
    return (success != 0) - 1;
}

HOOK(0x0054E140, C_008443c8_SerializeInt16, "auto");
