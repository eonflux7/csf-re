#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Forwards a non-null C string to the object's vtable slot 3 (+0xC),
// passing the string and its length. Returns 0 for a null pointer.
static int __fastcall C_0085ec74_ForwardStringWithLength(C_0085ec74* self, void* /*edx*/, const char* text)
{
    if (text == nullptr)
        return 0;

    // Length computed inline, matching the original's byte loop (no CRT call).
    uint32_t textLength = 0;
    while (text[textLength] != '\0')
        ++textLength;

    auto forwardSlot = reinterpret_cast<int (__fastcall*)(void*, void*, const char*, uint32_t)>(CSF_AT(void**, self, 0)[3]);
    return forwardSlot(self, nullptr, text, textLength);
}

HOOK(0x6A04B0, C_0085ec74_ForwardStringWithLength, "auto");
