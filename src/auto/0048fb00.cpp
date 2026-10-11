#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

static uint8_t __fastcall C_00833908_ClearStateBytes(
    C_00833908* self, void* /*edx*/)
{
    CSF_AT(uint8_t, reinterpret_cast<void*>(self->field_00ac), 0x02) = 0;
    CSF_AT(uint8_t, reinterpret_cast<void*>(self->field_00ac), 0x03) = 0;
    CSF_AT(uint8_t, reinterpret_cast<void*>(self->field_00ac), 0x04) = 0;
    CSF_AT(uint8_t, reinterpret_cast<void*>(self->field_00ac), 0x07) = 0;
    CSF_AT(uint8_t, reinterpret_cast<void*>(self->field_00ac), 0x08) = 0;
    CSF_AT(uint8_t, reinterpret_cast<void*>(self->field_00ac), 0x09) = 0;
    CSF_AT(uint8_t, reinterpret_cast<void*>(self->field_00ac), 0x0A) = 0;
    CSF_AT(uint8_t, reinterpret_cast<void*>(self->field_00ac), 0x0B) = 0;
    CSF_AT(uint8_t, reinterpret_cast<void*>(self->field_00ac), 0x0C) = 0;
    CSF_AT(uint8_t, reinterpret_cast<void*>(self->field_00ac), 0x0D) = 0;
    CSF_AT(uint8_t, reinterpret_cast<void*>(self->field_00ac), 0x0E) = 0;
    CSF_AT(uint8_t, reinterpret_cast<void*>(self->field_00ac), 0x0F) = 0;
    CSF_AT(uint8_t, reinterpret_cast<void*>(self->field_00ac), 0x11) = 0;
    CSF_AT(uint8_t, reinterpret_cast<void*>(self->field_00ac), 0x12) = 0;
    CSF_AT(uint8_t, reinterpret_cast<void*>(self->field_00ac), 0x13) = 0;
    CSF_AT(uint8_t, reinterpret_cast<void*>(self->field_00ac), 0x14) = 0;
    CSF_AT(uint8_t, reinterpret_cast<void*>(self->field_00ac), 0x15) = 0;
    CSF_AT(uint8_t, reinterpret_cast<void*>(self->field_00ac), 0x16) = 0;
    CSF_AT(uint8_t, reinterpret_cast<void*>(self->field_00ac), 0x18) = 0;
    CSF_AT(uint8_t, reinterpret_cast<void*>(self->field_00ac), 0x19) = 0;
    CSF_AT(uint8_t, reinterpret_cast<void*>(self->field_00ac), 0x1A) = 0;
    return 0;
}

HOOK(0x0048FB00, C_00833908_ClearStateBytes, "auto");
