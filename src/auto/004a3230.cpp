#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

// Element helper used by the range loop: destination first, then source (both stdcall).
// The original also loads the copy context (4th argument) into ecx before the call;
// this binding follows the packet's bind line, so ecx is not passed.
CSF_FUNC(std::uint32_t*, __stdcall, CopyConstructTwelveByteElement,
         0x004A3200, (std::uint32_t* destination, std::uint32_t* source));

// __CxxThrowException@8 - used to rethrow out of the catch(...) block.
CSF_FUNC(void, __stdcall, RethrowCppException,
         0x007AF666, (void* exceptionObject, void* throwInformation));

// Referenced by the SEH frame emitted for this function.
CSF_FUNC(int, __cdecl, ___CxxFrameHandler,
         0x007AF226, (void* exceptionRecord, void* frame, void* context, void* dispatcher));

// Exception information referenced by the frame's unwind/handler funclet (mov eax, 0x88a150).
CSF_GLOBAL(std::uint8_t, g_TwelveByteRangeExceptionInformation, 0x0088A150);

// Copies the range of 12-byte elements [source, sourceEnd) to `destination`,
// invoking the element helper for every element. The fourth argument is a copy
// context that the original passes to the helper in ecx.
// Returns the end of the written destination range.
static std::uint32_t* __cdecl CopyTwelveByteRange(
    std::uint32_t* source,
    std::uint32_t* sourceEnd,
    std::uint32_t* destination,
    std::uint32_t* /*copyContext*/)
{
    try
    {
        while (source != sourceEnd)
        {
            CopyConstructTwelveByteElement(destination, source);
            destination += 3;
            source += 3;
        }
    }
    catch (...)
    {
        RethrowCppException(nullptr, nullptr);
    }

    return destination;
}

HOOK(0x004A3230, CopyTwelveByteRange, "auto");
