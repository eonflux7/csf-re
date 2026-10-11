#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

// This internal routine takes its sole argument in EAX, not ECX.
CSF_FUNC(void*, __attribute__((regparm(1))), CoreZlibLog_FlushDeflateBits,
         0x006A4420, (void* state));

static int CoreZlibLog_EmitBufferedWord(void* state)
{
    std::uint32_t position = CSF_AT(std::uint32_t, state, 0x14);
    const std::uint8_t lowByte = CSF_AT(std::uint8_t, state, 0x16B0);
    auto* buffer = CSF_AT(std::uint8_t*, state, 0x08);
    buffer[position] = lowByte;

    position = CSF_AT(std::uint32_t, state, 0x14);
    const std::uint8_t highByte = CSF_AT(std::uint8_t, state, 0x16B1);
    buffer = CSF_AT(std::uint8_t*, state, 0x08);
    ++position;
    CSF_AT(std::uint32_t, state, 0x14) = position;
    buffer[position] = highByte;

    const int validBits = CSF_AT(int, state, 0x16B4);
    ++CSF_AT(std::uint32_t, state, 0x14);
    return validBits;
}

static void CoreZlibLog_AppendEmptyFixedBlock(void* state)
{
    int validBits = CSF_AT(int, state, 0x16B4);
    CSF_AT(std::uint16_t, state, 0x16B0) |=
        static_cast<std::uint16_t>(2u << (static_cast<unsigned>(validBits) & 31));

    if (validBits <= 13)
    {
        CSF_AT(int, state, 0x16B4) = validBits + 3;
    }
    else
    {
        validBits = CoreZlibLog_EmitBufferedWord(state);
        CSF_AT(std::uint16_t, state, 0x16B0) =
            static_cast<std::uint16_t>(
                2u >> ((16u - static_cast<unsigned>(validBits)) & 31));
        CSF_AT(int, state, 0x16B4) = validBits - 13;
    }

    // The fixed-tree end-of-block symbol is seven zero bits.
    validBits = CSF_AT(int, state, 0x16B4);
    CSF_AT(std::uint16_t, state, 0x16B0) =
        CSF_AT(std::uint16_t, state, 0x16B0);
    if (validBits <= 9)
    {
        CSF_AT(int, state, 0x16B4) = validBits + 7;
    }
    else
    {
        validBits = CoreZlibLog_EmitBufferedWord(state);
        CSF_AT(std::uint16_t, state, 0x16B0) = 0;
        CSF_AT(int, state, 0x16B4) = validBits - 9;
    }
}

static int __cdecl CoreZlibLog_AlignDeflateBlock(void* state)
{
    CoreZlibLog_AppendEmptyFixedBlock(state);
    state = CoreZlibLog_FlushDeflateBits(state);

    const int validBits = CSF_AT(int, state, 0x16B4);
    if (CSF_AT(int, state, 0x16AC) - validBits + 11 < 9)
    {
        CoreZlibLog_AppendEmptyFixedBlock(state);
        state = CoreZlibLog_FlushDeflateBits(state);
    }

    CSF_AT(int, state, 0x16AC) = 7;
    return reinterpret_cast<int>(state);
}

HOOK(0x006A49C0, CoreZlibLog_AlignDeflateBlock, "auto");
