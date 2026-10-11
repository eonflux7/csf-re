#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"

// Callee bindings (one name and signature per address).
CSF_FUNC(char*, __cdecl, GetLocalSessionObject, 0x00668B70, ());
CSF_FUNC(int, __fastcall, SetSessionOptionString, 0x00680D30, (void* self, void* edx, const char* key, char* value));
CSF_FUNC(int, __cdecl, _sprintf, 0x007AF9BB, (char* buffer, const char* format, ...));

// Hook target: refreshes the "MaxJugs" option from the spawn-point count stored at +0x100.
static int __fastcall MpSpawnPoints_RefreshLocalSelection(int* self, void* /*edx*/)
{
    char* localSession = GetLocalSessionObject();
    if (!localSession)
        return 0;

    // Stack text buffer (12 bytes, ebp-0xC .. ebp-0x1).
    alignas(8) uint8_t valueBytes[12];
    char* valueText = reinterpret_cast<char*>(valueBytes);

    int maxJugs = CSF_AT(int, self, 0x100);

    valueText[0] = 0;
    std::memset(valueText + 1, 0, 4);
    std::memset(valueText + 5, 0, 4);
    valueText[9] = 0;

    _sprintf(valueText, reinterpret_cast<const char*>(CSF_ADDR_DATA(0x0083090C)), maxJugs);

    void* sessionForOption = GetLocalSessionObject();
    return SetSessionOptionString(sessionForOption, nullptr, CSF_STR(0x00867624, "MaxJugs"), valueText);
}

HOOK(0x0073C8A0, MpSpawnPoints_RefreshLocalSelection, "auto");
