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

// Copies one 12-byte element from src to dst; returns the destination pointer.
CSF_FUNC(std::uint32_t*, __stdcall, CopyConstructTwelveByteElement, 0x004A3200,
         (std::uint32_t* dst, std::uint32_t* src));

CSF_FUNC(void, __stdcall, RethrowCppException, 0x007AF666,
         (void* exceptionObject, void* throwInformation));

CSF_FUNC(int, __cdecl, ___CxxFrameHandler, 0x007AF226,
         (void* exceptionRecord, void* frame, void* context,
          void* dispatcherContext));

// Exception handler data referenced by the original's C++ frame (0x8265E0 stub loads it).
CSF_GLOBAL(std::uint8_t, g_TwelveByteCopyExceptionMetadata, 0x0088790C);

// Copies `count` 12-byte elements into `destination`, each one constructed from
// the same `source` element via CopyConstructTwelveByteElement, inside the
// original's C++ try block. The catch-all rethrows the current exception.
// The helper's `this` argument (ecx) is ignored by the __stdcall callee, so it
// is not forwarded.
static int __cdecl CopyTwelveByteArrayWithContext(
    std::uint8_t* destination,
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
            result = CopyConstructTwelveByteElement(
                reinterpret_cast<std::uint32_t*>(destination), source);

            --count;
            destination += 12;
        }
    }
    catch (...)
    {
        RethrowCppException(nullptr, nullptr);
        __builtin_unreachable();
    }

    return static_cast<int>(reinterpret_cast<std::uintptr_t>(result));
}

HOOK(0x0044AE20, CopyTwelveByteArrayWithContext, "auto");
