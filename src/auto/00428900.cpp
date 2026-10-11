#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, CoreZlibLog_BeginRecord, 0x006951B0, (void* self, void* edx, char a2, char a3, int a4));
CSF_FUNC(unsigned int, __fastcall, CoreZlibLog_EndGroup_6951D0, 0x006951D0, (void* self, void* edx));
CSF_FUNC(unsigned int, __fastcall, CoreZlibLog_WriteInteger, 0x006952F0, (void* self, void* edx, int a2, char* a3));

CSF_GLOBAL(char*, g_pszKey_CONTROL, 0x00899524);
CSF_GLOBAL(char*, g_pszKey_DEVICE, 0x00899528);
CSF_GLOBAL(char*, g_pszKey_DEVICE_ID, 0x0089952C);

// Writes a table of 20-byte entries (5 dwords each: device type, device id, ...)
// into the zlib log, one record group per entry. Returns a3 (the entry count).
static unsigned int __cdecl CheatsLog_WriteControlEntries(S_00694cd0* logger, int* entries, unsigned int entryCount)
{
    unsigned int result = 0;
    if (entries)
    {
        result = entryCount;
        int* entry = entries;
        for (unsigned int index = 0; index < entryCount; entry += 5)
        {
            CoreZlibLog_BeginRecord(logger, nullptr, 0, 0, 0);
            CoreZlibLog_WriteInteger(logger, nullptr, static_cast<int>(index), g_pszKey_CONTROL);
            CoreZlibLog_WriteInteger(logger, nullptr, entry[0], g_pszKey_DEVICE);
            CoreZlibLog_WriteInteger(logger, nullptr, entry[1], g_pszKey_DEVICE_ID);
            CoreZlibLog_EndGroup_6951D0(logger, nullptr);
            result = entryCount;
            ++index;
        }
    }
    return result;
}

HOOK(0x428900, CheatsLog_WriteControlEntries, "auto");
