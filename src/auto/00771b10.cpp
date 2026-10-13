#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// Copies one 12-byte element (3 dwords) from src to dst; returns the destination pointer.
CSF_FUNC(std::uint32_t*, __stdcall, CopyConstructTwelveByteElement, 0x004A3200,
         (std::uint32_t* dst, std::uint32_t* src));

CSF_FUNC(void, __stdcall, RethrowCppException, 0x007AF666,
         (void* exceptionObject, void* throwInformation));

CSF_FUNC(int, __cdecl, ___CxxFrameHandler, 0x007AF226,
         (void* exceptionRecord, void* frame, void* context,
          void* dispatcherContext));

// Exception handler metadata referenced by the original's C++ frame.
CSF_GLOBAL(std::uint8_t, g_TwelveByteCopyExceptionMetadata, 0x00893FF8);

// Copies `count` 12-byte elements from `source` to `destination`, each via
// CopyConstructTwelveByteElement, inside the original's C++ try block. The
// catch-all rethrows the current exception. The helper's `this` argument (ecx)
// is ignored by the __stdcall callee, so it is not forwarded.
static std::uint32_t* __cdecl CopyTwelveByteArrayWithContext(
    std::uint32_t* destination,
    std::uint32_t count,
    std::uint32_t* source,
    void* /*unusedContext*/,
    void* /*unusedArg*/)
{
    // The original leaves its last EAX value when count is zero; pass nullptr.
    std::uint32_t* result = nullptr;

    try
    {
        while (count != 0)
        {
            result = CopyConstructTwelveByteElement(destination, source);

            --count;
            destination += 3;
        }
    }
    catch (...)
    {
        RethrowCppException(nullptr, nullptr);
        __builtin_unreachable();
    }

    return result;
}

HOOK(0x00771B10, CopyTwelveByteArrayWithContext, "auto");
