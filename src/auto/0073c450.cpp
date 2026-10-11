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

// Sleep-like import slot (stdcall, pops its own argument).
using SleepImportFn = void(__stdcall*)(uint32_t);

CSF_GLOBAL(char, g_NetSessionReadyFlag, 0x008A8924);
CSF_GLOBAL(char, g_NetSessionWaitingFlag, 0x008BC120);
CSF_GLOBAL(SleepImportFn, g_SleepImportSlot, 0x0082D05C);

CSF_FUNC(int, __cdecl, GetGlobalValue_75FDF0, 0x0075FDF0, ());
CSF_FUNC(char, __cdecl, DumpSessionStationRecord, 0x006476A0, (int a1));

// Returns the ready flag. If it is not yet set, marks waiting, sleeps in 50-unit
// steps until the ready flag becomes nonzero, then dumps the station record
// located at (global value + 0xC4).
static char NetSession_WaitForReadyAndDumpStation()
{
    char result = g_NetSessionReadyFlag;
    if (!result)
    {
        g_NetSessionWaitingFlag = 1;
        if (!g_NetSessionReadyFlag)
        {
            SleepImportFn sleepImport = g_SleepImportSlot;
            do
            {
                sleepImport(0x32u);
            } while (!g_NetSessionReadyFlag);
        }
        int globalValue = GetGlobalValue_75FDF0();
        int stationRecord = CSF_AT(int, reinterpret_cast<uint8_t*>(globalValue), 0xC4);
        return DumpSessionStationRecord(stationRecord);
    }
    return result;
}

HOOK(0x0073C450, NetSession_WaitForReadyAndDumpStation, "auto");
