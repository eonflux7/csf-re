#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "legacy/string.h"

// Copies one dword from `source` to `destination` when the destination is non-null.
// Packet bind line is authoritative.
CSF_FUNC(std::uint32_t*, __stdcall, CopyDwordIfNotNull, 0x00730830,
         (std::uint32_t* dest, const std::uint32_t* src));

CSF_FUNC(void, __stdcall, RethrowCppException, 0x007AF666,
         (void* exceptionObject, void* throwInformation));

CSF_FUNC(int, __cdecl, ___CxxFrameHandler, 0x007AF226,
         (void* exceptionRecord, void* frame, void* context,
          void* dispatcherContext));

// Exception frame data referenced by the original's C++ frame.
CSF_GLOBAL(std::uint8_t, g_CopyDwordExceptionMetadata, 0x0088C208);

// Copies `count` dwords starting at `destination`, each via
// CopyDwordIfNotNull(destination, source). The loop runs inside the original's
// C++ try block; the catch-all rethrows the current exception.
static int __cdecl CopyDwordArrayWithContext(
    int destination,
    int count,
    int source,
    int context)
{
    // The original leaves the last helper result in EAX; when count is zero
    // the value is never set, so it is passed back as 0.
    int result = 0;

    try
    {
        while (count != 0)
        {
            result = reinterpret_cast<int>(CopyDwordIfNotNull(
                reinterpret_cast<std::uint32_t*>(destination),
                reinterpret_cast<const std::uint32_t*>(source)));

            --count;
            destination += 4;
        }
    }
    catch (...)
    {
        RethrowCppException(nullptr, nullptr);
        __builtin_unreachable();
    }

    return result;
}

HOOK(0x00552EF0, CopyDwordArrayWithContext, "auto");
