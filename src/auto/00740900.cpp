#include <cstdint>
#include "hook.h"

// Copies a 0x20D-byte header block (one leading byte plus 256 bytes), two dwords,
// and 128 words from `source` into `self`. Returns a pointer just past the last word written.
static uint16_t* __fastcall NetCogibleDdl_CopyRecord(uint8_t* self, void* /*edx*/, const uint8_t* source)
{
    // Byte copy from offset 0 through 256, forward order.
    self[0] = source[0];
    for (int i = 1; i <= 256; ++i) {
        self[i] = source[i];
    }

    // Two dword fields at +0x104 and +0x108.
    *reinterpret_cast<uint32_t*>(self + 0x104) = *reinterpret_cast<const uint32_t*>(source + 0x104);
    *reinterpret_cast<uint32_t*>(self + 0x108) = *reinterpret_cast<const uint32_t*>(source + 0x108);

    // 128 words starting at +0x10C, forward order.
    uint16_t* dst = reinterpret_cast<uint16_t*>(self + 0x10C);
    for (int i = 0; i < 128; ++i) {
        dst[i] = *reinterpret_cast<const uint16_t*>(source + 0x10C + i * 2);
    }

    return dst + 128;
}

HOOK(0x740900, NetCogibleDdl_CopyRecord, "auto");
