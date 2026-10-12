#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"

// Per-element converter: (destination record, source value), stdcall, 60-byte records.
CSF_FUNC(char*, __stdcall, ConvertRecord_774EF0, 0x00774EF0, (char* a1, int a2));

// Record release used by the cleanup path (stdcall, one argument, 60-byte records).
CSF_FUNC(char, __stdcall, ReleaseRecord_774F10, 0x00774F10, (int a1));

CSF_FUNC(void, __stdcall, RethrowCppException, 0x007AF666,
         (void* exceptionObject, void* throwInfo));

CSF_FUNC(int, __cdecl, ___CxxFrameHandler, 0x007AF226,
         (void* exceptionRecord, void* frame, void* context, void* dispatcher));

// Compiler-generated EH function-info data referenced by the frame setup: bind by address.
CSF_GLOBAL(std::uint8_t, g_ConvertRangeFuncInfo, 0x008945C8);

// Walks the half-open range [sourceBegin, sourceEnd) of 60-byte records and converts each one
// into the matching destination slot (same 60-byte stride). If a conversion throws, the
// destination slots already filled are released before rethrowing. Returns the past-the-end
// destination pointer.
static uint32_t* __cdecl ConvertRecordRange(
    void* sourceBegin,
    void* sourceEnd,
    void* destination,
    int /*contextEcx*/,
    int /*unusedArg5*/)
{
    void* const destinationBegin = destination;

    try
    {
        while (sourceBegin != sourceEnd)
        {
            ConvertRecord_774EF0(static_cast<char*>(destination),
                                 static_cast<int>(reinterpret_cast<std::intptr_t>(sourceBegin)));
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
            ReleaseRecord_774F10(static_cast<int>(reinterpret_cast<std::intptr_t>(released)));
            released = static_cast<std::uint8_t*>(released) + 0x3C;
        }

        RethrowCppException(nullptr, nullptr);
        __assume(0);
    }

    return static_cast<uint32_t*>(destination);
}

HOOK(0x00775000, ConvertRecordRange, "auto");
