#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

// Binds the original Profiles_AppendStringRange (0x43EB30): __thiscall with two stack args, ret 8.
CSF_FUNC(void*, __fastcall, Profiles_AppendStringRange, 0x0043EB30, (void* self, int a2, void* a3, unsigned int a4));

// Original computes strlen(text) inline (no CRT call), then forwards this (ecx), text and length
// to Profiles_AppendStringRange. Returns the callee's eax unchanged.
static int __fastcall C_0082d410_AppendResourceString(void* self, void* /*edx*/, const char* text)
{
    const char* end = text;
    while (*end != '\0')
        ++end;
    unsigned int length = static_cast<unsigned int>(end - text);

    void* result = Profiles_AppendStringRange(self, 0, const_cast<char*>(text), length);
    return static_cast<int>(reinterpret_cast<uintptr_t>(result));
}

HOOK(0x0043F750, C_0082d410_AppendResourceString, "auto");
