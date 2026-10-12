#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

static int __fastcall C_008443c8_SerializeByte(
    C_008443c8* self, void* /*edx*/, std::uint8_t* value, int context)
{
    const std::uint8_t scalarMode =
        CSF_AT(std::uint8_t, reinterpret_cast<void*>(context), 0x14);
    void* serializer = reinterpret_cast<void*>(self->field_0004);
    void** vtable = CSF_AT(void**, serializer, 0);

    if (scalarMode)
    {
        const bool success =
            reinterpret_cast<bool(__fastcall*)(void*, void*, unsigned int)>(
                vtable[0x34 / 4])(serializer, nullptr, *value);
        return static_cast<int>(success) - 1;
    }

    const bool success =
        reinterpret_cast<bool(__fastcall*)(void*, void*, std::uint8_t*)>(
            vtable[0x14 / 4])(serializer, nullptr, value);
    return static_cast<int>(success) - 1;
}

HOOK(0x0054E200, C_008443c8_SerializeByte, "auto");
