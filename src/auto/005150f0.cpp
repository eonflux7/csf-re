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

// std::_String_base::AssignStringRange-like helper (ret 12, __fastcall: self, source, position, count)
CSF_FUNC(void*, __fastcall, AssignStringRange_488830, 0x00488830,
         (void* self, void* edx, void* source, std::uint32_t position, std::uint32_t count));

// Copies an embedded 0x18-byte block (a2+0x34 -> self+0x34). Name chosen here; signature fixed by the packet.
CSF_FUNC(int, __fastcall, ObjectivePanel_CopyEmbeddedBlock, 0x00514B10,
         (void* self, void* edx, int a4a));

// __thiscall(this, a2) pops 4 bytes -> bound as __fastcall with one stack argument.
// The source is a std::_String_base object that is not declared in our headers, so it is
// addressed as raw bytes via CSF_AT at the offsets the original reads.
static int __fastcall ObjectivePanel_Assign(S_004f21d0* self, void* /*edx*/, void* a2)
{
    AssignStringRange_488830(self, nullptr, a2, 0, 0xFFFFFFFFu);

    // Six consecutive dwords copied from source +0x1c..+0x30 into self +0x1c..+0x30.
    self->field_001c = CSF_AT(std::uint32_t, a2, 0x1c);
    CSF_AT(std::uint32_t, self, 0x20) = CSF_AT(std::uint32_t, a2, 0x20);
    CSF_AT(std::uint32_t, self, 0x24) = CSF_AT(std::uint32_t, a2, 0x24);
    CSF_AT(std::uint32_t, self, 0x28) = CSF_AT(std::uint32_t, a2, 0x28);
    CSF_AT(std::uint32_t, self, 0x2c) = CSF_AT(std::uint32_t, a2, 0x2c);
    CSF_AT(std::uint32_t, self, 0x30) = CSF_AT(std::uint32_t, a2, 0x30);

    // Embedded sub-object at +0x34: copy via helper, passing its source address.
    ObjectivePanel_CopyEmbeddedBlock(
        reinterpret_cast<uint8_t*>(self) + 0x34, nullptr,
        reinterpret_cast<int>(reinterpret_cast<uint8_t*>(a2) + 0x34));

    // Low byte of field_0044 only.
    self->field_0044 = (self->field_0044 & 0xFFFFFF00u) | CSF_AT(std::uint8_t, a2, 0x44);

    self->field_0048 = CSF_AT(std::uint32_t, a2, 0x48);

    // Byte 0 and byte 1 of field_004c individually.
    self->field_004c = (self->field_004c & 0xFFFF0000u)
                     | (static_cast<std::uint32_t>(CSF_AT(std::uint8_t, a2, 0x4c)))
                     | (static_cast<std::uint32_t>(CSF_AT(std::uint8_t, a2, 0x4d)) << 8);

    return reinterpret_cast<int>(self);
}

HOOK(0x005150F0, ObjectivePanel_Assign, "auto");
