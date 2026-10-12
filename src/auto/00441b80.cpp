#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"

// Per-element forward: (destination record, source record), stdcall, 16-byte records.
CSF_FUNC(int, __stdcall, MissionScript_ForwardEntryIfNonNull, 0x004685B0, (int a1, int a2));

// Handle release used by the cleanup path (stdcall, one argument, 16-byte records).
CSF_FUNC(int, __stdcall, MissionScript_FreeAndClearHandle, 0x00468940, (int* handle));

CSF_FUNC(void, __stdcall, RethrowCppException, 0x007AF666,
         (void* exceptionObject, void* throwInfo));

CSF_FUNC(int, __cdecl, ___CxxFrameHandler, 0x007AF226,
         (void* exceptionRecord, void* frame, void* context, void* dispatcher));

// Compiler-generated EH function-info data referenced by the frame setup: bind by address.
CSF_GLOBAL(std::uint8_t, g_ForwardRangeFuncInfo, 0x008874B0);

// Walks the half-open range [sourceBegin, sourceEnd) of 16-byte records and forwards each one
// to the matching destination slot (same 16-byte stride). If a forward throws, the destination
// slots already filled are released before rethrowing. Returns the past-the-end destination.
static int __cdecl DynamicObj_ForwardEntryRange(
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
            MissionScript_ForwardEntryIfNonNull(
                static_cast<int>(reinterpret_cast<std::intptr_t>(destination)),
                static_cast<int>(reinterpret_cast<std::intptr_t>(sourceBegin)));
            destination = static_cast<std::uint8_t*>(destination) + 0x10;
            sourceBegin = static_cast<std::uint8_t*>(sourceBegin) + 0x10;
        }
    }
    catch (...)
    {
        // Release every destination handle already filled in [begin, current).
        void* released = destinationBegin;

        while (released != destination)
        {
            MissionScript_FreeAndClearHandle(reinterpret_cast<int*>(released));
            released = static_cast<std::uint8_t*>(released) + 0x10;
        }

        RethrowCppException(nullptr, nullptr);
        __assume(0);
    }

    return static_cast<int>(reinterpret_cast<std::intptr_t>(destination));
}

HOOK(0x00441B80, DynamicObj_ForwardEntryRange, "auto");
