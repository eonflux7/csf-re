#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

using LengthValues = int32_t[29];
using DistanceValues = int32_t[30];
using CodeLookup = uint8_t[256];

CSF_GLOBAL(LengthValues, g_LengthExtraBits, 0x0085EE58);
CSF_GLOBAL(DistanceValues, g_DistanceExtraBits, 0x0085EED0);
CSF_GLOBAL(CodeLookup, g_SmallDistanceCodes, 0x0085F4A0);
CSF_GLOBAL(CodeLookup, g_LargeDistanceCodes, 0x0085F5A0);
CSF_GLOBAL(CodeLookup, g_LengthCodes, 0x0085F6A0);
CSF_GLOBAL(LengthValues, g_LengthBases, 0x0085F7A0);
CSF_GLOBAL(DistanceValues, g_DistanceBases, 0x0085F818);

static void CoreZlibLog_SendBits(void* state, uint32_t value, int length)
{
    int validBits = CSF_AT(int32_t, state, 0x16B4);

    if (validBits <= 16 - length)
    {
        CSF_AT(uint16_t, state, 0x16B0) |=
            static_cast<uint16_t>(value << validBits);
        CSF_AT(int32_t, state, 0x16B4) = validBits + length;
        return;
    }

    uint8_t* output = CSF_AT(uint8_t*, state, 0x08);
    CSF_AT(uint16_t, state, 0x16B0) |=
        static_cast<uint16_t>(value << validBits);

    output[CSF_AT(uint32_t, state, 0x14)] =
        CSF_AT(uint8_t, state, 0x16B0);

    output = CSF_AT(uint8_t*, state, 0x08);
    uint32_t pending = CSF_AT(uint32_t, state, 0x14) + 1;
    CSF_AT(uint32_t, state, 0x14) = pending;
    output[pending] = CSF_AT(uint8_t, state, 0x16B1);

    validBits = CSF_AT(int32_t, state, 0x16B4);
    ++CSF_AT(uint32_t, state, 0x14);
    CSF_AT(int32_t, state, 0x16B4) = validBits + length - 16;
    CSF_AT(uint16_t, state, 0x16B0) =
        static_cast<uint16_t>(value) >> (16 - validBits);
}

// The optimized original receives state in EAX and its two tables on
// the stack. regparm(1) preserves that convention and leaves stack cleanup
// to the caller.
static void* __cdecl __attribute__((regparm(1)))
CoreZlibLog_CompressBlock(
    void* state, const void* literalTree, const void* distanceTree)
{
    if (CSF_AT(uint32_t, state, 0x1698))
    {
        uint32_t symbolIndex = 0;
        do
        {
            uint32_t distance = CSF_AT(
                uint16_t, CSF_AT(void*, state, 0x169C), 2 * symbolIndex);
            int lengthOrLiteral = CSF_AT(
                uint8_t, CSF_AT(void*, state, 0x1690), symbolIndex);

            if (!distance)
            {
                int bits = CSF_AT(
                    uint16_t, literalTree, 4 * lengthOrLiteral + 2);
                uint32_t code = CSF_AT(
                    uint16_t, literalTree, 4 * lengthOrLiteral);
                CoreZlibLog_SendBits(state, code, bits);
            }
            else
            {
                int lengthCode = g_LengthCodes[lengthOrLiteral];
                int bits = CSF_AT(
                    uint16_t, literalTree, 4 * lengthCode + 1030);
                uint32_t code = CSF_AT(
                    uint16_t, literalTree, 4 * lengthCode + 1028);
                CoreZlibLog_SendBits(state, code, bits);

                int extraBits = g_LengthExtraBits[lengthCode];
                if (extraBits)
                {
                    int extraValue =
                        lengthOrLiteral - g_LengthBases[lengthCode];
                    CoreZlibLog_SendBits(
                        state, static_cast<uint32_t>(extraValue), extraBits);
                }

                --distance;
                int distanceCode;
                if (distance >= 0x100)
                    distanceCode = g_LargeDistanceCodes[distance >> 7];
                else
                    distanceCode = g_SmallDistanceCodes[distance];

                bits = CSF_AT(
                    uint16_t, distanceTree, 4 * distanceCode + 2);
                code = CSF_AT(
                    uint16_t, distanceTree, 4 * distanceCode);
                CoreZlibLog_SendBits(state, code, bits);

                extraBits = g_DistanceExtraBits[distanceCode];
                if (extraBits)
                {
                    uint32_t extraValue =
                        distance - g_DistanceBases[distanceCode];
                    CoreZlibLog_SendBits(state, extraValue, extraBits);
                }
            }

            ++symbolIndex;
        }
        while (symbolIndex < CSF_AT(uint32_t, state, 0x1698));
    }

    int endBits = CSF_AT(uint16_t, literalTree, 1026);
    uint32_t endCode = CSF_AT(uint16_t, literalTree, 1024);
    CoreZlibLog_SendBits(state, endCode, endBits);
    CSF_AT(int32_t, state, 0x16AC) =
        CSF_AT(uint16_t, literalTree, 1026);

    return state;
}

HOOK(0x006A3F30, CoreZlibLog_CompressBlock, "auto");
