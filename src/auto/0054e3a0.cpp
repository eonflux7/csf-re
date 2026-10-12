#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

static int __fastcall C_008443c8_SerializeValue(
    C_008443c8* self, void* /*edx*/, uint32_t* value, int context)
{
    const uint8_t mode = CSF_AT(uint8_t, reinterpret_cast<void*>(context), 0x14);
    void* serializer = reinterpret_cast<void*>(self->field_0004);
    void** vtable = CSF_AT(void**, serializer, 0);

    if (mode)
    {
        return reinterpret_cast<int (__fastcall*)(void*, void*, uint32_t)>(
            vtable[0x40 / 4])(serializer, nullptr, *value);
    }

    return reinterpret_cast<int (__fastcall*)(void*, void*, uint32_t*)>(
        vtable[0x20 / 4])(serializer, nullptr, value);
}

HOOK(0x0054E3A0, C_008443c8_SerializeValue, "auto");
