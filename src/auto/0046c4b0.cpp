#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

// Copies one 8-byte (two-dword) pair from source to destination, only when the destination is non-null.
CSF_FUNC(uint32_t*, __stdcall, CopyTwoDwordsIfDestNonNull, 0x0071A2E0,
         (uint32_t* dest, const uint32_t* src));

// Destroys one 8-byte destination entry during cleanup after a failed copy.
CSF_FUNC(int, __stdcall, DestroyCopiedPair_46C460, 0x0046C460, (int a1));

CSF_FUNC(void, __stdcall, RethrowCppException, 0x007AF666,
         (void* exceptionObject, void* throwInfo));

CSF_FUNC(int, __cdecl, ___CxxFrameHandler, 0x007AF226,
         (void* exceptionRecord, void* frame, void* context, void* dispatcher));

// Compiler-generated EH data/code referenced by the frame setup: bind by address.
CSF_GLOBAL(std::uint8_t, g_CopyPairRangeExceptionHandlerCode, 0x008268F0);
CSF_GLOBAL(std::uint8_t, g_CopyPairRangeEhData, 0x00888E8C);

// Walks the half-open range [source, sourceEnd) of 8-byte pairs and copies each one into the
// destination array (also 8-byte stride). If a copy throws, the destination entries already
// written are destroyed before rethrowing. Returns the past-the-end destination pointer.
static int __cdecl CopyPairRangeToDestination(
    void* source,
    void* sourceEnd,
    void* destination,
    void* /*contextArg*/,
    int /*unusedArg4*/)
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
        // Destroy every destination entry already written in [begin, current).
        void* handled = destinationBegin;

        while (handled != destination)
        {
            DestroyCopiedPair_46C460(static_cast<int>(reinterpret_cast<std::intptr_t>(handled)));
            handled = static_cast<std::uint8_t*>(handled) + 8;
        }

        RethrowCppException(nullptr, nullptr);
        __assume(0);
    }

    return static_cast<int>(reinterpret_cast<std::intptr_t>(destination));
}

HOOK(0x0046C4B0, CopyPairRangeToDestination, "auto");
