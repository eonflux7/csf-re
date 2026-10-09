#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

// Helper copies one dword. Packet bind line is authoritative.
CSF_FUNC(std::uint32_t*, __stdcall, CopyDwordIfNotNull, 0x00730830,
         (std::uint32_t* dest, const std::uint32_t* src));

CSF_FUNC(void, __stdcall, RethrowCppException, 0x007AF666,
         (void* exceptionObject, void* throwInformation));

CSF_FUNC(int, __cdecl, ___CxxFrameHandler, 0x007AF226,
         (void* exceptionRecord, void* frame, void* context,
          void* dispatcherContext));

// Exception handler data referenced by the original's C++ frame funclet.
CSF_GLOBAL(std::uint8_t, g_DwordCopyExceptionMetadata, 0x00889BD0);

// Copies `count` dwords starting at `destination`, each via
// CopyDwordIfNotNull(destination, source), advancing destination by 4 bytes
// per iteration. The loop runs inside the original's C++ try block; the
// catch-all rethrows the current exception.
static int __cdecl CopyDwordValueToArray(
    std::uint32_t* destination,
    std::uint32_t count,
    const std::uint32_t* source,
    void* context)
{
    // The original returns the last EAX value; with count == 0 it is
    // incidental, so pass 0.
    int result = 0;

    try
    {
        while (count != 0)
        {
            result = static_cast<int>(reinterpret_cast<std::uintptr_t>(
                CopyDwordIfNotNull(destination, source)));

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

HOOK(0x00480DF0, CopyDwordValueToArray, "auto");
