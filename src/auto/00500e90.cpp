#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Scans the string range [data, data + length) and returns a pointer result.
CSF_FUNC(uint32_t*, __fastcall, ScanStringRange, 0x004F4770, (void* self, void* edx, uint32_t* a2, int a3, unsigned int a4));

// Both embedded string buffers are scanned in order: the first at this+0x04 (length at +0x14, capacity at +0x18),
// the second at this+0x20 (length at +0x30, capacity at +0x34), scanned through the sub-object at this+0x1C.
// Resets the trailing state flags at +0x38 / +0x3C. Returns the result of the second scan.
static uint32_t* __fastcall C_0082d410_ScanBothStringRanges(C_0082d410* self, void* /*edx*/)
{
    uint32_t scanScratch = 0;

    // First range: inline buffer at +0x04 when capacity < 16, otherwise the heap pointer stored at +0x04.
    {
        uint32_t capacity = CSF_AT(uint32_t, self, 0x18);
        uintptr_t dataAddr = (capacity < 0x10)
            ? reinterpret_cast<uintptr_t>(&self->field_0004)
            : static_cast<uintptr_t>(self->field_0004);
        uint32_t length = CSF_AT(uint32_t, self, 0x14);
        ScanStringRange(self, nullptr, &scanScratch,
                        static_cast<int>(dataAddr),
                        static_cast<unsigned int>(dataAddr + length));
    }

    // Second range: inline buffer at +0x20 when capacity < 16, otherwise the heap pointer stored at +0x20.
    uint8_t* sub = reinterpret_cast<uint8_t*>(self) + 0x1C;
    uint32_t capacity2 = CSF_AT(uint32_t, self, 0x34);
    uintptr_t dataAddr2 = (capacity2 < 0x10)
        ? reinterpret_cast<uintptr_t>(reinterpret_cast<uint8_t*>(self) + 0x20)
        : static_cast<uintptr_t>(CSF_AT(uint32_t, self, 0x20));
    uint32_t length2 = CSF_AT(uint32_t, self, 0x30);

    uint32_t* result = ScanStringRange(sub, nullptr, &scanScratch,
                                       static_cast<int>(dataAddr2),
                                       static_cast<unsigned int>(dataAddr2 + length2));

    CSF_AT(uint32_t, self, 0x38) = 1;
    CSF_AT(uint32_t, self, 0x3C) = 0;
    return result;
}

HOOK(0x00500E90, C_0082d410_ScanBothStringRanges, "auto");
