#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

// Helper copies one 8-byte element. Packet bind line is authoritative.
CSF_FUNC(std::uint32_t*, __stdcall, CopyTwoDwordsIfDestNonNull, 0x0071A2E0,
         (std::uint32_t* dest, const std::uint32_t* src));

CSF_FUNC(void, __stdcall, RethrowCppException, 0x007AF666,
         (void* exceptionObject, void* throwInformation));

CSF_FUNC(int, __cdecl, ___CxxFrameHandler, 0x007AF226,
         (void* exceptionRecord, void* frame, void* context,
          void* dispatcherContext));

// Exception handler data referenced by the original's C++ frame.
CSF_GLOBAL(std::uint8_t, g_TwoDwordRangeExceptionMetadata, 0x00889FDC);

// Copies `count` 8-byte elements starting at `destination`, each via
// CopyTwoDwordsIfDestNonNull(destination, source). `context` is the value the
// original loads into ecx for the helper call; the stdcall helper ignores it.
// The loop runs inside the original's C++ try block; the catch-all rethrows.
static int __cdecl CopyTwoDwordRangeWithContext(
    std::uint32_t* destination,
    std::uint32_t count,
    const std::uint32_t* source,
    void* context)
{
    // With count == 0 the original returns whatever EAX held; pass 0.
    int result = 0;

    (void)context;

    try
    {
        while (count != 0)
        {
            result = static_cast<int>(reinterpret_cast<std::uintptr_t>(
                CopyTwoDwordsIfDestNonNull(destination, source)));

            --count;
            destination += 2;
        }
    }
    catch (...)
    {
        RethrowCppException(nullptr, nullptr);
        __builtin_unreachable();
    }

    return result;
}

HOOK(0x00497010, CopyTwoDwordRangeWithContext, "auto");
