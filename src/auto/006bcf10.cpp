#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

// Copies one 24-byte element. Packet bind line is authoritative.
CSF_FUNC(std::uint32_t*, __stdcall, InputBindings_CopyMenuElementIntoSlot, 0x007685C0,
         (std::uint32_t* dst, std::uint32_t* src));

CSF_FUNC(void, __stdcall, RethrowCppException, 0x007AF666,
         (void* exceptionObject, void* throwInformation));

CSF_FUNC(int, __cdecl, ___CxxFrameHandler, 0x007AF226,
         (void* exceptionRecord, void* frame, void* context,
          void* dispatcherContext));

// Exception frame data referenced by the original's C++ frame setup.
CSF_GLOBAL(std::uint8_t, g_MenuElementCopyExceptionMetadata, 0x008902D4);

// Copies `count` 24-byte elements starting at `destination`. Each element is
// copied by InputBindings_CopyMenuElementIntoSlot(destination, source). The
// loop runs inside the original's C++ try block; the catch-all rethrows.
static std::uint32_t* __cdecl CopyTwentyFourByteElementArray(
    void* destination,
    std::uint32_t count,
    void* source,
    void* context)
{
    // The original returns the last callee result (eax); zero when count is zero.
    std::uint32_t* result = nullptr;

    try
    {
        while (count != 0)
        {
            result = InputBindings_CopyMenuElementIntoSlot(
                static_cast<std::uint32_t*>(destination),
                static_cast<std::uint32_t*>(source));

            --count;
            destination = static_cast<std::uint8_t*>(destination) + 24;
        }
    }
    catch (...)
    {
        RethrowCppException(nullptr, nullptr);
        __builtin_unreachable();
    }

    return result;
}

HOOK(0x006BCF10, CopyTwentyFourByteElementArray, "auto");
