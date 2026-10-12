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

// Copies one 16-byte element from source to dest; returns the destination pointer.
CSF_FUNC(std::uint32_t*, __stdcall, CopySixteenByteElement, 0x006D9E80,
         (std::uint32_t* dest, std::uint32_t* source));

CSF_FUNC(void, __stdcall, RethrowCppException, 0x007AF666,
         (void* exceptionObject, void* throwInformation));

CSF_FUNC(int, __cdecl, ___CxxFrameHandler, 0x007AF226,
         (void* exceptionRecord, void* frame, void* context,
          void* dispatcherContext));

// Exception handler data referenced by the original's C++ frame.
CSF_GLOBAL(std::uint8_t, g_SixteenByteCopyExceptionMetadata, 0x00890978);

// Copies `count` 16-byte elements from the same source block into consecutive
// 16-byte slots starting at `destination`, each via CopySixteenByteElement,
// inside the original's C++ try block. The catch-all rethrows the current
// exception. The fourth argument is only loaded into ecx for the __stdcall
// callee, which ignores it, so it is not forwarded.
static std::uint32_t* __cdecl CopySixteenByteArrayWithContext(
    std::uint32_t* destination,
    std::uint32_t count,
    std::uint32_t* source,
    void* /*unusedContext*/)
{
    // The original leaves its last EAX value when count is zero; pass nullptr.
    std::uint32_t* result = nullptr;

    try
    {
        while (count != 0)
        {
            result = CopySixteenByteElement(destination, source);

            --count;
            destination += 4; // 4 dwords = 16 bytes per element
        }
    }
    catch (...)
    {
        RethrowCppException(nullptr, nullptr);
        __builtin_unreachable();
    }

    return result;
}

HOOK(0x006DA2F0, CopySixteenByteArrayWithContext, "auto");
