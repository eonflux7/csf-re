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

// Copies one dword pair from src to dest when dest is non-null; returns the destination pointer.
CSF_FUNC(std::uint32_t*, __stdcall, CopyTwoDwordsIfDestNonNull, 0x0071A2E0,
         (std::uint32_t* dest, const std::uint32_t* src));

CSF_FUNC(void, __stdcall, RethrowCppException, 0x007AF666,
         (void* exceptionObject, void* throwInformation));

CSF_FUNC(int, __cdecl, ___CxxFrameHandler, 0x007AF226,
         (void* exceptionRecord, void* frame, void* context,
          void* dispatcherContext));

// Exception handler data referenced by the original's C++ frame funclet.
[[maybe_unused]] CSF_GLOBAL(std::uint8_t, g_DwordPairCopyExceptionMetadata, 0x0088F2D4);

// Copies `count` dword pairs (8 bytes each) from `source` to consecutive destinations,
// each element via CopyTwoDwordsIfDestNonNull, inside the original's C++ try block.
// The catch-all rethrows the current exception. The original's stack arguments are
// (a1 = destination, a2 = count, a3 = source, a4 = ignored `this` for the __stdcall
// callee); the fifth Hex-Rays argument is never read by the original.
static int __cdecl CopyDwordPairArrayWithContext(
    std::uint32_t* destination,
    std::uint32_t count,
    const std::uint32_t* source,
    void* /*unusedContext*/)
{
    // The original leaves its last EAX value when count is zero; pass 0.
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

HOOK(0x00699180, CopyDwordPairArrayWithContext, "auto");
