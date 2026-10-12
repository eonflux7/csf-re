#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// void* AssignStringSubstring(this, source, position, count) - __thiscall, ret 12
CSF_FUNC(void*, __fastcall, AssignStringSubstring, 0x004109E0,
         (void* self, void* edx, const void* source, std::uint32_t position, std::uint32_t count));

// C_0082d410 *__thiscall WaterEntry_PrepareFromDescriptor(C_0082d410 *this, descriptor *a2)
// Copies a 0x78-byte descriptor block into this object, then assigns the string
// member at +0x30 from the descriptor's string at +0x30 (full length, position 0).
static C_0082d410* __fastcall WaterEntry_PrepareFromDescriptor(C_0082d410* self, void* /*edx*/, void* descriptor)
{
    auto* src = reinterpret_cast<uint8_t*>(descriptor);

    // Header dwords: vtable slot and the two named fields.
    CSF_AT(uint32_t, self, 0x000) = CSF_AT(uint32_t, src, 0x000);
    self->field_0004 = CSF_AT(uint32_t, src, 0x004);
    self->field_0008 = CSF_AT(uint32_t, src, 0x008);

    // Dwords at +0x0C .. +0x2C.
    for (std::size_t off = 0x00C; off <= 0x02C; off += 4) {
        CSF_AT(uint32_t, self, off) = CSF_AT(uint32_t, src, off);
    }

    // String member at +0x30 assigned from the descriptor's string at +0x30.
    AssignStringSubstring(reinterpret_cast<uint8_t*>(self) + 0x30, nullptr,
                          src + 0x30, 0u, 0xFFFFFFFFu);

    // Dwords at +0x4C .. +0x58.
    for (std::size_t off = 0x04C; off <= 0x058; off += 4) {
        CSF_AT(uint32_t, self, off) = CSF_AT(uint32_t, src, off);
    }

    // 0x30 bytes (12 dwords) block from +0x5C.
    std::memcpy(reinterpret_cast<uint8_t*>(self) + 0x05C, src + 0x05C, 0x30);

    return self;
}

HOOK(0x006FE280, WaterEntry_PrepareFromDescriptor, "auto");
