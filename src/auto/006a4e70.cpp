#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

using InflateAllocator = void* (__cdecl*)(void*, std::uint32_t, std::uint32_t);
using InflateDeallocator = void (__cdecl*)(void*, void*);
using InflateChecksumFunction =
    int (__cdecl*)(std::uint32_t, std::uint32_t, std::uint32_t);

CSF_FUNC(InflateChecksumFunction, __cdecl, CoreZlibLog_ResetInflateBlocks,
         0x006A4DF0,
         (std::uint32_t* blocks, void* stream, std::uint32_t* checksum));

static std::uint32_t* __cdecl CoreZlibLog_CreateInflateBlocks(
    void* stream, std::uint32_t checksumFunction, std::uint32_t windowSize)
{
    auto* blocks = static_cast<std::uint32_t*>(
        CSF_AT(InflateAllocator, stream, 0x20)(
            CSF_AT(void*, stream, 0x28), 1, 64));
    if (!blocks)
        return nullptr;

    void* decodeWorkspace = CSF_AT(InflateAllocator, stream, 0x20)(
        CSF_AT(void*, stream, 0x28), 8, 1440);
    CSF_AT(void*, blocks, 0x24) = decodeWorkspace;
    if (!decodeWorkspace)
    {
        CSF_AT(InflateDeallocator, stream, 0x24)(
            CSF_AT(void*, stream, 0x28), blocks);
        return nullptr;
    }

    auto* window = static_cast<std::uint8_t*>(
        CSF_AT(InflateAllocator, stream, 0x20)(
            CSF_AT(void*, stream, 0x28), 1, windowSize));
    CSF_AT(std::uint8_t*, blocks, 0x28) = window;
    if (!window)
    {
        CSF_AT(InflateDeallocator, stream, 0x24)(
            CSF_AT(void*, stream, 0x28),
            CSF_AT(void*, blocks, 0x24));
        CSF_AT(InflateDeallocator, stream, 0x24)(
            CSF_AT(void*, stream, 0x28), blocks);
        return nullptr;
    }

    CSF_AT(std::uint8_t*, blocks, 0x2C) = window + windowSize;
    CSF_AT(std::uint32_t, blocks, 0x38) = checksumFunction;
    CSF_AT(std::uint32_t, blocks, 0x00) = 0;
    CoreZlibLog_ResetInflateBlocks(blocks, stream, nullptr);
    return blocks;
}

HOOK(0x006A4E70, CoreZlibLog_CreateInflateBlocks, "auto");
