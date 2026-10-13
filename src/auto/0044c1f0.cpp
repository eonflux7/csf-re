#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"

// Per-element copy: (destination record, source record), stdcall, 100-byte records.
CSF_FUNC(uint32_t*, __stdcall, CopyRecord_44C1D0, 0x0044C1D0, (uint32_t* a1, int a2));

// Record release used by the cleanup path (stdcall, one argument, 100-byte records).
CSF_FUNC(int, __stdcall, ReleaseRecord_44BF80, 0x0044BF80, (uint32_t* a1));

CSF_FUNC(void, __stdcall, RethrowCppException, 0x007AF666,
         (void* exceptionObject, void* throwInfo));

CSF_FUNC(int, __cdecl, ___CxxFrameHandler, 0x007AF226,
         (void* exceptionRecord, void* frame, void* context, void* dispatcher));

// Compiler-generated EH function-info data referenced by the frame setup: bind by address.
CSF_GLOBAL(std::uint8_t, g_RecordRangeFuncInfo, 0x00887B04);

// Walks the half-open range [sourceBegin, sourceEnd) of 100-byte records and copies each one
// into the matching destination slot (same 100-byte stride). If a copy throws, the destination
// slots already filled are released before rethrowing. Returns the past-the-end destination.
static uint32_t* __cdecl CopyRecordRange100(
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
            CopyRecord_44C1D0(static_cast<uint32_t*>(sourceBegin),
                              reinterpret_cast<std::intptr_t>(destination) != 0
                                  ? static_cast<int>(reinterpret_cast<std::intptr_t>(destination))
                                  : 0);
            destination = static_cast<std::uint8_t*>(destination) + 0x64;
            sourceBegin = static_cast<std::uint8_t*>(sourceBegin) + 0x64;
        }
    }
    catch (...)
    {
        // Release every destination record already filled in [begin, current).
        void* released = destinationBegin;

        while (released != destination)
        {
            ReleaseRecord_44BF80(static_cast<uint32_t*>(released));
            released = static_cast<std::uint8_t*>(released) + 0x64;
        }

        RethrowCppException(nullptr, nullptr);
        __assume(0);
    }

    return static_cast<uint32_t*>(destination);
}

HOOK(0x0044C1F0, CopyRecordRange100, "auto");
