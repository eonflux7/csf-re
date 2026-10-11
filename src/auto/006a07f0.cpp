#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

using ZlibFreeCallback = void (__cdecl*)(void* opaque, void* allocation);

static std::uint32_t __cdecl CoreZlibLog_EndDeflate(void* stream)
{
    if (!stream)
        return static_cast<std::uint32_t>(-2);

    void* state = CSF_AT(void*, stream, 0x1C);
    if (!state)
        return static_cast<std::uint32_t>(-2);

    const int status = CSF_AT(int, state, 0x04);
    if (status != 42 && status != 113 && status != 666)
        return static_cast<std::uint32_t>(-2);

    void* allocation = CSF_AT(void*, state, 0x08);
    if (allocation)
        CSF_AT(ZlibFreeCallback, stream, 0x24)(
            CSF_AT(void*, stream, 0x28), allocation);

    state = CSF_AT(void*, stream, 0x1C);
    allocation = CSF_AT(void*, state, 0x3C);
    if (allocation)
        CSF_AT(ZlibFreeCallback, stream, 0x24)(
            CSF_AT(void*, stream, 0x28), allocation);

    state = CSF_AT(void*, stream, 0x1C);
    allocation = CSF_AT(void*, state, 0x38);
    if (allocation)
        CSF_AT(ZlibFreeCallback, stream, 0x24)(
            CSF_AT(void*, stream, 0x28), allocation);

    state = CSF_AT(void*, stream, 0x1C);
    allocation = CSF_AT(void*, state, 0x30);
    if (allocation)
        CSF_AT(ZlibFreeCallback, stream, 0x24)(
            CSF_AT(void*, stream, 0x28), allocation);

    state = CSF_AT(void*, stream, 0x1C);
    CSF_AT(ZlibFreeCallback, stream, 0x24)(
        CSF_AT(void*, stream, 0x28), state);

    CSF_AT(void*, stream, 0x1C) = nullptr;
    return status != 113 ? 0 : 0xFFFFFFFD;
}

HOOK(0x006A07F0, CoreZlibLog_EndDeflate, "auto");
