#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"

// Per-element copy: (destination record, source record), stdcall, 60-byte records.
CSF_FUNC(int, __stdcall, CopyRecord_6D4C70, 0x006D4C70, (void* a1, void* a2));

// Record release used by the cleanup path (stdcall, one argument, 60-byte records).
CSF_FUNC(char, __stdcall, ReleaseRecord_6D49D0, 0x006D49D0, (int a1));

CSF_FUNC(void, __stdcall, RethrowCppException, 0x007AF666,
         (void* exceptionObject, void* throwInfo));

CSF_FUNC(int, __cdecl, ___CxxFrameHandler, 0x007AF226,
         (void* exceptionRecord, void* frame, void* context, void* dispatcher));

// Compiler-generated EH data referenced by the frame setup (funclet at 0x827c50): bind by address.
CSF_GLOBAL(std::uint8_t, g_CopyClumpRangeFuncInfo, 0x008907B4);

// Walks the half-open range [sourceBegin, sourceEnd) of 60-byte records and copies each one
// into the matching destination slot (same 60-byte stride). If a copy throws, the destination
// slots already filled are released before rethrowing. Returns the past-the-end destination.
static int __cdecl CopyClumpRecordRange(
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
            CopyRecord_6D4C70(destination, sourceBegin);
            destination = static_cast<std::uint8_t*>(destination) + 0x3C;
            sourceBegin = static_cast<std::uint8_t*>(sourceBegin) + 0x3C;
        }
    }
    catch (...)
    {
        // Release every destination record already filled in [begin, current).
        void* released = destinationBegin;

        while (released != destination)
        {
            ReleaseRecord_6D49D0(static_cast<int>(reinterpret_cast<std::intptr_t>(released)));
            released = static_cast<std::uint8_t*>(released) + 0x3C;
        }

        RethrowCppException(nullptr, nullptr);
        __assume(0);
    }

    return static_cast<int>(reinterpret_cast<std::intptr_t>(destination));
}

HOOK(0x006D4C90, CopyClumpRecordRange, "auto");
