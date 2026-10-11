#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_GLOBAL(uint8_t, g_byte_82E65C, 0x0082E65C);
CSF_FUNC(void*, __fastcall, TextInputSubobject_Init, 0x00697130, (void* self, void* edx, uint8_t* arg));

static uint32_t* __fastcall S_0073c380_Initialize(S_0073c380* self, void* /*edx*/)
{
    self->field_0000 = 0;
    self->field_0004 = 0;
    self->field_0008 = 0;
    self->field_00c8 = 0;

    TextInputSubobject_Init(reinterpret_cast<uint8_t*>(self) + 0x0C, nullptr, &g_byte_82E65C);
    TextInputSubobject_Init(reinterpret_cast<uint8_t*>(self) + 0x28, nullptr, &g_byte_82E65C);
    TextInputSubobject_Init(reinterpret_cast<uint8_t*>(self) + 0x44, nullptr, &g_byte_82E65C);
    TextInputSubobject_Init(reinterpret_cast<uint8_t*>(self) + 0x60, nullptr, &g_byte_82E65C);
    auto* result = static_cast<uint32_t*>(
        TextInputSubobject_Init(reinterpret_cast<uint8_t*>(self) + 0x7C, nullptr, &g_byte_82E65C));

    self->field_0098 = 0;
    self->field_00cc = 0;
    self->field_00d0 = 0;
    self->field_00d4 = 0;
    self->field_00fc = 0;
    self->field_0100 = 0;
    self->field_0104 = 0;
    self->field_0108 = 0;
    self->field_010c = 0;
    self->field_0110 = 0;
    self->field_0114 = 0;
    self->field_014f = 0;
    self->field_015c = 0;
    self->field_0150 = 0;
    self->field_0154 = 0;
    self->field_0158 = 0;
    self->field_00c0 = 1065353216;
    self->field_00c4 = -1;

    return result;
}

HOOK(0x0073C380, S_0073c380_Initialize, "auto");
