#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_FUNC(char*, __cdecl, _strncpy, 0x007AFEC0, (char* dest, const char* src, size_t count));
CSF_FUNC(char*, __cdecl, _strstr, 0x007B2350, (char* Str, char* SubStr));
CSF_FUNC(char*, __cdecl, __strupr, 0x007B302E, (char* String));

// DirectSound enumeration callback. Matches the device GUID against the
// expected one; if the device description contains "SOUNDMAX" (case-insensitive),
// clears the byte at context+4 and returns 0 (FALSE) to stop enumeration.
// Returns 1 (TRUE) to continue enumeration otherwise.
static int __stdcall DSoundEnum_MatchSoundMaxCallback(const uint8_t* guid, const char* description, const char* moduleName, const void** context)
{
    char Destination[200];

    if (!guid || !context || memcmp(guid, *context, 0x10u))
        return 1;

    memset(Destination, 0, sizeof(Destination));
    _strncpy(Destination, description, 0xC7u);
    __strupr(Destination);

    if (_strstr(Destination, const_cast<char*>(CSF_STR(0x00863B98, "SOUNDMAX"))))
        CSF_AT(uint8_t, context, 4) = 0;

    return 0;
}

HOOK(0x0070EED0, DSoundEnum_MatchSoundMaxCallback, "auto");
