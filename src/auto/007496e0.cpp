#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"

// Original C runtime strncpy, bound by address.
CSF_FUNC(char*, __cdecl, _strncpy, 0x007AFEC0, (char* dest, const char* src, size_t count));

// Copies up to 0x80 bytes of Source into the buffer embedded at offset +0x87 of the object.
// __thiscall: ecx = this, one stack argument (Source), callee pops 4 bytes.
static char* __fastcall MpSpawnPoints_SetPrimaryString(char* self, void* /*edx*/, char* source)
{
    char* primaryBuffer = self + 0x87;
    return _strncpy(primaryBuffer, source, 0x80);
}

HOOK(0x007496E0, MpSpawnPoints_SetPrimaryString, "auto");
