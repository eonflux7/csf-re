#include <cstddef>

#include "globals.h"
#include "hook.h"

CSF_FUNC(char*, __cdecl, _strncpy, 0x007AFEC0, (char* dest, const char* src, size_t count));

static char* __fastcall SceneLoader_SetObjectName(char* self, void* /*edx*/, char* source)
{
    char* nameBuffer = self + 0x58;
    char* result = _strncpy(nameBuffer, source, 0x20u);
    nameBuffer[31] = 0;
    return result;
}

HOOK(0x0048E0B0, SceneLoader_SetObjectName, "auto");
