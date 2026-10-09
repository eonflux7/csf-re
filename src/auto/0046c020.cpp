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

// Exception frame FuncInfo data referenced by the original's C++ frame.
CSF_GLOBAL(std::uint8_t, g_TwoDwordCopyFrameInfo, 0x00888CFC);

// Copies `count` 8-byte elements starting at `destination`, each via
// CopyTwoDwordsIfDestNonNull(destination, source). `context` is the value the
// original loads into ecx for the helper call; the bound helper is __stdcall,
// so it is not forwarded. The loop runs inside the original's C++ try block;
// the catch-all rethrows the current exception.
static int __cdecl CopyTwoDwordArrayWithContext(
    int destinationAddress,
    int count,
    int sourceAddress,
    int context,
    int unusedArg)
{
    (void)context;
    (void)unusedArg;

    auto* destination = reinterpret_cast<std::uint32_t*>(destinationAddress);
    auto* source = reinterpret_cast<const std::uint32_t*>(sourceAddress);
    auto remaining = static_cast<std::uint32_t>(count);

    // The original leaves an incidental EAX value when count is zero; pass 0.
    std::uint32_t* result = nullptr;

    try
    {
        while (remaining != 0)
        {
            result = CopyTwoDwordsIfDestNonNull(destination, source);

            --remaining;
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

HOOK(0x0046C020, CopyTwoDwordArrayWithContext, "auto");
