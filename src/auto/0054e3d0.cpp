#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

static char __fastcall C_008443c8_SerializeFields(
    C_008443c8* self, void* /*edx*/, uint8_t* data, int context)
{
    const bool writing =
        CSF_AT(uint8_t, reinterpret_cast<void*>(context), 0x14) != 0;
    void* serializer = reinterpret_cast<void*>(self->field_0004);
    uint8_t result;

    if (writing)
    {
        result = static_cast<uint8_t>(
            ((int(__fastcall*)(void*, void*, uint32_t))
                CSF_AT(void**, serializer, 0)[0x34 / 4])(
                    serializer, nullptr, data[1])) & 1;

        serializer = reinterpret_cast<void*>(self->field_0004);
        result &= ((int(__fastcall*)(void*, void*, uint32_t))
            CSF_AT(void**, serializer, 0)[0x34 / 4])(
                serializer, nullptr, data[0]);

        serializer = reinterpret_cast<void*>(self->field_0004);
        result &= ((int(__fastcall*)(void*, void*, uint32_t))
            CSF_AT(void**, serializer, 0)[0x44 / 4])(
                serializer, nullptr, CSF_AT(uint32_t, data, 0x08));

        serializer = reinterpret_cast<void*>(self->field_0004);
        result &= ((int(__fastcall*)(void*, void*, uint32_t))
            CSF_AT(void**, serializer, 0)[0x40 / 4])(
                serializer, nullptr, CSF_AT(uint32_t, data, 0x04));

        serializer = reinterpret_cast<void*>(self->field_0004);
        result &= ((int(__fastcall*)(void*, void*, uint32_t))
            CSF_AT(void**, serializer, 0)[0x4C / 4])(
                serializer, nullptr, CSF_AT(uint16_t, data, 0x0C));

        serializer = reinterpret_cast<void*>(self->field_0004);
        result &= ((int(__fastcall*)(void*, void*, uint32_t))
            CSF_AT(void**, serializer, 0)[0x4C / 4])(
                serializer, nullptr, CSF_AT(uint16_t, data, 0x0E));
    }
    else
    {
        result = static_cast<uint8_t>(
            ((int(__fastcall*)(void*, void*, uint8_t*))
                CSF_AT(void**, serializer, 0)[0x14 / 4])(
                    serializer, nullptr, data + 1)) & 1;

        serializer = reinterpret_cast<void*>(self->field_0004);
        result &= ((int(__fastcall*)(void*, void*, uint8_t*))
            CSF_AT(void**, serializer, 0)[0x14 / 4])(
                serializer, nullptr, data);

        serializer = reinterpret_cast<void*>(self->field_0004);
        result &= ((int(__fastcall*)(void*, void*, uint8_t*))
            CSF_AT(void**, serializer, 0)[0x24 / 4])(
                serializer, nullptr, data + 8);

        serializer = reinterpret_cast<void*>(self->field_0004);
        result &= ((int(__fastcall*)(void*, void*, uint8_t*))
            CSF_AT(void**, serializer, 0)[0x20 / 4])(
                serializer, nullptr, data + 4);

        serializer = reinterpret_cast<void*>(self->field_0004);
        result &= ((int(__fastcall*)(void*, void*, uint8_t*))
            CSF_AT(void**, serializer, 0)[0x2C / 4])(
                serializer, nullptr, data + 12);

        serializer = reinterpret_cast<void*>(self->field_0004);
        result &= ((int(__fastcall*)(void*, void*, uint8_t*))
            CSF_AT(void**, serializer, 0)[0x2C / 4])(
                serializer, nullptr, data + 14);
    }

    return static_cast<char>(result);
}

HOOK(0x0054E3D0, C_008443c8_SerializeFields, "auto");
