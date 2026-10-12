#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

// Per-element copy constructor: (destination record, source record), 60-byte records.
// The original also loads the element context into ecx, which the verified clone does not model.
CSF_FUNC(int, __stdcall, CopyConstructEmitterRecord, 0x006E1540, (void* a1, void* a2));

// Record destructor used by the cleanup path (stdcall, one argument).
CSF_FUNC(int, __stdcall, MissionScript_ReleaseRecordBuffer, 0x00469270, (int* a1));

CSF_FUNC(void, __stdcall, RethrowCppException, 0x007AF666,
         (void* exceptionObject, void* throwInfo));

CSF_FUNC(int, __cdecl, ___CxxFrameHandler, 0x007AF226,
         (void* exceptionRecord, void* frame, void* context, void* dispatcher));

// Compiler-generated EH function-info data referenced by the frame setup: bind by address.
CSF_GLOBAL(std::uint8_t, g_EmitterRangeFuncInfo, 0x00890EA8);

// Walks the half-open range [sourceBegin, sourceEnd) of 60-byte emitter records and
// copy-constructs each one into the matching destination slot (same 60-byte stride).
// If a copy throws, the destination slots already constructed are released before rethrowing.
// Returns the past-the-end destination pointer.
static int __cdecl CopyConstructEmitterRange(
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
            CopyConstructEmitterRecord(destination, sourceBegin);
            destination = static_cast<std::uint8_t*>(destination) + 0x3C;
            sourceBegin = static_cast<std::uint8_t*>(sourceBegin) + 0x3C;
        }
    }
    catch (...)
    {
        // Release every destination record already constructed in [begin, current).
        void* handled = destinationBegin;

        while (handled != destination)
        {
            MissionScript_ReleaseRecordBuffer(static_cast<int*>(handled));
            handled = static_cast<std::uint8_t*>(handled) + 0x3C;
        }

        RethrowCppException(nullptr, nullptr);
        __assume(0);
    }

    return static_cast<int>(reinterpret_cast<std::intptr_t>(destination));
}

HOOK(0x006E1560, CopyConstructEmitterRange, "auto");
