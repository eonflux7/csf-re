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

// Copies one pair of dwords from src to dest when dest is non-null; returns the destination pointer.
CSF_FUNC(std::uint32_t*, __stdcall, CopyTwoDwordsIfDestNonNull, 0x0071A2E0,
         (std::uint32_t* dest, const std::uint32_t* src));

CSF_FUNC(void, __stdcall, RethrowCppException, 0x007AF666,
         (void* exceptionObject, void* throwInformation));

CSF_FUNC(int, __cdecl, ___CxxFrameHandler, 0x007AF226,
         (void* exceptionRecord, void* frame, void* context,
          void* dispatcherContext));

// Exception handler (FuncInfo) data referenced by the original's C++ frame.
CSF_GLOBAL(std::uint8_t, g_CopyTwoDwordsFuncInfo, 0x0089411C);

// Copies `count` dword pairs from `source` to `destination` (each element via
// CopyTwoDwordsIfDestNonNull), inside the original's C++ try block. The catch-all
// rethrows the current exception. The helper's `this` argument (ecx) is ignored
// by the __stdcall callee, so it is not forwarded.
static std::uint32_t* __cdecl CopyTwoDwordArrayFromSource(
    std::uint32_t* destination,
    std::uint32_t count,
    const std::uint32_t* source,
    void* /*unusedContext*/,
    void* /*unusedArg*/)
{
    // The original leaves the last EAX value when count is zero; pass nullptr.
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

    return result;
}

HOOK(0x00772060, CopyTwoDwordArrayFromSource, "auto");
