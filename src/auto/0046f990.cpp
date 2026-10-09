#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

// Copy-constructs one 0x1C-byte record from source into destination (stdcall, 2 args).
CSF_FUNC(int*, __stdcall, CopyConstructRecord_410B50, 0x00410B50, (void* destination, const void* source));

// Destroys one 0x1C-byte record (stdcall, 1 arg).
CSF_FUNC(int, __stdcall, DestroyRecord_469270, 0x00469270, (void* record));

CSF_FUNC(void, __stdcall, RethrowCppException, 0x007AF666,
         (void* exceptionObject, void* throwInfo));

CSF_FUNC(int, __cdecl, ___CxxFrameHandler, 0x007AF226,
         (void* exceptionRecord, void* frame, void* context, void* dispatcher));

// Compiler-generated EH stub and FuncInfo data referenced by the frame setup: bind by address.
CSF_GLOBAL(std::uint8_t, g_CopyRecordRangeExceptionHandlerCode, 0x00826A00);
CSF_GLOBAL(std::uint8_t, g_CopyRecordRangeFuncInfo, 0x0088961C);

// Copies the half-open range [source, sourceEnd) of 0x1C-byte records into the
// destination array (same stride), one CopyConstructRecord call per element.
// If a copy throws, the destination slots already built are destroyed before rethrowing.
// Returns the past-the-end destination pointer.
static int __cdecl MissionScript_CopyRecordRange(
    void* source,
    void* sourceEnd,
    void* destination,
    void* /*contextEcx*/,
    int /*unusedArg5*/)
{
    void* const destinationBegin = destination;

    try
    {
        while (source != sourceEnd)
        {
            CopyConstructRecord_410B50(destination, source);
            destination = static_cast<std::uint8_t*>(destination) + 0x1C;
            source = static_cast<std::uint8_t*>(source) + 0x1C;
        }
    }
    catch (...)
    {
        // Destroy every destination record already built in [begin, current).
        void* built = destinationBegin;

        while (built != destination)
        {
            DestroyRecord_469270(built);
            built = static_cast<std::uint8_t*>(built) + 0x1C;
        }

        RethrowCppException(nullptr, nullptr);
        __assume(0);
    }

    return static_cast<int>(reinterpret_cast<std::intptr_t>(destination));
}

HOOK(0x0046F990, MissionScript_CopyRecordRange, "auto");
