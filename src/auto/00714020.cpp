#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

// Callee: AssignStringRange (0x429B60), __thiscall, ret 8
CSF_FUNC(void*, __fastcall, AssignStringRange_429B60, 0x00429B60,
         (void* self, void* edx, char* text, unsigned int length));

// Assigns a NUL-terminated C string into the string member embedded at +0x10.
static void* __fastcall C_008656d0_SetText(void* self, void* /*edx*/, char* text)
{
    unsigned int length = static_cast<unsigned int>(std::strlen(text));
    void* stringMember = static_cast<uint8_t*>(self) + 0x10;
    return AssignStringRange_429B60(stringMember, nullptr, text, length);
}

HOOK(0x00714020, C_008656d0_SetText, "auto");
