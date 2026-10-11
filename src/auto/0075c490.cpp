#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

// Per-element copy constructor for graph entries (stdcall, two arguments, 44-byte records).
CSF_FUNC(uint32_t*, __stdcall, CopyConstructGraphEntry_75C330, 0x0075C330,
         (uint32_t* destination, uint32_t* source));

// Record destructor used by the cleanup path (stdcall, one argument).
CSF_FUNC(char, __stdcall, DestroyGraphEntry_75C460, 0x0075C460, (int a1));

CSF_FUNC(void, __stdcall, RethrowCppException, 0x007AF666,
         (void* exceptionObject, void* throwInfo));

CSF_FUNC(int, __cdecl, ___CxxFrameHandler, 0x007AF226,
         (void* exceptionRecord, void* frame, void* context, void* dispatcher));

// Compiler-generated EH function-info data referenced by the frame setup thunk: bind by address.
CSF_GLOBAL(std::uint8_t, g_GraphEntryRangeFuncInfo, 0x008932AC);

// Walks the half-open range [sourceBegin, sourceEnd) of 44-byte graph entries and copy-constructs
// each one into the matching destination slot (same 44-byte stride). If a copy throws,
// the destination slots already constructed are destroyed before rethrowing.
// Returns the past-the-end destination pointer.
static int __cdecl CopyConstructGraphEntryRange(
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
            CopyConstructGraphEntry_75C330(static_cast<uint32_t*>(destination),
                                           static_cast<uint32_t*>(sourceBegin));
            destination = static_cast<std::uint8_t*>(destination) + 0x2C;
            sourceBegin = static_cast<std::uint8_t*>(sourceBegin) + 0x2C;
        }
    }
    catch (...)
    {
        // Destroy every destination entry already constructed in [begin, current).
        void* handled = destinationBegin;

        while (handled != destination)
        {
            DestroyGraphEntry_75C460(
                static_cast<int>(reinterpret_cast<std::intptr_t>(handled)));
            handled = static_cast<std::uint8_t*>(handled) + 0x2C;
        }

        RethrowCppException(nullptr, nullptr);
        __assume(0);
    }

    return static_cast<int>(reinterpret_cast<std::intptr_t>(destination));
}

HOOK(0x0075C490, CopyConstructGraphEntryRange, "auto");
