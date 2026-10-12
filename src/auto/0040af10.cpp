#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"

// Per-element copy: (destination record, source record), stdcall, 28-byte records.
CSF_FUNC(uint32_t*, __stdcall, CopyRecord_40AEB0, 0x0040AEB0, (uint32_t* a1, uint32_t* a2));

// Record release used by the cleanup path (stdcall, one argument, 28-byte records).
CSF_FUNC(int, __stdcall, ReleaseRecord_40AEE0, 0x0040AEE0, (int a1));

CSF_FUNC(void, __stdcall, RethrowCppException, 0x007AF666,
         (void* exceptionObject, void* throwInfo));

CSF_FUNC(int, __cdecl, ___CxxFrameHandler, 0x007AF226,
         (void* exceptionRecord, void* frame, void* context, void* dispatcher));

// Compiler-generated EH function-info data referenced by the frame setup: bind by address.
CSF_GLOBAL(std::uint8_t, g_CopyRangeFuncInfo, 0x00886908);

// Walks the half-open range [sourceBegin, sourceEnd) of 28-byte records and copies each one
// into the matching destination slot (same 28-byte stride). If a copy throws, the destination
// slots already filled are released before rethrowing. Returns the past-the-end destination.
static int __cdecl CopyRecordRange(
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
            CopyRecord_40AEB0(static_cast<uint32_t*>(destination),
                              static_cast<uint32_t*>(sourceBegin));
            destination = static_cast<std::uint8_t*>(destination) + 0x1C;
            sourceBegin = static_cast<std::uint8_t*>(sourceBegin) + 0x1C;
        }
    }
    catch (...)
    {
        // Release every destination record already filled in [begin, current).
        void* released = destinationBegin;

        while (released != destination)
        {
            ReleaseRecord_40AEE0(static_cast<int>(reinterpret_cast<std::intptr_t>(released)));
            released = static_cast<std::uint8_t*>(released) + 0x1C;
        }

        RethrowCppException(nullptr, nullptr);
        __assume(0);
    }

    return static_cast<int>(reinterpret_cast<std::intptr_t>(destination));
}

HOOK(0x0040AF10, CopyRecordRange, "auto");
