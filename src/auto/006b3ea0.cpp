#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

// Per-element initializer: (destination slot, source element), stdcall, two arguments.
CSF_FUNC(void, __stdcall, InitEntryFromSource_6B33A0, 0x006B33A0, (int a1, void** a2));

// Entry destructor used by the cleanup path (stdcall, one argument).
CSF_FUNC(int, __stdcall, ReleaseEntry_6B3DF0, 0x006B3DF0, (int a1));

CSF_FUNC(void, __stdcall, RethrowCppException, 0x007AF666,
         (void* exceptionObject, void* throwInfo));

CSF_FUNC(int, __cdecl, ___CxxFrameHandler, 0x007AF226,
         (void* exceptionRecord, void* frame, void* context, void* dispatcher));

// Compiler-generated EH function-info data referenced by the frame setup: bind by address.
CSF_GLOBAL(std::uint8_t, g_InitEntryRangeFuncInfo, 0x0088F8C0);

// Walks the half-open range [sourceBegin, sourceEnd) of pointer-pair elements (8-byte stride)
// and initializes each one into the matching destination slot (same 8-byte stride).
// If an initialization throws, the destination slots already initialized are released
// before rethrowing. Returns the past-the-end destination value.
static int __cdecl InitEntryRangeFromSources(
    void** sourceBegin,
    void** sourceEnd,
    int destination,
    void* contextThis,
    int /*unusedArg5*/)
{
    const int destinationBegin = destination;

    try
    {
        while (sourceBegin != sourceEnd)
        {
            InitEntryFromSource_6B33A0(destination, sourceBegin);
            destination += 8;
            sourceBegin += 2;
        }
    }
    catch (...)
    {
        // Release every destination entry already initialized in [begin, current).
        int handled = destinationBegin;

        while (handled != destination)
        {
            ReleaseEntry_6B3DF0(handled);
            handled += 8;
        }

        RethrowCppException(nullptr, nullptr);
        __assume(0);
    }

    return destination;
}

HOOK(0x006B3EA0, InitEntryRangeFromSources, "auto");
