#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// Bound callees (signatures copied from the packet).
CSF_FUNC(void*, __fastcall, AssignStringRange_488830, 0x00488830,
         (void* self, void* edx, void* source, std::uint32_t position, std::uint32_t count));
CSF_FUNC(int*, __fastcall, sub_516490, 0x00516490, (void* self, void* edx, int a2));

// Copies the state of a source string/record into this object (ctor-style init).
// Original: int __thiscall sub_5167D0(S_004f21d0* this, std::_String_base* a2); ret 4.
static int __fastcall S_004f21d0_InitFromSource(S_004f21d0* self, void* /*edx*/, void* source)
{
    self->field_0018 = 7;
    self->field_0014 = 0;
    self->field_0004 = 0;

    AssignStringRange_488830(self, nullptr, source, 0, 0xFFFFFFFFu);

    // Six consecutive dwords, source offsets 0x1C..0x30 copied to the same offsets.
    self->field_001c = CSF_AT(std::uint32_t, source, 0x1C);
    CSF_AT(std::uint32_t, self, 0x20) = CSF_AT(std::uint32_t, source, 0x20);
    CSF_AT(std::uint32_t, self, 0x24) = CSF_AT(std::uint32_t, source, 0x24);
    CSF_AT(std::uint32_t, self, 0x28) = CSF_AT(std::uint32_t, source, 0x28);
    CSF_AT(std::uint32_t, self, 0x2C) = CSF_AT(std::uint32_t, source, 0x2C);
    CSF_AT(std::uint32_t, self, 0x30) = CSF_AT(std::uint32_t, source, 0x30);

    // Embedded sub-object at +0x34 initialised from source +0x34.
    sub_516490(&self->field_0034, nullptr, static_cast<int>(reinterpret_cast<std::uintptr_t>(
        static_cast<std::uint8_t*>(source) + 0x34)));

    // Low byte only of field_0044 is written (LOBYTE), upper bytes preserved.
    *reinterpret_cast<std::uint8_t*>(&self->field_0044) = CSF_AT(std::uint8_t, source, 0x44);

    self->field_0048 = CSF_AT(std::uint32_t, source, 0x48);

    // Two low bytes of field_004c written individually (LOBYTE / BYTE1).
    reinterpret_cast<std::uint8_t*>(&self->field_004c)[0] = CSF_AT(std::uint8_t, source, 0x4C);
    reinterpret_cast<std::uint8_t*>(&self->field_004c)[1] = CSF_AT(std::uint8_t, source, 0x4D);

    return static_cast<int>(reinterpret_cast<std::uintptr_t>(self));
}

HOOK(0x5167D0, S_004f21d0_InitFromSource, "auto");
