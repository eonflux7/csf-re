#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

// Helper copies one dword. The packet bind line is authoritative.
// The original sets ecx to its fourth argument before the call; this
// __stdcall binding does not take a this-value, so that register is not
// forwarded.
CSF_FUNC(std::uint32_t*, __stdcall, CopyDwordIfNotNull, 0x00730830,
         (std::uint32_t* dest, const std::uint32_t* src));

CSF_FUNC(void, __stdcall, RethrowCppException, 0x007AF666,
         (void* exceptionObject, void* throwInformation));

CSF_FUNC(int, __cdecl, ___CxxFrameHandler, 0x007AF226,
         (void* exceptionRecord, void* frame, void* context,
          void* dispatcherContext));

// Exception handler data referenced by the original's C++ frame.
CSF_GLOBAL(std::uint8_t, g_DwordCopyExceptionMetadata, 0x008894DC);

// Copies `count` dwords starting at `destination`, each via
// CopyDwordIfNotNull(destination, source). The loop runs inside the
// original's C++ try block; the catch-all rethrows the current exception.
static int __cdecl CopyDwordArrayWithContext(
    int destination,
    std::uint32_t count,
    int source,
    int context,
    int /*unusedArg5*/)
{
    int result = 0;

    try
    {
        std::uint32_t* dest = reinterpret_cast<std::uint32_t*>(destination);
        const std::uint32_t* src = reinterpret_cast<const std::uint32_t*>(source);

        while (count != 0)
        {
            result = reinterpret_cast<int>(CopyDwordIfNotNull(dest, src));

            --count;
            ++dest;
        }
    }
    catch (...)
    {
        RethrowCppException(nullptr, nullptr);
        __builtin_unreachable();
    }

    return result;
}

HOOK(0x0046EF10, CopyDwordArrayWithContext, "auto");
