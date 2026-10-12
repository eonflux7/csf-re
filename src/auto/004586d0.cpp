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

// Updates one 5-dword element of an array; returns the callee's result pointer.
CSF_FUNC(std::uint32_t*, __stdcall, UpdateArrayElement, 0x00458500,
         (std::uint32_t* a1, std::uint32_t* a2));

CSF_FUNC(void, __stdcall, RethrowCppException, 0x007AF666,
         (void* exceptionObject, void* throwInformation));

CSF_FUNC(int, __cdecl, ___CxxFrameHandler, 0x007AF226,
         (void* exceptionRecord, void* frame, void* context,
          void* dispatcherContext));

// Exception frame data referenced by the original's C++ frame.
CSF_GLOBAL(std::uint8_t, g_UpdateArrayExceptionMetadata, 0x00888304);

// Applies UpdateArrayElement to `count` consecutive 20-byte elements starting at
// `elements`, inside the original's C++ try block. The catch-all rethrows the
// current exception. The helper's `this` argument (ecx) is ignored by the
// __stdcall callee, so it is not forwarded. Returns the last callee result.
static std::uint32_t* __cdecl UpdateArrayElementRange(
    std::uint32_t* elements,
    std::uint32_t count,
    std::uint32_t* arg3,
    std::uint32_t /*unusedThisArg*/,
    std::uint32_t /*unusedArg5*/)
{
    // The original leaves its last EAX value when count is zero; pass nullptr.
    std::uint32_t* result = nullptr;

    try
    {
        while (count != 0)
        {
            result = UpdateArrayElement(elements, arg3);

            --count;
            elements += 5; // 0x14 bytes per element
        }
    }
    catch (...)
    {
        RethrowCppException(nullptr, nullptr);
        __builtin_unreachable();
    }

    return result;
}

HOOK(0x004586D0, UpdateArrayElementRange, "auto");
