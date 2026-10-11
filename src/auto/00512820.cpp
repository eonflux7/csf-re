#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, AssignStringSubstring, 0x004109E0, (void* self, void* edx, const void* source, std::uint32_t position, std::uint32_t count));
CSF_FUNC(void*, __fastcall, AssignStringRange_488830, 0x00488830, (void* self, void* edx, void* source, std::uint32_t position, std::uint32_t count));
CSF_FUNC(void*, __fastcall, CopyConstructStringPairRecord, 0x0071B460, (void* self, void* , const void* source));

static int __fastcall S_004f21d0_CopyConstruct(
    S_004f21d0* self, void* /*edx*/, S_004f21d0* source)
{
    self->field_0018 = 7;
    self->field_0014 = 0;
    self->field_0004 = 0;
    AssignStringRange_488830(self, nullptr, source, 0, 0xFFFFFFFF);

    self->field_001c = source->field_001c;
    CSF_AT(std::uint32_t, self, 0x20) = CSF_AT(std::uint32_t, source, 0x20);
    CSF_AT(std::uint32_t, self, 0x24) = CSF_AT(std::uint32_t, source, 0x24);
    CSF_AT(std::uint32_t, self, 0x28) = CSF_AT(std::uint32_t, source, 0x28);
    CSF_AT(std::uint32_t, self, 0x2C) = CSF_AT(std::uint32_t, source, 0x2C);
    CSF_AT(std::uint32_t, self, 0x30) = CSF_AT(std::uint32_t, source, 0x30);
    self->field_0034 = source->field_0034;
    self->field_0038 = source->field_0038;
    CSF_AT(std::uint32_t, self, 0x3C) = CSF_AT(std::uint32_t, source, 0x3C);
    CSF_AT(std::uint32_t, self, 0x40) = CSF_AT(std::uint32_t, source, 0x40);
    CSF_AT(std::uint32_t, self, 0x44) = CSF_AT(std::uint32_t, source, 0x44);
    CSF_AT(std::uint32_t, self, 0x48) = CSF_AT(std::uint32_t, source, 0x48);
    CSF_AT(std::uint32_t, self, 0x4C) = CSF_AT(std::uint32_t, source, 0x4C);

    CSF_AT(std::uint32_t, self, 0x68) = 15;
    CSF_AT(std::uint32_t, self, 0x64) = 0;
    CSF_AT(std::uint8_t, self, 0x54) = 0;
    AssignStringSubstring(
        reinterpret_cast<std::uint8_t*>(self) + 0x50,
        nullptr,
        reinterpret_cast<const std::uint8_t*>(source) + 0x50,
        0, 0xFFFFFFFF);

    CopyConstructStringPairRecord(
        reinterpret_cast<std::uint8_t*>(self) + 0x6C,
        nullptr,
        reinterpret_cast<const std::uint8_t*>(source) + 0x6C);

    self->field_00ac = source->field_00ac;
    self->field_00b0 = source->field_00b0;
    self->field_00b4 = source->field_00b4;
    self->field_00b8 = source->field_00b8;
    self->field_00bc = source->field_00bc;
    self->field_00c0 = source->field_00c0;
    self->field_00c4 = source->field_00c4;

    return static_cast<int>(reinterpret_cast<std::intptr_t>(self));
}

HOOK(0x00512820, S_004f21d0_CopyConstruct, "auto");
