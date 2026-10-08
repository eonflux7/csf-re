#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(std::uint32_t*, __fastcall, CopyMenuElementIntoSlot, 0x007685C0,
         (void* context, void* edx, std::uint32_t* destination,
          std::uint32_t* source));

CSF_FUNC(void, __stdcall, RethrowCppException, 0x007AF666,
         (void* exceptionObject, void* throwInformation));

CSF_FUNC(int, __cdecl, ___CxxFrameHandler, 0x007AF226,
         (void* exceptionRecord, void* frame, void* context,
          void* dispatcherContext));

// SEH metadata referenced by the original's C++ exception frame.
CSF_GLOBAL(std::uint8_t, g_TwentyFourByteCopyExceptionMetadata, 0x0088BA8C);

// Copies `count` 24-byte elements from `source` into `destination` using the
// context-based element copier. The whole loop runs inside a C++ try block
// whose catch-all rethrows the current exception.
static std::uint32_t* __cdecl CopyTwentyFourByteElements(
    std::uint32_t* destination,
    std::uint32_t count,
    std::uint32_t* source,
    void* context)
{
    // The original leaves an incidental EAX value when count is zero.
    std::uint32_t* result;

    try
    {
        while (count != 0)
        {
            result = CopyMenuElementIntoSlot(context, nullptr, destination, source);

            --count;
            destination = reinterpret_cast<std::uint32_t*>(
                reinterpret_cast<std::uintptr_t>(destination) + 24u);
        }
    }
    catch (...)
    {
        RethrowCppException(nullptr, nullptr);
        __builtin_unreachable();
    }

    return result;
}

HOOK(0x00549D70, CopyTwentyFourByteElements, "auto");
