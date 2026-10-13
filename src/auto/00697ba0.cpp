#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Bound callee: ScanStringRange (0x4F4770), __thiscall, 3 stack args (ret 12).
CSF_FUNC(uint32_t*, __fastcall, ScanStringRange, 0x004F4770, (void* self, void* edx, uint32_t* a2, int a3, unsigned int a4));

// Resets the string object's inline/heap state (capacity 15, size 0, first byte 0),
// then scans the current character range through ScanStringRange.
static uint32_t* __fastcall C_0082d410_ResetStringAndScanRange(C_0082d410* self, void* /*edx*/)
{
    CSF_AT(uint32_t, self, 0x18) = 15;
    CSF_AT(uint32_t, self, 0x14) = 0;
    *reinterpret_cast<uint8_t*>(&self->field_0004) = 0;

    // Start of character data: inline buffer when capacity < 16, otherwise the heap pointer.
    uint32_t capacity = CSF_AT(uint32_t, self, 0x18);
    uintptr_t dataStart;
    if (capacity < 16)
        dataStart = reinterpret_cast<uintptr_t>(&self->field_0004);
    else
        dataStart = static_cast<uintptr_t>(self->field_0004);

    uint32_t length = CSF_AT(uint32_t, self, 0x14);
    unsigned int rangeEnd = static_cast<unsigned int>(dataStart + length);

    uint32_t scanResult = 0;
    ScanStringRange(self, nullptr, &scanResult, static_cast<int>(dataStart), rangeEnd);

    CSF_AT(uint32_t, self, 0x1C) = 0;
    return reinterpret_cast<uint32_t*>(self);
}

HOOK(0x697BA0, C_0082d410_ResetStringAndScanRange, "auto");
