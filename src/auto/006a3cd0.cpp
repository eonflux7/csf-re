#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

typedef void* CoreZlibLog_SendTreeFunction(void*, void*, int)
    __attribute__((regparm(3)));

static CoreZlibLog_SendTreeFunction* const CoreZlibLog_SendTree =
    reinterpret_cast<CoreZlibLog_SendTreeFunction*>(CSF_ADDR_FN(0x006A3770));

using CodeLengthOrder = std::uint8_t[19];
CSF_GLOBAL(CodeLengthOrder, g_CodeLengthOrder, 0x0085EF94);

static void CoreZlibLog_SendHeaderBits(
    void* state, std::uint32_t value, int width, int maximumBufferedBits)
{
    const int bufferedBits = CSF_AT(int, state, 0x16B4);
    const std::uint16_t shifted = static_cast<std::uint16_t>(
        value << (static_cast<unsigned>(bufferedBits) & 31));

    if (bufferedBits <= maximumBufferedBits)
    {
        CSF_AT(std::uint16_t, state, 0x16B0) |= shifted;
        CSF_AT(int, state, 0x16B4) = bufferedBits + width;
        return;
    }

    const std::uint32_t pending = CSF_AT(std::uint32_t, state, 0x14);
    CSF_AT(std::uint16_t, state, 0x16B0) |= shifted;

    const std::uint8_t lowByte = CSF_AT(std::uint8_t, state, 0x16B0);
    CSF_AT(std::uint8_t*, state, 0x8)[pending] = lowByte;

    const std::uint8_t highByte = CSF_AT(std::uint8_t, state, 0x16B1);
    const std::uint32_t nextPending =
        CSF_AT(std::uint32_t, state, 0x14) + 1;
    CSF_AT(std::uint32_t, state, 0x14) = nextPending;
    CSF_AT(std::uint8_t*, state, 0x8)[nextPending] = highByte;

    const int previousBits = CSF_AT(int, state, 0x16B4);
    ++CSF_AT(std::uint32_t, state, 0x14);
    CSF_AT(int, state, 0x16B4) = previousBits - maximumBufferedBits;
    CSF_AT(std::uint16_t, state, 0x16B0) =
        static_cast<std::uint16_t>(
            static_cast<std::uint16_t>(value) >>
            ((16u - static_cast<unsigned>(previousBits)) & 31));
}

static void* __cdecl __attribute__((regparm(1)))
CoreZlibLog_SendDynamicTrees(
    void* state, int literalCodeCount, int distanceCodeCount,
    int bitLengthCodeCount)
{
    CoreZlibLog_SendHeaderBits(
        state, static_cast<std::uint32_t>(literalCodeCount - 257), 5, 11);
    CoreZlibLog_SendHeaderBits(
        state, static_cast<std::uint32_t>(distanceCodeCount - 1), 5, 11);
    CoreZlibLog_SendHeaderBits(
        state, static_cast<std::uint32_t>(bitLengthCodeCount - 4), 4, 12);

    for (int i = 0; i < bitLengthCodeCount; ++i)
    {
        const unsigned code = g_CodeLengthOrder[i];
        const std::uint16_t length =
            CSF_AT(std::uint16_t, state, 0xA76 + 4 * code);
        CoreZlibLog_SendHeaderBits(state, length, 3, 13);
    }

    void* nextState = CoreZlibLog_SendTree(
        state, static_cast<std::uint8_t*>(state) + 140,
        literalCodeCount - 1);
    return CoreZlibLog_SendTree(
        nextState, static_cast<std::uint8_t*>(nextState) + 2432,
        distanceCodeCount - 1);
}

HOOK(0x006A3CD0, CoreZlibLog_SendDynamicTrees, "auto");
