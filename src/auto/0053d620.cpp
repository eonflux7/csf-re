#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// 0x004FDBB0 — returns the object stored in global 0x008B0568.
CSF_FUNC(int, __cdecl, GetGlobalValue_8B0568, 0x4FDBB0, ());

// 0x00501E50 — method invoked on the fetched object; returns a _String_base*.
CSF_FUNC(void*, __fastcall, ReleaseMenuStringTable, 0x501E50, (unsigned int* self, void* edx));

// Thin wrapper: fetch the global object, then forward to its string accessor.
static void* __cdecl MenuMissionInfo_FetchGlobalString()
{
    return ReleaseMenuStringTable(
        reinterpret_cast<unsigned int*>(GetGlobalValue_8B0568()), nullptr);
}

HOOK(0x53D620, MenuMissionInfo_FetchGlobalString, "auto");
