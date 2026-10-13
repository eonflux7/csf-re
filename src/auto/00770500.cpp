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

// Copies one 12-byte element (three dwords) from src to dst; returns the destination pointer.
CSF_FUNC(std::uint32_t*, __stdcall, CopyConstructTwelveByteElement, 0x004A3200,
         (std::uint32_t* dst, std::uint32_t* src));

CSF_FUNC(void, __stdcall, RethrowCppException, 0x007AF666,
         (void* exceptionObject, void* throwInformation));

CSF_FUNC(int, __cdecl, ___CxxFrameHandler, 0x007AF226,
         (void* exceptionRecord, void* frame, void* context,
          void* dispatcherContext));

// Exception unwind data (FuncInfo) referenced by the original's C++ frame.
CSF_GLOBAL(std::uint8_t, g_TwelveByteCopyExceptionMetadata, 0x00893C20);

// Copies `count` 12-byte elements from `source` to `destination`, one element at a
// time, inside the original's C++ try block. The source pointer is not advanced; the
// destination advances by one element (three dwords) per iteration. The catch-all
// rethrows the current exception. The fourth argument is passed in ecx by the
// original to a __stdcall callee that ignores ecx, so it is not forwarded.
static std::uint32_t* __cdecl CopyTwelveByteArrayWithContext(
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

HOOK(0x00770500, CopyTwelveByteArrayWithContext, "auto");
