#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"

// Copies one 8-byte pair (two dwords) from src to dest when dest is non-null (stdcall, two arguments).
CSF_FUNC(uint32_t*, __stdcall, CopyTwoDwordsIfDestNonNull, 0x0071A2E0,
         (uint32_t* dest, const uint32_t* src));

// Per-element cleanup used by the exception path (stdcall, one argument).
CSF_FUNC(int, __stdcall, PairRange_ReleaseElement, 0x00709A70, (int a1));

CSF_FUNC(void, __stdcall, RethrowCppException, 0x007AF666,
         (void* exceptionObject, void* throwInfo));

CSF_FUNC(int, __cdecl, ___CxxFrameHandler, 0x007AF226,
         (void* exceptionRecord, void* frame, void* context, void* dispatcher));

// Compiler-generated EH function-info data referenced by the frame setup: bind by address.
CSF_GLOBAL(std::uint8_t, g_WaterCopyPairFuncInfo, 0x00891744);

// Walks the half-open range [sourceBegin, sourceEnd) of 8-byte pairs and copies each one
// into the matching destination slot (same 8-byte stride). If a copy throws, the destination
// slots already written are released before rethrowing.
// Returns the past-the-end destination pointer.
static int __cdecl Water_CopyDwordPairRange(
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
            CopyTwoDwordsIfDestNonNull(static_cast<uint32_t*>(destination),
                                       static_cast<const uint32_t*>(sourceBegin));
            destination = static_cast<std::uint8_t*>(destination) + 8;
            sourceBegin = static_cast<std::uint8_t*>(sourceBegin) + 8;
        }
    }
    catch (...)
    {
        // Release every destination element already written in [begin, current).
        void* handled = destinationBegin;

        while (handled != destination)
        {
            PairRange_ReleaseElement(static_cast<int>(reinterpret_cast<std::intptr_t>(handled)));
            handled = static_cast<std::uint8_t*>(handled) + 8;
        }

        RethrowCppException(nullptr, nullptr);
        __assume(0);
    }

    return static_cast<int>(reinterpret_cast<std::intptr_t>(destination));
}

HOOK(0x0070CF70, Water_CopyDwordPairRange, "auto");
