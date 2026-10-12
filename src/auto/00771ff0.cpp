#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

// Element helper used by the range loop: destination first, then source (both stdcall).
// The original also loads the copy context (4th argument) into ecx before the call;
// this binding follows the packet's bind line, so ecx is not passed.
CSF_FUNC(std::uint32_t*, __stdcall, CopyTwoDwordsIfDestNonNull,
         0x0071A2E0, (std::uint32_t* dest, const std::uint32_t* src));

// __CxxThrowException@8 - used to rethrow out of the catch(...) block.
CSF_FUNC(void, __stdcall, RethrowCppException,
         0x007AF666, (void* exceptionObject, void* throwInformation));

// Referenced by the SEH frame emitted for this function.
CSF_FUNC(int, __cdecl, ___CxxFrameHandler,
         0x007AF226, (void* exceptionRecord, void* frame, void* context, void* dispatcher));

// Exception information referenced by the frame's unwind/handler code.
CSF_GLOBAL(std::uint8_t, g_TwoDwordRangeExceptionInformation, 0x008940CC);

// Copies the range of two-dword elements [source, sourceEnd) to `destination`,
// invoking the element helper for every element. The fourth argument is a copy
// context that the original loads into ecx for the helper call.
// The fifth argument is never read by the original.
// Returns the end of the written destination range.
static std::uint32_t* __cdecl CopyTwoDwordRangeToDestination(
    std::uint32_t* source,
    std::uint32_t* sourceEnd,
    std::uint32_t* destination,
    std::uint32_t /*copyContext*/,
    std::uint32_t /*unused*/)
{
    try
    {
        while (source != sourceEnd)
        {
            CopyTwoDwordsIfDestNonNull(destination, source);
            destination += 2;
            source += 2;
        }
    }
    catch (...)
    {
        RethrowCppException(nullptr, nullptr);
    }

    return destination;
}

HOOK(0x00771FF0, CopyTwoDwordRangeToDestination, "auto");
