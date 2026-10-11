#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

static std::uint32_t* __fastcall S_006cb160_Initialize(
    S_006cb160* self, void* /*edx*/)
{
    self->field_000c = 10;
    self->field_0000 = 0;
    self->field_0004 = 0;
    self->field_0008 = 0;

    CSF_AT(std::uint32_t, self, 0x18) = 0;
    CSF_AT(std::uint32_t, self, 0x1C) = 0;
    CSF_AT(std::uint32_t, self, 0x20) = 0;
    CSF_AT(std::uint32_t, self, 0x24) = 0;
    CSF_AT(std::uint32_t, self, 0x28) = 0;
    CSF_AT(std::uint32_t, self, 0x2C) = 0;
    CSF_AT(std::uint32_t, self, 0x30) = 0;
    CSF_AT(std::uint32_t, self, 0x34) = 0;
    CSF_AT(std::uint32_t, self, 0x38) = 0;
    CSF_AT(std::uint32_t, self, 0x3C) = 0;
    CSF_AT(std::uint32_t, self, 0x40) = 0;
    CSF_AT(std::uint32_t, self, 0x44) = 0;
    CSF_AT(std::uint32_t, self, 0x48) = 0;
    CSF_AT(std::uint32_t, self, 0x4C) = 0;
    CSF_AT(std::uint32_t, self, 0x50) = 0;
    CSF_AT(std::uint32_t, self, 0x54) = 0;
    CSF_AT(std::uint32_t, self, 0x58) = 0;
    CSF_AT(std::uint32_t, self, 0x5C) = 0;
    CSF_AT(std::uint32_t, self, 0x60) = 0;
    CSF_AT(std::uint32_t, self, 0x64) = 0;

    self->field_006c = 0;
    self->field_0070 = 0;
    self->field_0074 = 0;
    CSF_AT(std::uint32_t, self, 0x78) = 0;
    CSF_AT(std::uint32_t, self, 0x7C) = 0;
    CSF_AT(std::uint32_t, self, 0x80) = 0;
    CSF_AT(std::uint32_t, self, 0x84) = 0;
    CSF_AT(std::uint32_t, self, 0x88) = 0;
    CSF_AT(std::uint32_t, self, 0x8C) = 0;
    CSF_AT(std::uint32_t, self, 0x90) = 0;
    CSF_AT(std::uint32_t, self, 0x94) = 0;
    CSF_AT(std::uint32_t, self, 0x98) = 0;

    self->field_009c = 0;
    self->field_00a0 = 0;
    self->field_00a4 = 0;
    self->field_00a8 = 0;
    self->field_00ac = 0;
    self->field_00b0 = 0;
    self->field_00b4 = 0;
    self->field_00b8 = 0;
    self->field_00bc = 0;

    CSF_AT(std::uint32_t, self, 0xC0) = 0;
    CSF_AT(std::uint32_t, self, 0xC4) = 0;
    CSF_AT(std::uint32_t, self, 0xC8) = 0;
    CSF_AT(std::uint32_t, self, 0xCC) = 0;
    CSF_AT(std::uint32_t, self, 0xD0) = 0;
    CSF_AT(std::uint32_t, self, 0xD4) = 0;
    CSF_AT(std::uint32_t, self, 0xD8) = 0;
    CSF_AT(std::uint32_t, self, 0xDC) = 0;
    CSF_AT(std::uint32_t, self, 0xE0) = 0;
    CSF_AT(std::uint32_t, self, 0xE4) = 0;
    CSF_AT(std::uint32_t, self, 0xE8) = 0;
    CSF_AT(std::uint32_t, self, 0xEC) = 0;

    return &self->field_0000;
}

HOOK(0x006CB160, S_006cb160_Initialize, "auto");
