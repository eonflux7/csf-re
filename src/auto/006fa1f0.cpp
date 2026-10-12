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

// Updates one 20-byte element using the given parameter; returns the element pointer.
CSF_FUNC(std::uint32_t*, __stdcall, UpdateArrayElement, 0x00458500,
         (std::uint32_t* a1, std::uint32_t* a2));

CSF_FUNC(void, __stdcall, RethrowCppException, 0x007AF666,
         (void* exceptionObject, void* throwInformation));

CSF_FUNC(int, __cdecl, ___CxxFrameHandler, 0x007AF226,
         (void* exceptionRecord, void* frame, void* context,
          void* dispatcherContext));

// Exception handler data referenced by the original's C++ frame.
CSF_GLOBAL(std::uint8_t, g_UpdateArrayExceptionMetadata, 0x008913F4);

// Walks `count` elements of 20 bytes starting at `elements`, calling
// UpdateArrayElement on each with `param`, inside the original's C++ try block.
// The catch-all rethrows the current exception. The fourth stack argument is
// loaded by the original into ecx for the call, but the __stdcall callee ignores
// ecx, so it is not forwarded.
static int __cdecl ScreenEffects_UpdateElementArray(
    std::uint8_t* elements,
    std::uint32_t count,
    std::uint32_t* param,
    void* /*unusedContext*/)
{
    // The original leaves its last EAX value when count is zero; pass 0.
    std::uint32_t* result = nullptr;

    try
    {
        while (count != 0)
        {
            result = UpdateArrayElement(
                reinterpret_cast<std::uint32_t*>(elements), param);

            --count;
            elements += 20;
        }
    }
    catch (...)
    {
        RethrowCppException(nullptr, nullptr);
        __builtin_unreachable();
    }

    return static_cast<int>(reinterpret_cast<std::uintptr_t>(result));
}

HOOK(0x006FA1F0, ScreenEffects_UpdateElementArray, "auto");
