#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

// Folds one byte into a running CRC-32 style checksum. The checksum lives at *checksum;
// the updated value is written back and returned. The 256-entry dword table sits at 0x008A6790.
static int __cdecl NetProfile_AccumulateChecksumByte(uint8_t inputByte, int* checksum)
{
    const uint32_t* checksumTable = reinterpret_cast<const uint32_t*>(CSF_ADDR_DATA(0x008A6790));
    uint32_t currentChecksum = static_cast<uint32_t>(*checksum);
    uint32_t tableIndex = (currentChecksum & 0xFFu) ^ static_cast<uint32_t>(inputByte);
    uint32_t updated = checksumTable[tableIndex] ^ (currentChecksum >> 8);
    *checksum = static_cast<int>(updated);
    return static_cast<int>(updated);
}

HOOK(0x0069D7B0, NetProfile_AccumulateChecksumByte, "auto");
