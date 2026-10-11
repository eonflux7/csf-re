#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(int, __cdecl, sub_6A6810, 0x006A6810, (uint32_t* a1, uint32_t* a2, int a3));
CSF_FUNC(int, __cdecl, sub_6A6950, 0x006A6950, (int a1, int a2, int a3, int a4, uint32_t* a5, void** a6));

using InflateBitMasks = uint32_t[17];
CSF_GLOBAL(InflateBitMasks, g_InflateBitMasks, 0x008A7D28);

static int CoreZlibLog_CommitAndFlushCodes(
    uint32_t* blocks, unsigned char** stream, unsigned char* input,
    uint32_t remaining, uint32_t bits, uint32_t buffer,
    unsigned char* output, int status, bool countFirst = false)
{
    if (countFirst) {
        CSF_AT(uint32_t, blocks, 0x1C) = bits;
        CSF_AT(uint32_t, blocks, 0x20) = buffer;
    } else {
        CSF_AT(uint32_t, blocks, 0x20) = buffer;
        CSF_AT(uint32_t, blocks, 0x1C) = bits;
    }
    CSF_AT(uint32_t, stream, 0x04) = remaining;
    CSF_AT(uint32_t, stream, 0x08) +=
        reinterpret_cast<uintptr_t>(input) - CSF_AT(uint32_t, stream, 0x00);
    CSF_AT(unsigned char*, stream, 0x00) = input;
    CSF_AT(unsigned char*, blocks, 0x34) = output;
    return sub_6A6810(blocks, reinterpret_cast<uint32_t*>(stream), status);
}

static bool CoreZlibLog_EnsureCodeOutputSpace(
    uint32_t* blocks, unsigned char** stream,
    unsigned char*& output, uint32_t& space, int& status)
{
    if (space != 0)
        return true;

    if (output == CSF_AT(unsigned char*, blocks, 0x2C)) {
        uint32_t read = CSF_AT(uint32_t, blocks, 0x30);
        uint32_t start = CSF_AT(uint32_t, blocks, 0x28);
        if (read != start) {
            output = CSF_AT(unsigned char*, blocks, 0x28);
            space = start >= read
                ? CSF_AT(uint32_t, blocks, 0x2C) - start
                : read - start - 1;
        }
    }
    if (space != 0)
        return true;

    CSF_AT(unsigned char*, blocks, 0x34) = output;
    status = sub_6A6810(blocks, reinterpret_cast<uint32_t*>(stream), status);
    output = CSF_AT(unsigned char*, blocks, 0x34);
    uint32_t read = CSF_AT(uint32_t, blocks, 0x30);
    uint32_t write = reinterpret_cast<uintptr_t>(output);
    space = write >= read
        ? CSF_AT(uint32_t, blocks, 0x2C) - write
        : read - write - 1;

    unsigned char* end = CSF_AT(unsigned char*, blocks, 0x2C);
    if (output == end) {
        uint32_t start = CSF_AT(uint32_t, blocks, 0x28);
        if (read != start) {
            output = CSF_AT(unsigned char*, blocks, 0x28);
            space = start >= read
                ? reinterpret_cast<uintptr_t>(end) - start
                : read - start - 1;
        }
    }
    return space != 0;
}

