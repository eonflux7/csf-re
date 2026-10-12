#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Per-entry processor: (destination record, source record), stdcall, 16-byte records.
CSF_FUNC(int*, __stdcall, BddTables_ProcessEntryIfNonNull, 0x0051A200, (S_00519c10* entry, int argument));

// Record destructor used by the cleanup path (stdcall, one argument).
CSF_FUNC(int, __stdcall, DestroyElementThroughContext, 0x006B5220, (int* record));

CSF_FUNC(void, __stdcall, RethrowCppException, 0x007AF666,
         (void* exceptionObject, void* throwInfo));

CSF_FUNC(int, __cdecl, ___CxxFrameHandler, 0x007AF226,
         (void* exceptionRecord, void* frame, void* context, void* dispatcher));

// Compiler-generated EH function-info data referenced by the frame setup: bind by address.
CSF_GLOBAL(std::uint8_t, g_ForwardTableFuncInfo, 0x0088B2C4);

// Walks the half-open range [sourceBegin, sourceEnd) of 16-byte entries and processes each
// one against the matching destination slot (same 16-byte stride). If processing throws,
// the destination slots already handled are released before rethrowing.
// Returns the past-the-end destination pointer.
static int* __cdecl BddTables_ForwardTableOperation(
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
            BddTables_ProcessEntryIfNonNull(reinterpret_cast<S_00519c10*>(destination), sourceBegin);
            destination = reinterpret_cast<int*>(static_cast<std::uint8_t*>(static_cast<void*>(destination)) + 0x10);
            sourceBegin += 0x10;
        }
    }
    catch (...)
    {
        // Release every destination record already handled in [begin, current).
        int* handled = destinationBegin;

        while (handled != destination)
        {
            DestroyElementThroughContext(handled);
            handled = reinterpret_cast<int*>(static_cast<std::uint8_t*>(static_cast<void*>(handled)) + 0x10);
        }

        RethrowCppException(nullptr, nullptr);
        __assume(0);
    }

    return destination;
}

HOOK(0x0051A220, BddTables_ForwardTableOperation, "auto");
