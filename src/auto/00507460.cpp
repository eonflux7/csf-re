#include <cstdint>
#include "globals.h"
#include "hook.h"

CSF_GLOBAL(uint8_t, g_byte_82E65C, 0x0082E65C);

CSF_FUNC(int, __fastcall, C_0082d410_AssignResourceString, 0x00429C60, (void* self, void* , const char* text));

// Original: push 0x82e65c; add ecx, 0x170c; call 0x429c60; ret 0
// Passes the embedded sub-object at this+0x170C and the global's address as the text argument.
static int __fastcall AssignDefaultStringToEmbeddedMember(void* self, void* /*edx*/)
{
    void* embeddedMember = static_cast<uint8_t*>(self) + 0x170C;
    const char* defaultText = reinterpret_cast<const char*>(&g_byte_82E65C);
    return C_0082d410_AssignResourceString(embeddedMember, nullptr, defaultText);
}

HOOK(0x507460, AssignDefaultStringToEmbeddedMember, "auto");
