#include <cstdint>
#include "globals.h"
#include <cstring>
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(int, __cdecl, sub_6A18C0, 0x006A18C0, (uint32_t* a1));
CSF_FUNC(int, __cdecl, sub_6A1A10, 0x006A1A10, (uint32_t* a1, uint8_t* a2, int a3));
CSF_FUNC(int, __cdecl, sub_6A1A30, 0x006A1A30, (void** a1, int a2));

static char __fastcall CoreZlibLog_InflateAndReset(
    void** self, void* /*edx*/, int outputBuffer,
    uint32_t* outputSize, int inputBuffer, int inputSize)
{
    if (static_cast<uint32_t>(inputSize) == 0) {
        *outputSize = 0;
        return 1;
    }

    CSF_AT(uint32_t, *self, 0x10) = *outputSize;
    CSF_AT(uint32_t, *self, 0x0C) = static_cast<uint32_t>(outputBuffer);
    CSF_AT(uint32_t, *self, 0x00) = static_cast<uint32_t>(inputBuffer);
    CSF_AT(uint32_t, *self, 0x04) = static_cast<uint32_t>(inputSize);

    if (sub_6A1A30(static_cast<void**>(*self), 2) != 1)
        return 0;

    *outputSize -= CSF_AT(uint32_t, *self, 0x10);
    sub_6A18C0(static_cast<uint32_t*>(*self));
    std::memset(*self, 0, 0x38);

    CSF_AT(uintptr_t, *self, 0x20) =
        reinterpret_cast<uintptr_t>(CSF_ADDR_FN(0x006A2F90));
    CSF_AT(uintptr_t, *self, 0x24) =
        reinterpret_cast<uintptr_t>(CSF_ADDR_FN(0x006A2EB0));
    CSF_AT(uint32_t, *self, 0x2C) = 0;

    sub_6A1A10(
        static_cast<uint32_t*>(*self),
        reinterpret_cast<uint8_t*>(
            const_cast<char*>(CSF_STR(0x0085E758, "1.1.4"))),
        56);
    return 1;
}

HOOK(0x006A3150, CoreZlibLog_InflateAndReset, "auto");
