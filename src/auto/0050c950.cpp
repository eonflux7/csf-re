#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(std::uint32_t*, __fastcall, CopyDwordIfNotNull, 0x00730830,
         (void* context, void* edx, std::uint32_t* destination,
          std::uint32_t* source));

CSF_FUNC(void, __stdcall, RethrowCppException, 0x007AF666,
         (void* exceptionObject, void* throwInformation));

CSF_FUNC(int, __cdecl, ___CxxFrameHandler, 0x007AF226,
         (void* exceptionRecord, void* frame, void* context,
          void* dispatcherContext));

// Exception metadata blob material referenced by the original's C++ frame.
CSF_GLOBAL(std::uint8_t, g_DwordCopyExceptionMetadata, 0x0088AB64);

// Copies `count` dwords from `source` to `destination` through the
// context-based "copy if not null" helper. The loop body runs inside the
// original's C++ try block; its catch-all rethrows the current exception.
static std::uint32_t* __cdecl CopyDwordsIfNotNull(
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
            result = CopyDwordIfNotNull(context, nullptr, destination, source);

            --count;
            ++destination;
        }
    }
    catch (...)
    {
        RethrowCppException(nullptr, nullptr);
        __builtin_unreachable();
    }

    return result;
}

HOOK(0x0050C950, CopyDwordsIfNotNull, "auto");
