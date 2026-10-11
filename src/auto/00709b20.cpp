#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, AssignStringSubstring, 0x004109E0, (void* self, void* edx, const void* source, std::uint32_t position, std::uint32_t count));

// Copy-constructs a water parameter block from another one (plain fields, an embedded string, more plain fields).
static void* __fastcall WaterParams_CopyConstruct(void* self, void* /*edx*/, void* source)
{
    uint8_t* dst = reinterpret_cast<uint8_t*>(self);
    uint8_t* src = reinterpret_cast<uint8_t*>(source);

    for (uint32_t off = 0; off < 0x30; off += 4) {
        CSF_AT(uint32_t, dst, off) = CSF_AT(uint32_t, src, off);
    }

    // Initialize the embedded string at +0x30 (empty, small-buffer capacity 15), then assign.
    uint8_t* str = dst + 0x30;
    CSF_AT(uint32_t, str, 0x14) = 0;
    CSF_AT(uint32_t, str, 0x18) = 15;
    CSF_AT(uint8_t, str, 4) = 0;
    AssignStringSubstring(str, nullptr, src + 0x30, 0, static_cast<std::uint32_t>(-1));

    CSF_AT(uint32_t, dst, 0x4C) = CSF_AT(uint32_t, src, 0x4C);
    CSF_AT(uint32_t, dst, 0x50) = CSF_AT(uint32_t, src, 0x50);
    CSF_AT(uint32_t, dst, 0x54) = CSF_AT(uint32_t, src, 0x54);
    CSF_AT(uint32_t, dst, 0x58) = CSF_AT(uint32_t, src, 0x58);

    std::memcpy(dst + 0x5C, src + 0x5C, 0x30);

    return self;
}

HOOK(0x00709B20, WaterParams_CopyConstruct, "auto");
