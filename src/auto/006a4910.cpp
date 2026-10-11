#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

typedef std::uint32_t* (__cdecl *CoreZlibLog_CopyStoredBlockFn)(
    void* state, std::uint8_t* source, int length, int writeHeader)
    __attribute__((regparm(3)));

static const CoreZlibLog_CopyStoredBlockFn CoreZlibLog_CopyStoredBlock =
    reinterpret_cast<CoreZlibLog_CopyStoredBlockFn>(
        CSF_ADDR_FN(0x006A4510));

static std::uint32_t* __cdecl CoreZlibLog_EmitStoredBlock(
    void* state, std::uint8_t* source, int length, int blockFlags)
{
    const int bitCount = CSF_AT(int, state, 0x16B4);
    const std::uint32_t shiftedFlags =
        static_cast<std::uint32_t>(blockFlags) <<
        (static_cast<std::uint32_t>(bitCount) & 31);

    if (bitCount <= 13) {
        CSF_AT(int, state, 0x16B4) = bitCount + 3;
        CSF_AT(std::uint16_t, state, 0x16B0) |=
            static_cast<std::uint16_t>(shiftedFlags);
    } else {
        std::uint8_t* pendingBuffer =
            CSF_AT(std::uint8_t*, state, 0x08);

        CSF_AT(std::uint16_t, state, 0x16B0) |=
            static_cast<std::uint16_t>(shiftedFlags);

        const std::uint32_t firstIndex =
            CSF_AT(std::uint32_t, state, 0x14);
        const std::uint8_t lowByte =
            CSF_AT(std::uint8_t, state, 0x16B0);
        pendingBuffer[firstIndex] = lowByte;

        const std::uint32_t secondIndex =
            CSF_AT(std::uint32_t, state, 0x14) + 1;
        const std::uint8_t highByte =
            CSF_AT(std::uint8_t, state, 0x16B1);
        pendingBuffer = CSF_AT(std::uint8_t*, state, 0x08);

        CSF_AT(std::uint32_t, state, 0x14) = secondIndex;
        pendingBuffer[secondIndex] = highByte;

        const int remainingBitCount = CSF_AT(int, state, 0x16B4);
        const std::uint32_t pendingCount =
            CSF_AT(std::uint32_t, state, 0x14) + 1;
        const std::uint32_t shift =
            (16u - static_cast<std::uint32_t>(remainingBitCount)) & 31;

        CSF_AT(std::uint16_t, state, 0x16B0) =
            static_cast<std::uint16_t>(
                static_cast<std::uint16_t>(blockFlags) >> shift);
        CSF_AT(std::uint32_t, state, 0x14) = pendingCount;
        CSF_AT(int, state, 0x16B4) = remainingBitCount - 13;
    }

    return CoreZlibLog_CopyStoredBlock(state, source, length, 1);
}

HOOK(0x006A4910, CoreZlibLog_EmitStoredBlock, "auto");
