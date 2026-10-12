#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

// Element helper used by the range loop: destination first, then source (both stdcall).
// The original also loads the 4th argument (copy context) into ecx before the call;
// this binding follows the packet's bind line, so ecx is not passed.
CSF_FUNC(void, __stdcall, ProcessFortyByteRecord,
         0x004584E0, (void* destination, void* source));

// __CxxThrowException@8 - used to rethrow out of the catch(...) block.
CSF_FUNC(void, __stdcall, RethrowCppException,
         0x007AF666, (void* exceptionObject, void* throwInformation));

// Referenced by the SEH frame emitted for this function.
CSF_FUNC(int, __cdecl, ___CxxFrameHandler,
         0x007AF226, (void* exceptionRecord, void* frame, void* context, void* dispatcher));

// Data referenced by the frame's handler setup (the thunk at 0x826730 loads it).
CSF_GLOBAL(std::uint8_t, g_FortyByteRangeExceptionInformation, 0x00888214);

// Walks the range of 40-byte records [source, sourceEnd), invoking the record
// helper for each pair (destination, source) and advancing both by 40 bytes.
// The fourth argument is a copy context that the original loads into ecx for the
// helper call; the fifth argument is never read by the original.
// Returns the end of the written destination range.
static char* __cdecl CopyFortyByteRangeToDestination(
    char* source,
    char* sourceEnd,
    char* destination,
    int /*copyContext*/,
    int /*unused*/)
{
    try
    {
        while (source != sourceEnd)
        {
            ProcessFortyByteRecord(destination, source);
            destination += 40;
            source += 40;
        }
    }
    catch (...)
    {
        RethrowCppException(nullptr, nullptr);
    }

    return destination;
}

HOOK(0x00458530, CopyFortyByteRangeToDestination, "auto");
