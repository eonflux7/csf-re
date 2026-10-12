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
CSF_GLOBAL(std::uint8_t, g_SixteenByteCopyExceptionMetadata, 0x0088C7D8);

// Copies `count` 16-byte elements from `source` to `destination`, calling
// CopySixteenByteElement(destination, source) for each element. The original
// passes its fourth argument in ecx for every call. The loop runs inside the
// original's C++ try block; the catch-all rethrows the current exception.
static int __cdecl MpRules_CopySixteenByteRecords(
    int destination,
    int count,
    int source,
    int context)
{
    // The original leaves an incidental EAX value when count is zero.
    int result = 0;

    try
    {
        while (count != 0)
        {
            result = static_cast<int>(reinterpret_cast<std::uintptr_t>(
                CopySixteenByteElement(
                    reinterpret_cast<std::uint32_t*>(destination),
                    reinterpret_cast<std::uint32_t*>(source))));

            --count;
            destination += 16;
        }
    }
    catch (...)
    {
        RethrowCppException(nullptr, nullptr);
        __builtin_unreachable();
    }

    return result;
}

HOOK(0x0055AB00, MpRules_CopySixteenByteRecords, "auto");
