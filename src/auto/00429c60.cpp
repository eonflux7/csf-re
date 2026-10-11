#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

// Binds the original AssignStringRange (0x429B60): __thiscall, ret 8.
CSF_FUNC(void*, __fastcall, AssignStringRange_429B60, 0x00429B60, (void* self, void* edx, char* text, unsigned int length));

// Original computes strlen(text) inline (no CRT call), then forwards this/text/length to AssignStringRange.
static int __fastcall C_0082d410_AssignResourceString(void* self, void* /*edx*/, const char* text)
{
    const char* end = text;
    while (*end != '\0')
        ++end;
    unsigned int length = static_cast<unsigned int>(end - text);

    void* result = AssignStringRange_429B60(self, nullptr, const_cast<char*>(text), length);
    return static_cast<int>(reinterpret_cast<uintptr_t>(result));
}

HOOK(0x00429C60, C_0082d410_AssignResourceString, "auto");
