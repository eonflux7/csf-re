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

// Callees, bound by original address.
CSF_FUNC(int*, __fastcall, NetSession_PrepareStationRecord, 0x0073D9C0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, NetSession_ReadStationStatus, 0x0065E200, (void* self, void* edx));
CSF_FUNC(void, __fastcall, NetSession_SetDumpMode, 0x0065E130, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, NetSession_ClearDumpCounter, 0x00651C70, (void* self, void* edx, uint32_t* a2));
CSF_FUNC(void, __fastcall, NetSession_ReleaseDumpBuffer, 0x00651B40, (void* self, void* edx));

static int __cdecl NetSession_FlushStationDump()
{
    // Stack objects of the original frame:
    //   stationHandle  at [ebp-0x28], 4 bytes
    //   dumpBuffer     at [ebp-0x24], 0x20 bytes
    //   clearedCounter at [ebp-0x04], 4 bytes
    int stationHandle = 0;
    alignas(8) uint8_t dumpBuffer[0x20];
    uint32_t clearedCounter = 0;

    NetSession_PrepareStationRecord(&stationHandle, nullptr);
    int status = NetSession_ReadStationStatus(&stationHandle, nullptr);
    NetSession_SetDumpMode(&stationHandle, nullptr, 0);
    clearedCounter = 0;
    NetSession_ClearDumpCounter(dumpBuffer, nullptr, &clearedCounter);
    NetSession_ReleaseDumpBuffer(dumpBuffer, nullptr);
    return status;
}

HOOK(0x0073E5A0, NetSession_FlushStationDump, "auto");
