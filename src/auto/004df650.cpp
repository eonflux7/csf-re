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

// Exception unwind metadata referenced by the original's C++ frame setup.
CSF_GLOBAL(std::uint8_t, g_TwoDwordCopyUnwindMetadata, 0x0088A4D8);

// Copies `count` 8-byte elements from `source` to `destination`, calling
// CopyTwoDwordsIfDestNonNull once per element. The loop runs inside the
// original's C++ try block; the catch-all rethrows the current exception.
static int __cdecl CopyTwoDwordArrayWithContext(
    std::uint32_t* destination,
    std::uint32_t count,
    std::uint32_t* source,
    void* context,
    int unusedArg)
{
    (void)context;
    (void)unusedArg;

    // The original leaves an incidental EAX value when count is zero; pass 0.
    std::uint32_t* result = nullptr;

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

    return static_cast<int>(reinterpret_cast<std::uintptr_t>(result));
}

HOOK(0x004DF650, CopyTwoDwordArrayWithContext, "auto");
