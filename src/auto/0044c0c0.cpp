#include <cstdint>
#include "globals.h"
#include <cstddef>
#include "hook.h"

// Binds to the string assignment routine (__fastcall, 3 stack args, ret 12).
CSF_FUNC(void*, __fastcall, AssignStringSubstring, 0x004109E0,
         (void* self, void* edx, const void* source, std::uint32_t position, std::uint32_t count));

// Copies a 0x64-byte record from `source` into `self`, skipping the members the original leaves alone,
// assigns the embedded string at +0x40 from `source`, then copies the trailing two dwords.
static char* __fastcall CopyRecordFromSource(char* self, void* /*edx*/, char* source) {
    std::uint8_t* dst = reinterpret_cast<std::uint8_t*>(self);
    std::uint8_t* src = reinterpret_cast<std::uint8_t*>(source);

    CSF_AT(std::uint32_t, dst, 0x00) = CSF_AT(std::uint32_t, src, 0x00);
    CSF_AT(std::uint32_t, dst, 0x04) = CSF_AT(std::uint32_t, src, 0x04);
    CSF_AT(std::uint32_t, dst, 0x08) = CSF_AT(std::uint32_t, src, 0x08);
    CSF_AT(std::uint32_t, dst, 0x0C) = CSF_AT(std::uint32_t, src, 0x0C);
    CSF_AT(std::uint32_t, dst, 0x14) = CSF_AT(std::uint32_t, src, 0x14);
    CSF_AT(std::uint32_t, dst, 0x18) = CSF_AT(std::uint32_t, src, 0x18);
    CSF_AT(std::uint32_t, dst, 0x1C) = CSF_AT(std::uint32_t, src, 0x1C);
    CSF_AT(std::uint32_t, dst, 0x20) = CSF_AT(std::uint32_t, src, 0x20);
    CSF_AT(std::uint32_t, dst, 0x24) = CSF_AT(std::uint32_t, src, 0x24);
    CSF_AT(std::uint8_t, dst, 0x28) = CSF_AT(std::uint8_t, src, 0x28);
    CSF_AT(std::uint32_t, dst, 0x2C) = CSF_AT(std::uint32_t, src, 0x2C);
    CSF_AT(std::uint32_t, dst, 0x30) = CSF_AT(std::uint32_t, src, 0x30);
    CSF_AT(std::uint32_t, dst, 0x34) = CSF_AT(std::uint32_t, src, 0x34);
    CSF_AT(std::uint32_t, dst, 0x38) = CSF_AT(std::uint32_t, src, 0x38);
    CSF_AT(std::uint32_t, dst, 0x3C) = CSF_AT(std::uint32_t, src, 0x3C);

    // Embedded string object at +0x40: AssignStringSubstring(this+0x40, source+0x40, 0, -1)
    AssignStringSubstring(dst + 0x40, nullptr, src + 0x40, 0u, 0xFFFFFFFFu);

    CSF_AT(std::uint32_t, dst, 0x5C) = CSF_AT(std::uint32_t, src, 0x5C);
    CSF_AT(std::uint32_t, dst, 0x60) = CSF_AT(std::uint32_t, src, 0x60);

    return self;
}

HOOK(0x0044C0C0, CopyRecordFromSource, "auto");
