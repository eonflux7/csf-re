#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"

// Per-element copy: (destination record, source record), stdcall, 52-byte records.
CSF_FUNC(int, __stdcall, CopyRecord_774F50, 0x00774F50, (void* a1, int a2));

// Record release used by the cleanup path (stdcall, one argument, 52-byte records).
CSF_FUNC(int, __stdcall, MissionScript_ReleaseRecordBuffer, 0x00469270, (int* a1));

CSF_FUNC(void, __stdcall, RethrowCppException, 0x007AF666,
         (void* exceptionObject, void* throwInfo));

CSF_FUNC(int, __cdecl, ___CxxFrameHandler, 0x007AF226,
         (void* exceptionRecord, void* frame, void* context, void* dispatcher));

// Compiler-generated EH function-info data referenced by the frame setup: bind by address.
CSF_GLOBAL(std::uint8_t, g_RecordRange52FuncInfo, 0x00894578);

// Walks the half-open range [sourceBegin, sourceEnd) of 52-byte records and copies each one
// into the matching destination slot (same 52-byte stride). If a copy throws, the destination
// slots already filled are released before rethrowing. Returns the past-the-end destination.
static int __cdecl CopyRecordRange52(
    void* sourceBegin,
    void* sourceEnd,
    void* destination,
    void* /*contextEcx*/,
    int /*unusedArg5*/)
{
    void* const destinationBegin = destination;

    try
    {
        while (sourceBegin != sourceEnd)
        {
            CopyRecord_774F50(destination, static_cast<int>(reinterpret_cast<std::intptr_t>(sourceBegin)));
            destination = static_cast<std::uint8_t*>(destination) + 0x34;
            sourceBegin = static_cast<std::uint8_t*>(sourceBegin) + 0x34;
        }
    }
    catch (...)
    {
        // Release every destination record already filled in [begin, current).
        void* released = destinationBegin;

        while (released != destination)
        {
            MissionScript_ReleaseRecordBuffer(static_cast<int*>(released));
            released = static_cast<std::uint8_t*>(released) + 0x34;
        }

        RethrowCppException(nullptr, nullptr);
        __assume(0);
    }

    return static_cast<int>(reinterpret_cast<std::intptr_t>(destination));
}

HOOK(0x00774F70, CopyRecordRange52, "auto");
