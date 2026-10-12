#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

// strncpy(dest, src, count) from the C runtime, bound by address.
CSF_FUNC(char*, __cdecl, _strncpy, 0x007AFEC0, (char* dest, const char* src, size_t count));

// Copies up to 0x100 bytes of Source into the buffer that starts one byte past `this`
// (the original increments ecx before pushing it) and returns that destination pointer.
static char* __fastcall NetCogibleDdl_ProcessSource(char* self, void* /*edx*/, char* Source)
{
    char* destination = self + 1;
    return _strncpy(destination, Source, 0x100);
}

HOOK(0x00749AA0, NetCogibleDdl_ProcessSource, "auto");
