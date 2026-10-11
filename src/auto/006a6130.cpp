#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

// The original receives rootBits in EAX and all other arguments on the
// caller-cleaned stack. Clang's x86 regparm(1) expresses that convention.
static int __cdecl __attribute__((regparm(1)))
CoreZlibLog_BuildInflateHuffmanTable(
    uint32_t* rootBits,
    const uint32_t* lengths,
    uint32_t symbolCount,
    uint32_t simpleSymbolCount,
    const uint32_t* bases,
    const uint32_t* extraBits,
    uint32_t* tableAddress,
    void* tablePool,
    uint32_t* entriesUsed,
    uint32_t* sortedSymbols)
{
    uint32_t counts[16] = {};
    uint32_t offsets[16];
    uint8_t* tables[15];

    const uint32_t* lengthCursor = lengths;
    uint32_t remainingSymbols = symbolCount;
    do {
        ++counts[*lengthCursor++];
    } while (--remainingSymbols);

    if (counts[0] == symbolCount) {
        *tableAddress = 0;
        *rootBits = 0;
        return 0;
    }

    int minimumLength = 1;
    while (!counts[minimumLength])
        ++minimumLength;

    int maximumLength = 15;
    while (!counts[maximumLength])
        --maximumLength;

    uint32_t tableBits = *rootBits;
    if (tableBits < static_cast<uint32_t>(minimumLength))
        tableBits = minimumLength;
    if (tableBits > static_cast<uint32_t>(maximumLength))
        tableBits = maximumLength;
    *rootBits = tableBits;

    int unusedCodes = 1 << minimumLength;
    for (int length = minimumLength; length < maximumLength; ++length) {
        unusedCodes = static_cast<int>(
            static_cast<uint32_t>(unusedCodes) - counts[length]);
        if (unusedCodes < 0)
            return -3;
        unusedCodes *= 2;
    }

    unusedCodes = static_cast<int>(
        static_cast<uint32_t>(unusedCodes) - counts[maximumLength]);
    if (unusedCodes < 0)
        return -3;
    counts[maximumLength] += unusedCodes;

    offsets[1] = 0;
    uint32_t cumulativeCount = 0;
    for (int length = 1; length < maximumLength; ++length) {
        cumulativeCount += counts[length];
        offsets[length + 1] = cumulativeCount;
    }

    for (uint32_t symbol = 0; symbol < symbolCount; ++symbol) {
        const uint32_t length = lengths[symbol];
        if (length)
            sortedSymbols[offsets[length]++] = symbol;
    }

    const uint32_t sortedCount = offsets[maximumLength];
    uint32_t* symbolCursor = sortedSymbols;
    offsets[0] = 0;
    tables[0] = nullptr;

    uint32_t code = 0;
    int level = -1;
    int consumedBits = -static_cast<int>(tableBits);
    uint8_t* currentTable = nullptr;
    uint32_t currentTableSize = 0;
    uint32_t entryBase = 0;

    for (int length = minimumLength; length <= maximumLength; ++length) {
        uint32_t codesAtLength = counts[length];
        while (codesAtLength) {
            --codesAtLength;

            while (length > consumedBits + static_cast<int>(tableBits)) {
                ++level;
                consumedBits += tableBits;

                uint32_t maximumTableBits = maximumLength - consumedBits;
                if (maximumTableBits > tableBits)
                    maximumTableBits = tableBits;

                uint32_t nextTableBits = length - consumedBits;
                uint32_t availableCodes = 1u << nextTableBits;
                if (availableCodes > codesAtLength + 1) {
                    availableCodes -= codesAtLength + 1;
                    int countLength = length;
                    if (nextTableBits < maximumTableBits) {
                        while (++nextTableBits < maximumTableBits) {
                            availableCodes *= 2;
                            if (availableCodes <= counts[++countLength])
                                break;
                            availableCodes -= counts[countLength];
                        }
                    }
                }

                currentTableSize = 1u << nextTableBits;
                const uint32_t poolIndex = *entriesUsed;
                const uint32_t nextPoolIndex = poolIndex + currentTableSize;
                if (nextPoolIndex > 0x5A0)
                    return -3;

                currentTable =
                    static_cast<uint8_t*>(tablePool) + 8 * poolIndex;
                tables[level] = currentTable;
                *entriesUsed = nextPoolIndex;

                if (level) {
                    offsets[level] = code;
                    const uint32_t parentIndex =
                        code >> (consumedBits - tableBits);
                    uint8_t* parentTable = tables[level - 1];
                    entryBase = static_cast<uint32_t>(
                        (currentTable - parentTable) / 8) - parentIndex;

                    CSF_AT(uint32_t, parentTable + 8 * parentIndex, 0) =
                        nextTableBits | (tableBits << 8);
                    CSF_AT(uint32_t, parentTable + 8 * parentIndex, 4) =
                        entryBase;
                } else {
                    *tableAddress =
                        static_cast<uint32_t>(
                            reinterpret_cast<uintptr_t>(currentTable));
                }
            }

            uint32_t operation;
            if (symbolCursor < sortedSymbols + sortedCount) {
                const uint32_t symbol = *symbolCursor++;
                if (symbol >= simpleSymbolCount) {
                    operation = static_cast<uint8_t>(
                        extraBits[symbol - simpleSymbolCount] + 80);
                    entryBase = bases[symbol - simpleSymbolCount];
                } else {
                    operation = symbol < 0x100 ? 0 : 0x60;
                    entryBase = symbol;
                }
            } else {
                operation = 0xC0;
            }

            const uint32_t entryBits = length - consumedBits;
            const uint32_t entryHeader =
                operation | (static_cast<uint8_t>(entryBits) << 8);
            const uint32_t stride = 1u << entryBits;
            for (uint32_t index = code >> consumedBits;
                 index < currentTableSize;
                 index += stride) {
                CSF_AT(uint32_t, currentTable + 8 * index, 0) = entryHeader;
                CSF_AT(uint32_t, currentTable + 8 * index, 4) = entryBase;
            }

            uint32_t bit = 1u << (length - 1);
            while (code & bit) {
                code ^= bit;
                bit >>= 1;
            }
            code ^= bit;

            while ((code & ((1u << consumedBits) - 1)) != offsets[level]) {
                --level;
                consumedBits -= tableBits;
            }
        }
    }

    return unusedCodes && maximumLength != 1 ? -5 : 0;
}

HOOK(0x006A6130, CoreZlibLog_BuildInflateHuffmanTable, "auto");
