#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

// Helper copies one 4-byte element. Packet bind line is authoritative.
CSF_FUNC(std::uint32_t*, __stdcall, CopyDwordIfNotNull, 0x00730830,
         (std::uint32_t* dest, const std::uint32_t* src));

CSF_FUNC(void, __stdcall, RethrowCppException, 0x007AF666,
         (void* exceptionObject, void* throwInformation));

CSF_FUNC(int, __cdecl, ___CxxFrameHandler, 0x007AF226,
         (void* exceptionRecord, void* frame, void* context,
          void* dispatcherContext));

// Exception handler data referenced by the original's C++ frame.
CSF_GLOBAL(std::uint8_t, g_DwordCopyExceptionMetadata, 0x00889D94);

// Copies `count` 4-byte elements starting at `destination`, each via
// CopyDwordIfNotNull(destination, source). `source` is not advanced;
// `destination` advances one dword per element. The loop runs inside the
// original's C++ try block; the catch-all rethrows the current exception.
static std::uint32_t* __cdecl CopyDwordArrayWithContext(
    std::uint32_t* destination,
    std::uint32_t count,
    const std::uint32_t* source,
    void* context)
{
    // Zero when count is zero (the original's EAX there is incidental).
    std::uint32_t* result = nullptr;

    try
    {
        while (count != 0)
        {
            result = CopyDwordIfNotNull(destination, source);

            --count;
            destination += 1;
        }
    }
    catch (...)
    {
        RethrowCppException(nullptr, nullptr);
        __builtin_unreachable();
    }

    return result;
}

HOOK(0x00484DD0, CopyDwordArrayWithContext, "auto");
