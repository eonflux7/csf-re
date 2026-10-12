#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"

// Per-element copy: (destination record, source record), stdcall, 32-byte records.
CSF_FUNC(void*, __stdcall, CopyRecordToDestination_69B930, 0x0069B930, (int a1, int a2));

// Record release used by the cleanup path (stdcall, one argument, 32-byte records).
CSF_FUNC(int, __stdcall, DestroyRecord, 0x0069B820, (std::uint32_t* record));

CSF_FUNC(void, __stdcall, RethrowCppException, 0x007AF666,
         (void* exceptionObject, void* throwInfo));

CSF_FUNC(int, __cdecl, ___CxxFrameHandler, 0x007AF226,
         (void* exceptionRecord, void* frame, void* context, void* dispatcher));

// Compiler-generated EH function-info data referenced by the frame setup: bind by address.
CSF_GLOBAL(std::uint8_t, g_FillRangeFuncInfo, 0x0088F448);

// Walks the half-open range [sourceBegin, sourceEnd) of 32-byte records and copies each one
// into the matching destination slot (same 32-byte stride). If a copy throws, the destination
// slots already filled are destroyed before rethrowing. Returns the past-the-end destination.
static int __cdecl FillRecordRange(
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
            CopyRecordToDestination_69B930(
                static_cast<int>(reinterpret_cast<std::intptr_t>(destination)),
                static_cast<int>(reinterpret_cast<std::intptr_t>(sourceBegin)));
            destination = static_cast<std::uint8_t*>(destination) + 0x20;
            sourceBegin = static_cast<std::uint8_t*>(sourceBegin) + 0x20;
        }
    }
    catch (...)
    {
        // Destroy every destination record already filled in [begin, current).
        void* released = destinationBegin;

        while (released != destination)
        {
            DestroyRecord(static_cast<std::uint32_t*>(released));
            released = static_cast<std::uint8_t*>(released) + 0x20;
        }

        RethrowCppException(nullptr, nullptr);
        __assume(0);
    }

    return static_cast<int>(reinterpret_cast<std::intptr_t>(destination));
}

HOOK(0x0069B970, FillRecordRange, "auto");
