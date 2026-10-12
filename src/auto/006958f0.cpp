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

// Copies one dword from src to dest when dest is non-null; returns the destination pointer.
CSF_FUNC(std::uint32_t*, __stdcall, CopyDwordIfNotNull, 0x00730830,
         (std::uint32_t* dest, const std::uint32_t* src));

CSF_FUNC(void, __stdcall, RethrowCppException, 0x007AF666,
         (void* exceptionObject, void* throwInformation));

CSF_FUNC(int, __cdecl, ___CxxFrameHandler, 0x007AF226,
         (void* exceptionRecord, void* frame, void* context,
          void* dispatcherContext));

// Exception unwind/frame info referenced by the original's C++ frame prologue.
CSF_GLOBAL(std::uint8_t, g_CopyDwordsUnwindInfo, 0x0088F08C);

// Copies `count` dwords from `source` to `destination`, one CopyDwordIfNotNull
// call per element, inside the original's C++ try block. The catch-all rethrows
// the current exception. The helper's `this` argument (ecx) is ignored by the
// __stdcall callee, so it is not forwarded.
static int __cdecl CoreVariantValues_CopyDwordArray(
    std::uint32_t* destination,
    std::uint32_t count,
    const std::uint32_t* source,
    void* /*unusedContext*/,
    void* /*unusedArg*/)
{
    // The original leaves its last EAX value when count is zero; pass 0.
    std::uint32_t* result = nullptr;

    try
    {
        while (count != 0)
        {
            result = CopyDwordIfNotNull(destination, source);

            --count;
            ++destination;
        }
    }
    catch (...)
    {
        RethrowCppException(nullptr, nullptr);
        __builtin_unreachable();
    }

    return static_cast<int>(reinterpret_cast<std::uintptr_t>(result));
}

HOOK(0x006958F0, CoreVariantValues_CopyDwordArray, "auto");
