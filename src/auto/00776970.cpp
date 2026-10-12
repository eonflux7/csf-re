#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"

// Per-element copy: (destination record, source record), stdcall, 44-byte records.
CSF_FUNC(int*, __stdcall, ParseStringEntryItem, 0x00776930, (void* a1, void* a2));

// Per-element reset used by the cleanup path (stdcall, one argument, 44-byte records).
CSF_FUNC(int, __stdcall, BddTables_ResetRecordBuffers, 0x0051A390, (int* a1));

CSF_FUNC(void, __stdcall, RethrowCppException, 0x007AF666,
         (void* exceptionObject, void* throwInfo));

CSF_FUNC(int, __cdecl, ___CxxFrameHandler, 0x007AF226,
         (void* exceptionRecord, void* frame, void* context, void* dispatcher));

// Compiler-generated EH function-info data referenced by the frame setup: bind by address.
CSF_GLOBAL(std::uint8_t, g_ParseStringEntryFuncInfo, 0x00894AC4);

// Walks the half-open range [sourceBegin, sourceEnd) of 44-byte records and parses each one
// into the matching destination slot (same 44-byte stride). If a parse throws, the destination
// slots already filled are reset before rethrowing. Returns the past-the-end destination.
static int __cdecl GameDataDefs_ParseStringEntry(
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
            ParseStringEntryItem(destination, sourceBegin);
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
            BddTables_ResetRecordBuffers(static_cast<int*>(released));
            released = static_cast<std::uint8_t*>(released) + 0x2C;
        }

        RethrowCppException(nullptr, nullptr);
        __assume(0);
    }

    return static_cast<int>(reinterpret_cast<std::intptr_t>(destination));
}

HOOK(0x00776970, GameDataDefs_ParseStringEntry, "auto");
