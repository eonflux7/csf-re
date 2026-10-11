#include <cstdint>
#include <cstddef>
#include "hook.h"
#include "types/auto_classes.h"

// Copies `count` records of 5 dwords (20 bytes) from `source` into the
// buffer embedded at this+4, one dword at a time. Returns the source
// pointer advanced past the copied records (or `source` unchanged when it
// is null or count is zero).
static uint32_t* __fastcall C_008300c0_CopyRecordsToPad(C_008300c0* self, void* /*edx*/, uint32_t* source, uint32_t count)
{
    uint32_t* result = source;
    if (source == nullptr || count == 0)
        return result;

    uint8_t* destination = reinterpret_cast<uint8_t*>(self) + 4;
    do {
        uint32_t* dst = reinterpret_cast<uint32_t*>(destination);
        dst[0] = result[0];
        dst[1] = result[1];
        dst[2] = result[2];
        dst[3] = result[3];
        uint32_t fifthValue = result[4];
        result += 5;
        destination += 20;
        --count;
        dst[4] = fifthValue;
    } while (count != 0);

    return result;
}

HOOK(0x4288B0, C_008300c0_CopyRecordsToPad, "auto");
