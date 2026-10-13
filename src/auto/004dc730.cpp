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

// Copies one 16-byte element from src to dst; returns the destination pointer.
CSF_FUNC(std::uint32_t*, __stdcall, CopySixteenByteElement, 0x006D9E80,
         (std::uint32_t* dst, std::uint32_t* src));

CSF_FUNC(void, __stdcall, RethrowCppException, 0x007AF666,
         (void* exceptionObject, void* throwInformation));

CSF_FUNC(int, __cdecl, ___CxxFrameHandler, 0x007AF226,
         (void* exceptionRecord, void* frame, void* context,
          void* dispatcherContext));

// Exception handler data referenced by the original's C++ frame.
CSF_GLOBAL(std::uint8_t, g_SixteenByteCopyExceptionMetadata, 0x0088A240);

// Copies `count` 16-byte elements from `source` (fixed) to `destination`
// (advancing 16 bytes per element), each via CopySixteenByteElement, inside the
// original's C++ try block. The catch-all rethrows the current exception.
// The helper's ecx argument is ignored by the __stdcall callee, so it is not
// forwarded.
static int __cdecl CopySixteenByteArrayWithContext(
    std::uint32_t* destination,
    std::uint32_t count,
    std::uint32_t* source,
    void* /*unusedContext*/,
    void* /*unusedArg*/)
{
    // The original leaves its last EAX value when count is zero; pass 0.
    std::uint32_t* result = nullptr;

    try
    {
        while (count != 0)
        {
            result = CopySixteenByteElement(destination, source);

            --count;
            destination = reinterpret_cast<std::uint32_t*>(
                reinterpret_cast<std::uint8_t*>(destination) + 16);
        }
    }
    catch (...)
    {
        RethrowCppException(nullptr, nullptr);
        __builtin_unreachable();
    }

    return static_cast<int>(reinterpret_cast<std::uintptr_t>(result));
}

HOOK(0x004DC730, CopySixteenByteArrayWithContext, "auto");
