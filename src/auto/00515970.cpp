#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

// Copies one 16-byte element. Packet bind line is authoritative.
CSF_FUNC(std::uint32_t*, __stdcall, CopySixteenByteElement, 0x006D9E80,
         (std::uint32_t* dest, std::uint32_t* source));

CSF_FUNC(void, __stdcall, RethrowCppException, 0x007AF666,
         (void* exceptionObject, void* throwInformation));

CSF_FUNC(int, __cdecl, ___CxxFrameHandler, 0x007AF226,
         (void* exceptionRecord, void* frame, void* context,
          void* dispatcherContext));

// Exception handler data referenced by the original's C++ frame.
CSF_GLOBAL(std::uint8_t, g_SixteenByteCopyExceptionMetadata, 0x0088AE9C);

// Copies `count` 16-byte elements starting at `destination`, each via
// CopySixteenByteElement(destination, source). The original passes
// `context` in ecx for every call. The loop runs inside the original's
// C++ try block; the catch-all rethrows the current exception.
static std::uint32_t* __cdecl CopySixteenByteArrayWithContext(
    std::uint32_t* destination,
    std::uint32_t count,
    std::uint32_t* source,
    void* context)
{
    std::uint32_t* result = nullptr;

    try
    {
        while (count != 0)
        {
            result = CopySixteenByteElement(destination, source);

            --count;
            destination += 4;
        }
    }
    catch (...)
    {
        RethrowCppException(nullptr, nullptr);
        __builtin_unreachable();
    }

    return result;
}

HOOK(0x00515970, CopySixteenByteArrayWithContext, "auto");
