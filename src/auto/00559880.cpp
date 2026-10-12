#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

// Per-element copy constructor: (destination record, source record), stdcall, 20-byte records.
CSF_FUNC(int, __stdcall, CopyConstructRecord_559760, 0x00559760, (int a1, uint32_t* a2));

// Record destructor used by the cleanup path (stdcall, one argument).
CSF_FUNC(int, __stdcall, ReleaseRecord_559830, 0x00559830, (int* a1));

CSF_FUNC(void, __stdcall, RethrowCppException, 0x007AF666,
         (void* exceptionObject, void* throwInfo));

CSF_FUNC(int, __cdecl, ___CxxFrameHandler, 0x007AF226,
         (void* exceptionRecord, void* frame, void* context, void* dispatcher));

// Compiler-generated EH function-info data referenced by the frame setup: bind by address.
CSF_GLOBAL(std::uint8_t, g_CopyRangeFuncInfo, 0x0088C664);

// Walks the half-open range [sourceBegin, sourceEnd) of 20-byte records and copy-constructs
// each one into the matching destination slot (same 20-byte stride). If a copy throws,
// the destination slots already constructed are released before rethrowing.
// Returns the past-the-end destination pointer.
static void* __cdecl CopyConstructMpRulesRecordRange(
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
            CopyConstructRecord_559760(reinterpret_cast<int>(destination),
                                       static_cast<uint32_t*>(sourceBegin));
            destination = static_cast<std::uint8_t*>(destination) + 0x14;
            sourceBegin = static_cast<std::uint8_t*>(sourceBegin) + 0x14;
        }
    }
    catch (...)
    {
        // Release every destination record already constructed in [begin, current).
        void* handled = destinationBegin;

        while (handled != destination)
        {
            ReleaseRecord_559830(static_cast<int*>(handled));
            handled = static_cast<std::uint8_t*>(handled) + 0x14;
        }

        RethrowCppException(nullptr, nullptr);
        __assume(0);
    }

    return destination;
}

HOOK(0x00559880, CopyConstructMpRulesRecordRange, "auto");
