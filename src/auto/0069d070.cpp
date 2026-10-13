#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_FUNC(void*, __fastcall, AssignStringRange_429B60, 0x00429B60, (void* self, void* edx, char* text, unsigned int length));

// Inline NUL-terminated length loop from the original (no CRT strlen call in the binary).
static unsigned int MeasureNulTerminatedLength(const char* text) {
    const char* p = text;
    while (*p != 0) {
        ++p;
    }
    return static_cast<unsigned int>(p - text);
}

static C_0082d410* __fastcall Registry_Initialize(C_0082d410* self, void* /*edx*/, const char* a2, const char* a3) {
    uint8_t* base = reinterpret_cast<uint8_t*>(self);

    CSF_AT(uint32_t, base, 0x1C) = 0;

    // First embedded string object at +0x08
    CSF_AT(uint32_t, base, 0x20) = 15;   // capacity
    CSF_AT(uint8_t, base, 0x0C) = 0;     // terminator byte

    // Second embedded string object at +0x24
    CSF_AT(uint32_t, base, 0x3C) = 15;   // capacity
    CSF_AT(uint32_t, base, 0x38) = 0;    // size
    CSF_AT(uint8_t, base, 0x28) = 0;     // terminator byte

    CSF_AT(uint8_t, base, 0x00) = 0;
    CSF_AT(uint32_t, base, 0x04) = 0;

    unsigned int firstLength = MeasureNulTerminatedLength(a2);
    AssignStringRange_429B60(base + 0x08, nullptr, const_cast<char*>(a2), firstLength);

    unsigned int secondLength = MeasureNulTerminatedLength(a3);
    AssignStringRange_429B60(base + 0x24, nullptr, const_cast<char*>(a3), secondLength);

    return self;
}

HOOK(0x0069D070, Registry_Initialize, "auto");
