#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

// Per-element worker used by the cleanup path (stride 8 records).
CSF_FUNC(int, __stdcall, ReleaseDwordPairSlot_46C090, 0x0046C090, (int a1));

// Copies one 8-byte pair from src to dest (dest slot is the first argument).
CSF_FUNC(uint32_t*, __stdcall, CopyTwoDwordsIfDestNonNull, 0x0071A2E0,
         (uint32_t* dest, const uint32_t* src));

CSF_FUNC(void, __stdcall, RethrowCppException, 0x007AF666,
         (void* exceptionObject, void* throwInfo));

CSF_FUNC(int, __cdecl, ___CxxFrameHandler, 0x007AF226,
         (void* exceptionRecord, void* frame, void* context, void* dispatcher));

// Compiler-generated EH data/code referenced by the frame setup: bind by address.
CSF_GLOBAL(std::uint8_t, g_CopyRangeExceptionHandlerCode, 0x008268D0);
CSF_GLOBAL(std::uint8_t, g_CopyRangeFuncInfo, 0x00888D9C);

// Copies the half-open range [source, sourceEnd) of 8-byte pairs into the
// destination array (also 8-byte stride). If a copy throws, the destination
// slots already written are released before rethrowing.
// Returns the past-the-end destination pointer.
static int __cdecl CopyDwordPairRangeWithRollback(
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
            CopyTwoDwordsIfDestNonNull(static_cast<uint32_t*>(destination),
                                       static_cast<const uint32_t*>(source));
            destination = static_cast<std::uint8_t*>(destination) + 8;
            source = static_cast<std::uint8_t*>(source) + 8;
        }
    }
    catch (...)
    {
        // Release every destination slot already processed in [begin, current).
        void* handled = destinationBegin;

        while (handled != destination)
        {
            ReleaseDwordPairSlot_46C090(
                static_cast<int>(reinterpret_cast<std::intptr_t>(handled)));
            handled = static_cast<std::uint8_t*>(handled) + 8;
        }

        RethrowCppException(nullptr, nullptr);
        __assume(0);
    }

    return static_cast<int>(reinterpret_cast<std::intptr_t>(destination));
}

HOOK(0x0046C0C0, CopyDwordPairRangeWithRollback, "auto");
