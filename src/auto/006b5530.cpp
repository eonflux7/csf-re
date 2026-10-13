#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"

// Per-element copy: (destination slot, source record), stdcall. The original pushes the source
// first and the destination second, so the destination is the first stdcall argument.
CSF_FUNC(int*, __stdcall, InitElement_6B5510, 0x006B5510, (void* a1, int a2));

// Element release used by the cleanup path (stdcall, one argument).
CSF_FUNC(int, __stdcall, DestroyElementThroughContext, 0x006B5220, (int* record));

CSF_FUNC(void, __stdcall, RethrowCppException, 0x007AF666,
         (void* exceptionObject, void* throwInfo));

CSF_FUNC(int, __cdecl, ___CxxFrameHandler, 0x007AF226,
         (void* exceptionRecord, void* frame, void* context, void* dispatcher));

// Compiler-generated EH function-info data referenced by the frame setup: bind by address.
CSF_GLOBAL(std::uint8_t, g_CopyElementRangeFuncInfo, 0x0088FAD4);

// Walks the half-open range [sourceBegin, sourceEnd) of 16-byte source records and initializes
// each matching 4-byte destination slot. If an element initialization throws, the destination
// slots already filled are destroyed (stepping 0x10 bytes, as the original cleanup does) before
// rethrowing. Returns the past-the-end destination pointer.
static int* __cdecl CopyElementRange(
    int sourceBegin,
    int sourceEnd,
    int* destination,
    int /*contextEcx*/,
    int /*unusedArg5*/)
{
    int* const destinationBegin = destination;

    try
    {
        while (sourceBegin != sourceEnd)
        {
            InitElement_6B5510(destination, sourceBegin);
            destination += 1;
            sourceBegin += 16;
        }
    }
    catch (...)
    {
        // Destroy every destination element already filled, walking the buffer in 0x10 steps.
        int* released = destinationBegin;

        while (released != destination)
        {
            DestroyElementThroughContext(released);
            released = reinterpret_cast<int*>(reinterpret_cast<std::uint8_t*>(released) + 0x10);
        }

        RethrowCppException(nullptr, nullptr);
        __assume(0);
    }

    return destination;
}

HOOK(0x006B5530, CopyElementRange, "auto");
