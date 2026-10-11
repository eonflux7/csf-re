#include <cstdint>
#include "globals.h"
#include "hook.h"

// Bind by address. The first call takes no arguments; the second is the same
// address called with (key, value), which is the only way the original uses it.
CSF_FUNC(char*, __cdecl, CfgGetStringPtr, 0x00668B70, ());
CSF_FUNC(int, __cdecl, _sprintf, 0x007AF9BB, (char* buffer, const char* format, ...));
CSF_FUNC(int, __fastcall, ApplyJugConfigValue, 0x00680D30, (void* self, void* edx, const char* key, char* value));

// Stores numJugs at this+0x108, then writes the "NumJugs" and "MaxJugs" entries
// using the formatted values of this+0x108 and this+0x100.
static int __fastcall ApplyJugCountConfig(int* self, void* /*edx*/, int numJugs)
{
    CSF_AT(int, self, 0x108) = numJugs;

    int lookupResult = reinterpret_cast<int>(CfgGetStringPtr());
    if (lookupResult == 0)
    {
        return lookupResult;
    }

    alignas(8) char buffer[0xC];
    buffer[0] = 0;
    CSF_AT(int, buffer, 1) = 0;
    CSF_AT(int, buffer, 5) = 0;
    CSF_AT(char, buffer, 9) = 0;

    _sprintf(buffer,
             reinterpret_cast<const char*>(CSF_ADDR_DATA(0x0083090C)),
             CSF_AT(int, self, 0x108));

    using CfgLookupWithValue = char* (__cdecl*)(const char* key, char* value);
    auto cfgLookup = reinterpret_cast<CfgLookupWithValue>(CSF_ADDR_FN(0x00668B70));

    int numJugsEntry = reinterpret_cast<int>(cfgLookup(CSF_STR(0x0086762C, "NumJugs"), buffer));
    ApplyJugConfigValue(reinterpret_cast<void*>(numJugsEntry), nullptr,
                        CSF_STR(0x0086762C, "NumJugs"), buffer);

    _sprintf(buffer,
             reinterpret_cast<const char*>(CSF_ADDR_DATA(0x0083090C)),
             CSF_AT(int, self, 0x100));

    int maxJugsEntry = reinterpret_cast<int>(cfgLookup(CSF_STR(0x00867624, "MaxJugs"), buffer));
    return ApplyJugConfigValue(reinterpret_cast<void*>(maxJugsEntry), nullptr,
                               CSF_STR(0x00867624, "MaxJugs"), buffer);
}

HOOK(0x0073C730, ApplyJugCountConfig, "auto");
