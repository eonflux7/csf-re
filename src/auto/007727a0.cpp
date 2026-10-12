#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"

// Per-element copy: (source track record, destination track record), 44-byte records.
CSF_FUNC(int, __stdcall, CopyTrackRecord_772760, 0x00772760, (void* a1, void* a2));

// Record reset used by the cleanup path (stdcall, one argument, 44-byte records).
CSF_FUNC(int, __stdcall, BddTables_ResetRecordBuffers, 0x0051A390, (int* a1));

CSF_FUNC(void, __stdcall, RethrowCppException, 0x007AF666,
         (void* exceptionObject, void* throwInfo));

CSF_FUNC(int, __cdecl, ___CxxFrameHandler, 0x007AF226,
         (void* exceptionRecord, void* frame, void* context, void* dispatcher));

// Compiler-generated EH function-info data referenced by the frame setup: bind by address.
CSF_GLOBAL(std::uint8_t, g_AnimationTrackFuncInfo, 0x00894290);

// Walks the half-open range [sourceBegin, sourceEnd) of 44-byte records and copies each one
// into the matching destination slot (same 44-byte stride). If a copy throws, the destination
// slots already filled are reset before rethrowing. Returns the past-the-end destination.
static int __cdecl AnimationDefs_CopyTrackRange(
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
            CopyTrackRecord_772760(sourceBegin, destination);
            destination = static_cast<std::uint8_t*>(destination) + 0x2C;
            sourceBegin = static_cast<std::uint8_t*>(sourceBegin) + 0x2C;
        }
    }
    catch (...)
    {
        // Reset every destination record already filled in [begin, current).
        void* released = destinationBegin;

        while (released != destination)
        {
            BddTables_ResetRecordBuffers(reinterpret_cast<int*>(released));
            released = static_cast<std::uint8_t*>(released) + 0x2C;
        }

        RethrowCppException(nullptr, nullptr);
        __assume(0);
    }

    return static_cast<int>(reinterpret_cast<std::intptr_t>(destination));
}

HOOK(0x007727A0, AnimationDefs_CopyTrackRange, "auto");
