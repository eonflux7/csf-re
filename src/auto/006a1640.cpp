#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_GLOBAL(const char*, g_ZlibVersion, 0x008A6BA8);
CSF_GLOBAL(uint32_t, g_ZlibMemoryError, 0x008A6C10);

CSF_FUNC(unsigned int, __cdecl, CoreZlibLog_EndDeflate, 0x006A07F0, (int a1));
CSF_FUNC(int, __cdecl, CoreZlibLog_ResetDeflate, 0x006A15C0, (uint32_t* a1));

using ZlibAllocator = void* (__cdecl*)(uint32_t opaque, uint32_t count, uint32_t size);

static int __cdecl CoreZlibLog_InitializeDeflate(
    uint32_t* stream, int level, int method, int windowBits,
    int memoryLevel, unsigned int strategy, const uint8_t* version,
    int streamSize)
{
    if (!version || *version != static_cast<uint8_t>(*g_ZlibVersion) ||
        streamSize != 56)
        return -6;

    if (!stream)
        return -2;

    const bool needsAllocator = CSF_AT(uint32_t, stream, 0x20) == 0;
    CSF_AT(uint32_t, stream, 0x18) = 0;
    if (needsAllocator)
    {
        CSF_AT(uint32_t, stream, 0x20) =
            reinterpret_cast<uint32_t>(CSF_ADDR_FN(0x006A4DB0));
        CSF_AT(uint32_t, stream, 0x28) = 0;
    }
    if (!CSF_AT(uint32_t, stream, 0x24))
        CSF_AT(uint32_t, stream, 0x24) =
            reinterpret_cast<uint32_t>(CSF_ADDR_FN(0x006A4DD0));

    if (level == -1)
        level = 6;

    int rawDeflate = 0;
    if (windowBits < 0)
    {
        windowBits = static_cast<int>(0u - static_cast<uint32_t>(windowBits));
        rawDeflate = 1;
    }

    if (memoryLevel < 1 || memoryLevel > 6 || method != 8 ||
        windowBits < 9 || windowBits > 15 ||
        level < 0 || level > 9 || strategy > 2)
        return -2;

    void* state =
        reinterpret_cast<ZlibAllocator>(CSF_AT(uint32_t, stream, 0x20))(
            CSF_AT(uint32_t, stream, 0x28), 1, 5816);
    if (!state)
        return -4;

    CSF_AT(uint32_t, stream, 0x1C) = reinterpret_cast<uint32_t>(state);
    CSF_AT(int, state, 0x28) = windowBits;
    const uint32_t windowSize = 1u << windowBits;
    CSF_AT(int, state, 0x18) = rawDeflate;
    CSF_AT(uint32_t, state, 0x2C) = windowSize - 1;
    CSF_AT(int, state, 0x48) = memoryLevel + 7;
    CSF_AT(uint32_t*, state, 0x00) = stream;
    CSF_AT(uint32_t, state, 0x44) = 1u << (memoryLevel + 7);
    CSF_AT(uint32_t, state, 0x4C) = (1u << (memoryLevel + 7)) - 1;
    CSF_AT(uint32_t, state, 0x24) = windowSize;
    CSF_AT(uint32_t, state, 0x50) = (memoryLevel + 9) / 3u;

    void* window =
        reinterpret_cast<ZlibAllocator>(CSF_AT(uint32_t, stream, 0x20))(
            CSF_AT(uint32_t, stream, 0x28), windowSize, 2);
    CSF_AT(void*, state, 0x30) = window;

    void* previous =
        reinterpret_cast<ZlibAllocator>(CSF_AT(uint32_t, stream, 0x20))(
            CSF_AT(uint32_t, stream, 0x28),
            CSF_AT(uint32_t, state, 0x24), 2);
    const uint32_t hashSize = CSF_AT(uint32_t, state, 0x44);
    CSF_AT(void*, state, 0x38) = previous;

    void* hashHeads =
        reinterpret_cast<ZlibAllocator>(CSF_AT(uint32_t, stream, 0x20))(
            CSF_AT(uint32_t, stream, 0x28), hashSize, 2);
    CSF_AT(void*, state, 0x3C) = hashHeads;

    const uint32_t literalBufferSize = 1u << (memoryLevel + 6);
    CSF_AT(uint32_t, state, 0x1694) = literalBufferSize;
    void* pendingBuffer =
        reinterpret_cast<ZlibAllocator>(CSF_AT(uint32_t, stream, 0x20))(
            CSF_AT(uint32_t, stream, 0x28), literalBufferSize, 4);

    const uint32_t bufferSize = CSF_AT(uint32_t, state, 0x1694);
    CSF_AT(uint32_t, state, 0x0C) = 4 * bufferSize;
    void* allocatedWindow = CSF_AT(void*, state, 0x30);
    CSF_AT(void*, state, 0x08) = pendingBuffer;

    if (allocatedWindow && CSF_AT(void*, state, 0x38) &&
        CSF_AT(void*, state, 0x3C) && pendingBuffer)
    {
        const uint32_t bufferAddress = reinterpret_cast<uint32_t>(pendingBuffer);
        CSF_AT(uint32_t, state, 0x169C) =
            bufferAddress + 2 * (bufferSize >> 1);
        CSF_AT(uint32_t, state, 0x1690) =
            bufferAddress + 2 * bufferSize + bufferSize;
        CSF_AT(int, state, 0x7C) = level;
        CSF_AT(unsigned int, state, 0x80) = strategy;
        CSF_AT(uint8_t, state, 0x1D) = 8;
        return CoreZlibLog_ResetDeflate(stream);
    }

    CSF_AT(uint32_t, stream, 0x18) = g_ZlibMemoryError;
    CoreZlibLog_EndDeflate(reinterpret_cast<int>(stream));
    return -4;
}

HOOK(0x006A1640, CoreZlibLog_InitializeDeflate, "auto");
