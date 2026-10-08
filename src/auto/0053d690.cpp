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

// 0x0044FB00: gate predicate checked before the destination lookup (__cdecl, no args).
CSF_FUNC(int, __cdecl, MissionInfo_IsPrerequisiteMet, 0x0044FB00, ());

// 0x004FDBB0: returns the singleton object used as `this` for the destination getter (__cdecl, no args).
CSF_FUNC(void*, __cdecl, GetGlobalValue_8B0568, 0x004FDBB0, ());

// 0x004FE4A0: __thiscall getter returning the default destination object, or null (no stack args).
CSF_FUNC(void*, __fastcall, DefaultDestination_Get, 0x004FE4A0, (void* self, void* edx));

// Returns 1 when the prerequisite holds and the default destination object's flag at +0x1C equals 1.
// Re-fetches the singleton and destination for the second check, matching the original's repeated calls.
static int __cdecl MenuMissionInfo_IsDefaultDestinationFlagSet()
{
    if (!MissionInfo_IsPrerequisiteMet())
    {
        return 0;
    }

    void* singleton = GetGlobalValue_8B0568();
    void* destination = DefaultDestination_Get(singleton, nullptr);
    if (!destination)
    {
        return 0;
    }

    singleton = GetGlobalValue_8B0568();
    destination = DefaultDestination_Get(singleton, nullptr);
    if (CSF_AT(int32_t, destination, 0x1C) == 1)
    {
        return 1;
    }

    return 0;
}

HOOK(0x0053D690, MenuMissionInfo_IsDefaultDestinationFlagSet, "auto");
