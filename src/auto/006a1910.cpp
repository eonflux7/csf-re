#include <cstdint>
#include "globals.h"
#include "hook.h"

CSF_FUNC(int, __cdecl, CoreZlibLog_ResetInflate, 0x006A1870, (uint32_t* a1));
CSF_FUNC(int, __cdecl, CoreZlibLog_EndInflate, 0x006A18C0, (uint32_t* a1));
CSF_FUNC(uint32_t*, __cdecl, CoreZlibLog_CreateInflateBlocks, 0x006A4E70,
         (int a1, int a2, int a3));

static int __cdecl CoreZlibLog_InitializeInflate(
    uint32_t* stream, int windowBits, uint8_t* version, int streamSize)
{
    if (!version || *version != 49 || streamSize != 56)
        return -6;
    if (!stream)
        return -2;

    const bool needsAllocator = CSF_AT(uint32_t, stream, 0x20) == 0;
    CSF_AT(uint32_t, stream, 0x18) = 0;

    if (needsAllocator) {
        CSF_AT(uint32_t, stream, 0x20) =
            static_cast<uint32_t>(CSF_ADDR_FN(0x006A4DB0));
        CSF_AT(uint32_t, stream, 0x28) = 0;
    }
    if (!CSF_AT(uint32_t, stream, 0x24)) {
        CSF_AT(uint32_t, stream, 0x24) =
            static_cast<uint32_t>(CSF_ADDR_FN(0x006A4DD0));
    }

    using Allocator = uint32_t* (__cdecl*)(uint32_t, int, int);
    uint32_t* state =
        reinterpret_cast<Allocator>(CSF_AT(uint32_t, stream, 0x20))(
            CSF_AT(uint32_t, stream, 0x28), 1, 24);
    CSF_AT(uint32_t*, stream, 0x1C) = state;
    if (!state)
        return -4;

    CSF_AT(uint32_t, state, 0x14) = 0;
    CSF_AT(uint32_t, CSF_AT(uint32_t*, stream, 0x1C), 0x0C) = 0;

    if (windowBits < 0) {
        windowBits = static_cast<int32_t>(
            0u - static_cast<uint32_t>(windowBits));
        CSF_AT(uint32_t, CSF_AT(uint32_t*, stream, 0x1C), 0x0C) = 1;
    }

    if (windowBits < 8 || windowBits > 15) {
        CoreZlibLog_EndInflate(stream);
        return -2;
    }

    CSF_AT(int, CSF_AT(uint32_t*, stream, 0x1C), 0x10) = windowBits;

    const int checksumCallback =
        CSF_AT(uint32_t, CSF_AT(uint32_t*, stream, 0x1C), 0x0C) != 0
            ? 0
            : static_cast<int>(CSF_ADDR_FN(0x006A3200));

    uint32_t* blocks = CoreZlibLog_CreateInflateBlocks(
        reinterpret_cast<int>(stream), checksumCallback, 1 << windowBits);
    CSF_AT(uint32_t*, CSF_AT(uint32_t*, stream, 0x1C), 0x14) = blocks;

    if (!CSF_AT(uint32_t*, CSF_AT(uint32_t*, stream, 0x1C), 0x14)) {
        CoreZlibLog_EndInflate(stream);
        return -4;
    }

    CoreZlibLog_ResetInflate(stream);
    return 0;
}

HOOK(0x006A1910, CoreZlibLog_InitializeInflate, "auto");
