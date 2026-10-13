#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"

// Per-element copy: (destination record, source record), stdcall, 36-byte records on both sides.
// The original also loads its fourth argument into ecx before the call; the callee never reads ecx.
CSF_FUNC(int, __stdcall, CopyNameRecordFromSource, 0x006C9080, (void* a1, int a2));

// Record release used by the cleanup path (stdcall, one argument).
CSF_FUNC(int, __stdcall, MissionScript_ReleaseRecordBuffer, 0x00469270, (int* a1));

CSF_FUNC(void, __stdcall, RethrowCppException, 0x007AF666,
         (void* exceptionObject, void* throwInfo));

CSF_FUNC(int, __cdecl, ___CxxFrameHandler, 0x007AF226,
         (void* exceptionRecord, void* frame, void* context, void* dispatcher));

// Compiler-generated EH function-info data referenced by the frame setup: bind by address.
CSF_GLOBAL(std::uint8_t, g_MissionRecordRangeFuncInfo, 0x008904CC);

// Walks the half-open range [sourceBegin, sourceEnd) of 36-byte source records and copies each one
// into the destination slot (36-byte stride too). If a copy throws, the destination slots already
// filled are released before rethrowing. Returns the past-the-end destination.
static int __cdecl CopyMissionRecordRange(
    int sourceBegin,
    int sourceEnd,
    int destination,
    int /*contextEcx*/,
    int /*unusedArg5*/)
{
    const int destinationBegin = destination;

    try
    {
        while (sourceBegin != sourceEnd)
        {
            CopyNameRecordFromSource(reinterpret_cast<void*>(static_cast<std::intptr_t>(destination)),
                                     sourceBegin);
            destination += 0x24;
            sourceBegin += 0x24;
        }
    }
    catch (...)
    {
        // Release every destination record already filled in [begin, current).
        int released = destinationBegin;

        while (released != destination)
        {
            MissionScript_ReleaseRecordBuffer(reinterpret_cast<int*>(static_cast<std::intptr_t>(released)));
            released += 0x24;
        }

        RethrowCppException(nullptr, nullptr);
        __assume(0);
    }

    return destination;
}

HOOK(0x006C90C0, CopyMissionRecordRange, "auto");

