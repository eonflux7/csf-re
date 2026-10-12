#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

// Element helper used by the range loop: destination first, then source (both stdcall).
// The original also loads the copy context (4th argument) into ecx before the call;
// this binding follows the packet's bind line, so ecx is not passed.
CSF_FUNC(void, __stdcall, CopyThirtyTwoByteElement,
         0x006FA030, (void* dest, void* source));

// __CxxThrowException@8 - used to rethrow out of the catch(...) block.
CSF_FUNC(void, __stdcall, RethrowCppException,
         0x007AF666, (void* exceptionObject, void* throwInformation));

// Referenced by the SEH frame emitted for this function.
CSF_FUNC(int, __cdecl, ___CxxFrameHandler,
         0x007AF226, (void* exceptionRecord, void* frame, void* context, void* dispatcher));

// Exception information referenced by the frame's unwind/handler code.
CSF_GLOBAL(std::uint8_t, g_ThirtyTwoByteRangeExceptionInformation, 0x00886ED8);

// Walks the range of 32-byte elements [source, sourceEnd), invoking the element
// helper for every element and advancing the destination pointer in step.
// The original also passes a copy context (ecx) to the helper; that is not
// reproduced here, matching the helper's bind line.
// Returns the end of the written destination range.
static int __cdecl CopyThirtyTwoByteRange(
    std::uint32_t source,
    std::uint32_t sourceEnd,
    std::uint32_t destination,
    std::uint32_t /*copyContext*/)
{
    try
    {
        while (source != sourceEnd)
        {
            CopyThirtyTwoByteElement(
                reinterpret_cast<void*>(destination),
                reinterpret_cast<void*>(source));
            destination += 32u;
            source += 32u;
        }
    }
    catch (...)
    {
        RethrowCppException(nullptr, nullptr);
    }

    return static_cast<int>(destination);
}

HOOK(0x0043CE80, CopyThirtyTwoByteRange, "auto");
