#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"

// Per-element copy: (destination record, source record). The original sets ecx to its
// own `this` (saved from the 4th argument) before the call, so it is bound as a method.
CSF_FUNC(uint32_t, __fastcall, CopyRecord_69EA90, 0x0069EA90,
         (void* self, void* edx, void* destRecord, void* srcRecord));

// Record release used by the cleanup path (stdcall, one argument).
CSF_FUNC(int, __stdcall, MissionScript_ReleaseRecordBuffer, 0x00469270, (int* a1));

CSF_FUNC(void, __stdcall, RethrowCppException, 0x007AF666,
         (void* exceptionObject, void* throwInfo));

CSF_FUNC(int, __cdecl, ___CxxFrameHandler, 0x007AF226,
         (void* exceptionRecord, void* frame, void* context, void* dispatcher));

// Compiler-generated EH function-info data referenced by the frame setup: bind by address.
CSF_GLOBAL(std::uint8_t, g_CopyRangeFuncInfo, 0x0088F56C);

// Walks the half-open range [sourceBegin, sourceEnd) of 44-byte records and copies each one
// into the matching destination slot (same 44-byte stride). The 4th argument is the
// `this` passed to the per-element copy. If a copy throws, the destination slots already
// filled are released before rethrowing. Returns the past-the-end destination.
static int __cdecl CopyRecordRange44(
    void* sourceBegin,
    void* sourceEnd,
    void* destination,
    void* contextThis,
    int /*unusedArg5*/)
{
    void* const destinationBegin = destination;

    try
    {
        while (sourceBegin != sourceEnd)
        {
            CopyRecord_69EA90(contextThis, nullptr,
                              destination, sourceBegin);
            destination = static_cast<std::uint8_t*>(destination) + 0x2C;
            sourceBegin = static_cast<std::uint8_t*>(sourceBegin) + 0x2C;
        }
    }
    catch (...)
    {
        // Release every destination record already filled in [begin, current).
        void* released = destinationBegin;

        while (released != destination)
        {
            MissionScript_ReleaseRecordBuffer(static_cast<int*>(released));
            released = static_cast<std::uint8_t*>(released) + 0x2C;
        }

        RethrowCppException(nullptr, nullptr);
        __assume(0);
    }

    return static_cast<int>(reinterpret_cast<std::intptr_t>(destination));
}

HOOK(0x0069EAE0, CopyRecordRange44, "auto");
