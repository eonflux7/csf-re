#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

// Element copy helper used by the range loop: destination first, then source,
// both on the stack (stdcall, ret 8). The original also loads the copy context
// into ecx before the call (see notes); this binding follows the packet's bind line.
CSF_FUNC(std::uint32_t*, __stdcall, CopyRangeElement,
         0x00458500, (std::uint32_t* dest, std::uint32_t* src));

// __CxxThrowException@8 - used to rethrow out of the catch(...) block.
CSF_FUNC(void, __stdcall, RethrowCppException,
         0x007AF666, (void* exceptionObject, void* throwInformation));

// Referenced by the SEH funclet emitted for this function.
CSF_FUNC(int, __cdecl, ___CxxFrameHandler,
         0x007AF226, (void* exceptionRecord, void* frame, void* context, void* dispatcher));
CSF_GLOBAL(std::uint8_t, g_TwentyByteRangeExceptionInformation, 0x00887CC8);

// Copies the range of 20-byte elements [source, sourceEnd) to `destination`,
// invoking the element helper for every element. The fourth argument is a copy
// context passed through to the helper. Returns the end of the written destination range.
static int __cdecl CopyTwentyByteRangeToDestination(
    std::uint32_t source,
    std::uint32_t sourceEnd,
    std::uint32_t destination,
    std::uint32_t /*copyContext*/,
    std::uint32_t /*unused*/)
{
    try
    {
        while (source != sourceEnd)
        {
            CopyRangeElement(
                reinterpret_cast<std::uint32_t*>(destination),
                reinterpret_cast<std::uint32_t*>(source));
            destination += 20u;
            source += 20u;
        }
    }
    catch (...)
    {
        RethrowCppException(nullptr, nullptr);
    }

    return static_cast<int>(destination);
}

HOOK(0x004526F0, CopyTwentyByteRangeToDestination, "auto");
