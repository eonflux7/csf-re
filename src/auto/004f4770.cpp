#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, StringEraseRange, 0x00403880, (void* self, void* edx, unsigned int pos, unsigned int count));

// Erases the range [a3, a4) from the string object, then writes the address of the
// position where the range started to *a2 and returns a2.
static uint32_t* __fastcall C_0082d410_EraseRangeAndGetStart(C_0082d410* self, void* /*edx*/, uint32_t* a2, int a3, unsigned int a4)
{
    // Small-string optimisation: when the capacity (+0x18) is below 16 the characters
    // live inline at &field_0004; otherwise field_0004 holds a heap pointer.
    uint32_t bufferBase;
    if (CSF_AT(uint32_t, self, 0x18) < 0x10u)
        bufferBase = static_cast<uint32_t>(reinterpret_cast<uintptr_t>(&self->field_0004));
    else
        bufferBase = self->field_0004;

    uint32_t eraseStart = 0;
    if (a3 != 0)
        eraseStart = static_cast<uint32_t>(a3) - bufferBase;

    uint32_t eraseCount = a4;
    if (a4 != 0)
        eraseCount = a4 - static_cast<uint32_t>(a3);

    StringEraseRange(self, nullptr, eraseStart, eraseCount);

    if (CSF_AT(uint32_t, self, 0x18) < 0x10u)
        *a2 = static_cast<uint32_t>(reinterpret_cast<uintptr_t>(&self->field_0004) + eraseStart);
    else
        *a2 = self->field_0004 + eraseStart;

    return a2;
}

HOOK(0x004F4770, C_0082d410_EraseRangeAndGetStart, "auto");
