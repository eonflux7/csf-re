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
CSF_GLOBAL(std::uint8_t, g_TwoDwordCopyExceptionMetadata, 0x00886CE0);

// Copies `count` 8-byte elements starting at `destination`, each via
// CopyTwoDwordsIfDestNonNull(destination, source) with `context` as the
// helper's implicit this-value. The loop runs inside the original's C++
// try block; the catch-all rethrows the current exception.
static std::uint32_t* __cdecl CopyTwoDwordArrayWithContext(
    std::uint32_t* destination,
    std::uint32_t count,
    std::uint32_t* source,
    void* context)
{
    // The original leaves an incidental EAX value when count is zero.
    std::uint32_t* result;

    try
    {
        while (count != 0)
        {
            result = CopyTwoDwordsIfDestNonNull(destination, source);

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

HOOK(0x0042FD90, CopyTwoDwordArrayWithContext, "auto");
