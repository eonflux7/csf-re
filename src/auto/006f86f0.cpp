#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

// Per-entry processor: (entry record, argument record), stdcall, 44-byte records.
CSF_FUNC(std::uint32_t*, __stdcall, GfxScreenEffects_ProcessEntryIfNonNull, 0x006F86D0,
         (std::uint32_t* entry, std::uint32_t* argument));

// Record destructor used by the cleanup path (stdcall, one argument).
CSF_FUNC(int, __stdcall, DestroyRecord, 0x0069B820, (std::uint32_t* record));

CSF_FUNC(void, __stdcall, RethrowCppException, 0x007AF666,
         (void* exceptionObject, void* throwInfo));

CSF_FUNC(int, __cdecl, ___CxxFrameHandler, 0x007AF226,
         (void* exceptionRecord, void* frame, void* context, void* dispatcher));

// Compiler-generated EH function-info data referenced by the frame setup: bind by address.
CSF_GLOBAL(std::uint8_t, g_ForwardRangeFuncInfo, 0x00891190);

// Walks the half-open range [sourceBegin, sourceEnd) of 44-byte records and processes each
// one against the matching destination slot (same 44-byte stride). If processing throws,
// the destination slots already handled are released before rethrowing.
// Returns the past-the-end destination pointer.
static std::uint32_t* __cdecl GfxScreenEffects_ForwardRangeWithContext(
    void* sourceBegin,
    void* sourceEnd,
    void* destination,
    void* contextEcx,
    int /*unusedArg5*/)
{
    void* const destinationBegin = destination;

    try
    {
        while (sourceBegin != sourceEnd)
        {
            GfxScreenEffects_ProcessEntryIfNonNull(static_cast<std::uint32_t*>(destination),
                                                   static_cast<std::uint32_t*>(sourceBegin));
            destination = static_cast<std::uint8_t*>(destination) + 0x2C;
            sourceBegin = static_cast<std::uint8_t*>(sourceBegin) + 0x2C;
        }
    }
    catch (...)
    {
        // Release every destination record already handled in [begin, current).
        void* handled = destinationBegin;

        while (handled != destination)
        {
            DestroyRecord(static_cast<std::uint32_t*>(handled));
            handled = static_cast<std::uint8_t*>(handled) + 0x2C;
        }

        RethrowCppException(nullptr, nullptr);
        __assume(0);
    }

    return static_cast<std::uint32_t*>(destination);
}

HOOK(0x006F86F0, GfxScreenEffects_ForwardRangeWithContext, "auto");
