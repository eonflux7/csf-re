#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

// Per-element copy constructor: (destination record, source record), stdcall, 28-byte records.
CSF_FUNC(int*, __stdcall, CopyConstructRecord_410B50, 0x00410B50, (void* destination, const void* source));

// Record destructor used by the cleanup path (stdcall, one argument).
CSF_FUNC(int, __stdcall, MissionScript_ReleaseRecordBuffer, 0x00469270, (int* a1));

CSF_FUNC(void, __stdcall, RethrowCppException, 0x007AF666,
         (void* exceptionObject, void* throwInfo));

CSF_FUNC(int, __cdecl, ___CxxFrameHandler, 0x007AF226,
         (void* exceptionRecord, void* frame, void* context, void* dispatcher));

// Compiler-generated EH function-info data referenced by the frame setup: bind by address.
CSF_GLOBAL(std::uint8_t, g_CopyRangeFuncInfo, 0x00886BA0);

// Walks the half-open range [sourceBegin, sourceEnd) of 28-byte records and copy-constructs
// each one into the matching destination slot (same 28-byte stride). If a copy throws,
// the destination slots already constructed are released before rethrowing.
// Returns the past-the-end destination pointer.
static int __cdecl CopyConstructRecordRange(
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
            CopyConstructRecord_410B50(destination, sourceBegin);
            destination = static_cast<std::uint8_t*>(destination) + 0x1C;
            sourceBegin = static_cast<std::uint8_t*>(sourceBegin) + 0x1C;
        }
    }
    catch (...)
    {
        // Release every destination record already constructed in [begin, current).
        void* handled = destinationBegin;

        while (handled != destination)
        {
            MissionScript_ReleaseRecordBuffer(static_cast<int*>(handled));
            handled = static_cast<std::uint8_t*>(handled) + 0x1C;
        }

        RethrowCppException(nullptr, nullptr);
        __assume(0);
    }

    return static_cast<int>(reinterpret_cast<std::intptr_t>(destination));
}

HOOK(0x00410C20, CopyConstructRecordRange, "auto");
