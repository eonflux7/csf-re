#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

// Per-element copy constructor: (source element, destination element), stdcall, 80-byte records.
CSF_FUNC(int, __stdcall, CopyConstructElement_516960, 0x00516960, (void* a1, void* a2));

// Element destructor used by the cleanup path (stdcall, one argument).
CSF_FUNC(int, __stdcall, DestroyElement_514F30, 0x00514F30, (int* a1));

CSF_FUNC(void, __stdcall, RethrowCppException, 0x007AF666,
         (void* exceptionObject, void* throwInfo));

CSF_FUNC(int, __cdecl, ___CxxFrameHandler, 0x007AF226,
         (void* exceptionRecord, void* frame, void* context, void* dispatcher));

// Compiler-generated EH function-info data referenced by the frame setup: bind by address.
CSF_GLOBAL(std::uint8_t, g_ElementRangeFuncInfo, 0x0088B0B0);

// Walks the half-open range [sourceBegin, sourceEnd) of 80-byte records and copy-constructs
// each one into the matching destination slot (same 80-byte stride). If a copy throws,
// the destination slots already constructed are destroyed before rethrowing.
// Returns the past-the-end destination pointer.
static int __cdecl CopyConstructElementRange(
    void* sourceBegin,
    void* sourceEnd,
    void* destination,
    void* contextArg,
    int /*unusedArg5*/)
{
    void* const destinationBegin = destination;

    try
    {
        while (sourceBegin != sourceEnd)
        {
            CopyConstructElement_516960(sourceBegin, destination);
            destination = static_cast<std::uint8_t*>(destination) + 0x50;
            sourceBegin = static_cast<std::uint8_t*>(sourceBegin) + 0x50;
        }
    }
    catch (...)
    {
        // Destroy every destination element already constructed in [begin, current).
        void* handled = destinationBegin;

        while (handled != destination)
        {
            DestroyElement_514F30(static_cast<int*>(handled));
            handled = static_cast<std::uint8_t*>(handled) + 0x50;
        }

        RethrowCppException(nullptr, nullptr);
        __assume(0);
    }

    return static_cast<int>(reinterpret_cast<std::intptr_t>(destination));
}

HOOK(0x00516980, CopyConstructElementRange, "auto");