static int __cdecl CoreZlibLog_InflateCodes(
    uint32_t* blocks, unsigned char** stream, int status)
{
    uint32_t* codes = CSF_AT(uint32_t*, blocks, 0x04);
    uint32_t bits = CSF_AT(uint32_t, blocks, 0x1C);
    uint32_t buffer = CSF_AT(uint32_t, blocks, 0x20);
    unsigned char* input = CSF_AT(unsigned char*, stream, 0x00);
    uint32_t remaining = CSF_AT(uint32_t, stream, 0x04);
    unsigned char* output = CSF_AT(unsigned char*, blocks, 0x34);
    uint32_t read = CSF_AT(uint32_t, blocks, 0x30);
    uint32_t write = reinterpret_cast<uintptr_t>(output);
    uint32_t space = write >= read
        ? CSF_AT(uint32_t, blocks, 0x2C) - write
        : read - write - 1;

    for (;;) {
        switch (CSF_AT(uint32_t, codes, 0x00)) {
        case 0: {
            if (space >= 0x102 && remaining >= 0xA) {
                CSF_AT(uint32_t, blocks, 0x20) = buffer;
                CSF_AT(uint32_t, blocks, 0x1C) = bits;
                CSF_AT(uint32_t, stream, 0x04) = remaining;
                CSF_AT(uint32_t, stream, 0x08) +=
                    reinterpret_cast<uintptr_t>(input) -
                    CSF_AT(uint32_t, stream, 0x00);
                CSF_AT(unsigned char*, stream, 0x00) = input;
                CSF_AT(unsigned char*, blocks, 0x34) = output;

                status = sub_6A6950(
                    CSF_AT(uint8_t, codes, 0x10),
                    CSF_AT(uint8_t, codes, 0x11),
                    CSF_AT(int, codes, 0x14),
                    CSF_AT(int, codes, 0x18),
                    blocks, reinterpret_cast<void**>(stream));

                bits = CSF_AT(uint32_t, blocks, 0x1C);
                input = CSF_AT(unsigned char*, stream, 0x00);
                remaining = CSF_AT(uint32_t, stream, 0x04);
                output = CSF_AT(unsigned char*, blocks, 0x34);
                buffer = CSF_AT(uint32_t, blocks, 0x20);
                read = CSF_AT(uint32_t, blocks, 0x30);
                write = reinterpret_cast<uintptr_t>(output);
                space = write >= read
                    ? CSF_AT(uint32_t, blocks, 0x2C) - write
                    : read - write - 1;
                if (status != 0) {
                    CSF_AT(uint32_t, codes, 0x00) = 2 * (status != 1) + 7;
                    continue;
                }
            }
            CSF_AT(uint32_t, codes, 0x0C) = CSF_AT(uint8_t, codes, 0x10);
            CSF_AT(uint32_t, codes, 0x08) = CSF_AT(uint32_t, codes, 0x14);
            CSF_AT(uint32_t, codes, 0x00) = 1;
            [[fallthrough]];
        }
        case 1: {
            uint32_t need = CSF_AT(uint32_t, codes, 0x0C);
            while (bits < need) {
                if (remaining == 0)
                    return CoreZlibLog_CommitAndFlushCodes(
                        blocks, stream, input, 0, bits, buffer, output, status);
                --remaining;
                buffer |= uint32_t(*input++) << bits;
                bits += 8;
                status = 0;
                need = CSF_AT(uint32_t, codes, 0x0C);
            }
            unsigned char* entry = reinterpret_cast<unsigned char*>(
                CSF_AT(uint32_t, codes, 0x08) +
                8 * (buffer & g_InflateBitMasks[need]));
            buffer >>= entry[1];
            bits -= entry[1];
            uint32_t operation = entry[0];
            if (operation == 0) {
                CSF_AT(uint32_t, codes, 0x08) = CSF_AT(uint32_t, entry, 0x04);
                CSF_AT(uint32_t, codes, 0x00) = 6;
            } else if ((operation & 0x10) != 0) {
                CSF_AT(uint32_t, codes, 0x08) = operation & 0xF;
                CSF_AT(uint32_t, codes, 0x04) = CSF_AT(uint32_t, entry, 0x04);
                CSF_AT(uint32_t, codes, 0x00) = 2;
            } else if ((operation & 0x40) == 0) {
                CSF_AT(uint32_t, codes, 0x0C) = operation;
                CSF_AT(unsigned char*, codes, 0x08) =
                    entry + 8 * CSF_AT(uint32_t, entry, 0x04);
            } else if ((operation & 0x20) != 0) {
                CSF_AT(uint32_t, codes, 0x00) = 7;
            } else {
                CSF_AT(uint32_t, codes, 0x00) = 9;
                CSF_AT(const char*, stream, 0x18) =
                    CSF_STR(0x0085F9D8, "invalid literal/length code");
                return CoreZlibLog_CommitAndFlushCodes(
                    blocks, stream, input, remaining, bits, buffer, output, -3);
            }
            break;
        }
        case 2: {
            uint32_t extra = CSF_AT(uint32_t, codes, 0x08);
            uint32_t maskBits = extra;
            while (bits < extra) {
                if (remaining == 0)
                    return CoreZlibLog_CommitAndFlushCodes(
                        blocks, stream, input, 0, bits, buffer, output, status);
                --remaining;
                buffer |= uint32_t(*input++) << bits;
                bits += 8;
                status = 0;
                maskBits = CSF_AT(uint32_t, codes, 0x08);
            }
            CSF_AT(uint32_t, codes, 0x04) += buffer & g_InflateBitMasks[maskBits];
            buffer >>= extra;
            bits -= extra;
            CSF_AT(uint32_t, codes, 0x0C) = CSF_AT(uint8_t, codes, 0x11);
            CSF_AT(uint32_t, codes, 0x08) = CSF_AT(uint32_t, codes, 0x18);
            CSF_AT(uint32_t, codes, 0x00) = 3;
            [[fallthrough]];
        }
        case 3: {
            uint32_t need = CSF_AT(uint32_t, codes, 0x0C);
            while (bits < need) {
                if (remaining == 0)
                    return CoreZlibLog_CommitAndFlushCodes(
                        blocks, stream, input, 0, bits, buffer, output, status);
                --remaining;
                buffer |= uint32_t(*input++) << bits;
                bits += 8;
                status = 0;
            }
            unsigned char* entry = reinterpret_cast<unsigned char*>(
                CSF_AT(uint32_t, codes, 0x08) +
                8 * (buffer & g_InflateBitMasks[need]));
            buffer >>= entry[1];
            uint32_t operation = entry[0];
            bits -= entry[1];
            if ((operation & 0x10) != 0) {
                CSF_AT(uint32_t, codes, 0x08) = operation & 0xF;
                CSF_AT(uint32_t, codes, 0x0C) = CSF_AT(uint32_t, entry, 0x04);
                CSF_AT(uint32_t, codes, 0x00) = 4;
            } else if ((operation & 0x40) == 0) {
                CSF_AT(uint32_t, codes, 0x0C) = operation;
                CSF_AT(unsigned char*, codes, 0x08) =
                    entry + 8 * CSF_AT(uint32_t, entry, 0x04);
            } else {
                CSF_AT(uint32_t, codes, 0x00) = 9;
                CSF_AT(const char*, stream, 0x18) =
                    CSF_STR(0x0085F9C0, "invalid distance code");
                return CoreZlibLog_CommitAndFlushCodes(
                    blocks, stream, input, remaining, bits, buffer, output, -3);
            }
            break;
        }
        case 4: {
            uint32_t extra = CSF_AT(uint32_t, codes, 0x08);
            uint32_t maskBits = extra;
            while (bits < extra) {
                if (remaining == 0)
                    return CoreZlibLog_CommitAndFlushCodes(
                        blocks, stream, input, 0, bits, buffer, output, status);
                --remaining;
                buffer |= uint32_t(*input++) << bits;
                bits += 8;
                status = 0;
                maskBits = CSF_AT(uint32_t, codes, 0x08);
            }
            CSF_AT(uint32_t, codes, 0x0C) += buffer & g_InflateBitMasks[maskBits];
            buffer >>= extra;
            bits -= extra;
            CSF_AT(uint32_t, codes, 0x00) = 5;
            [[fallthrough]];
        }
        case 5: {
            uint32_t sourceAddress = reinterpret_cast<uintptr_t>(output) -
                CSF_AT(uint32_t, codes, 0x0C);
            uint32_t start = CSF_AT(uint32_t, blocks, 0x28);
            if (sourceAddress < start) {
                uint32_t windowSize = CSF_AT(uint32_t, blocks, 0x2C) - start;
                do {
                    sourceAddress += windowSize;
                } while (sourceAddress < CSF_AT(uint32_t, blocks, 0x28));
            }
            unsigned char* source = reinterpret_cast<unsigned char*>(sourceAddress);
            if (CSF_AT(uint32_t, codes, 0x04) != 0) {
                for (;;) {
                    if (!CoreZlibLog_EnsureCodeOutputSpace(
                            blocks, stream, output, space, status))
                        return CoreZlibLog_CommitAndFlushCodes(
                            blocks, stream, input, remaining, bits, buffer,
                            output, status);

                    *output++ = *source;
                    --space;
                    bool wrapSource =
                        source + 1 == CSF_AT(unsigned char*, blocks, 0x2C);
                    status = 0;
                    ++source;
                    if (wrapSource)
                        source = CSF_AT(unsigned char*, blocks, 0x28);
                    if (--CSF_AT(uint32_t, codes, 0x04) == 0)
                        break;
                }
            }
            CSF_AT(uint32_t, codes, 0x00) = 0;
            break;
        }
        case 6:
            if (!CoreZlibLog_EnsureCodeOutputSpace(
                    blocks, stream, output, space, status))
                return CoreZlibLog_CommitAndFlushCodes(
                    blocks, stream, input, remaining, bits, buffer, output, status);
            *output++ = CSF_AT(uint8_t, codes, 0x08);
            --space;
            status = 0;
            CSF_AT(uint32_t, codes, 0x00) = 0;
            break;

        case 7: {
            if (bits > 7) {
                bits -= 8;
                ++remaining;
                --input;
            }
            CSF_AT(unsigned char*, blocks, 0x34) = output;
            int flushStatus = sub_6A6810(
                blocks, reinterpret_cast<uint32_t*>(stream), status);
            output = CSF_AT(unsigned char*, blocks, 0x34);
            if (CSF_AT(unsigned char*, blocks, 0x30) != output)
                return CoreZlibLog_CommitAndFlushCodes(
                    blocks, stream, input, remaining, bits, buffer,
                    output, flushStatus);
            CSF_AT(uint32_t, codes, 0x00) = 8;
            [[fallthrough]];
        }
        case 8:
            return CoreZlibLog_CommitAndFlushCodes(
                blocks, stream, input, remaining, bits, buffer, output, 1);

        case 9:
            return CoreZlibLog_CommitAndFlushCodes(
                blocks, stream, input, remaining, bits, buffer, output, -3, true);

        default:
            return CoreZlibLog_CommitAndFlushCodes(
                blocks, stream, input, remaining, bits, buffer, output, -2, true);
        }
    }
}

HOOK(0x006A5A50, CoreZlibLog_InflateCodes, "auto");
