#include <cstdint>
#include "globals.h"
#include <cstring>
#include "hook.h"
#include "core/memory.h"

using ChecksumCallback = int (__cdecl *)(std::uint32_t, char*, std::uint32_t);

static int __cdecl CoreZlibLog_FlushInflateWindow(
    std::uint32_t* state, std::uint32_t* stream, int status)
{
    char* readCursor = CSF_AT(char*, state, 0x30);
    char* writeLimit = CSF_AT(char*, state, 0x34);
    char* output = CSF_AT(char*, stream, 0x0C);

    if (reinterpret_cast<std::uintptr_t>(readCursor) >
        reinterpret_cast<std::uintptr_t>(writeLimit))
    {
        writeLimit = CSF_AT(char*, state, 0x2C);
    }

    std::uint32_t available = CSF_AT(std::uint32_t, stream, 0x10);
    std::uint32_t count = static_cast<std::uint32_t>(
        reinterpret_cast<std::uintptr_t>(writeLimit) -
        reinterpret_cast<std::uintptr_t>(readCursor));

    if (count > available)
        count = available;

    if (count && status == -5)
        status = 0;

    CSF_AT(std::uint32_t, stream, 0x10) = available - count;
    CSF_AT(std::uint32_t, stream, 0x14) += count;

    ChecksumCallback checksum = CSF_AT(ChecksumCallback, state, 0x38);
    if (checksum)
    {
        int result = checksum(
            CSF_AT(std::uint32_t, state, 0x3C), readCursor, count);
        CSF_AT(std::uint32_t, state, 0x3C) = result;
        CSF_AT(std::uint32_t, stream, 0x30) = result;
    }

    std::memcpy(output, readCursor, count);
    output += count;
    readCursor += count;

    char* windowEnd = CSF_AT(char*, state, 0x2C);
    if (readCursor == windowEnd)
    {
        bool writeAtEnd = CSF_AT(char*, state, 0x34) == windowEnd;
        readCursor = CSF_AT(char*, state, 0x28);
        if (writeAtEnd)
            CSF_AT(char*, state, 0x34) = readCursor;

        writeLimit = CSF_AT(char*, state, 0x34);
        available = CSF_AT(std::uint32_t, stream, 0x10);
        count = static_cast<std::uint32_t>(
            reinterpret_cast<std::uintptr_t>(writeLimit) -
            reinterpret_cast<std::uintptr_t>(readCursor));

        if (count > available)
            count = available;

        if (count && status == -5)
            status = 0;

        CSF_AT(std::uint32_t, stream, 0x10) = available - count;
        CSF_AT(std::uint32_t, stream, 0x14) += count;

        checksum = CSF_AT(ChecksumCallback, state, 0x38);
        if (checksum)
        {
            int result = checksum(
                CSF_AT(std::uint32_t, state, 0x3C), readCursor, count);
            CSF_AT(std::uint32_t, state, 0x3C) = result;
            CSF_AT(std::uint32_t, stream, 0x30) = result;
        }

        std::memcpy(output, readCursor, count);
        output += count;
        readCursor += count;
    }

    CSF_AT(char*, stream, 0x0C) = output;
    CSF_AT(char*, state, 0x30) = readCursor;
    return status;
}

HOOK(0x006A6810, CoreZlibLog_FlushInflateWindow, "auto");
