#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

// Copies one 12-byte element (CVector3-sized) from src to dst. The original also loads the
// copy context into ecx before the call; this binding follows the packet's bind line, so ecx is not passed.
CSF_FUNC(std::uint32_t*, __stdcall, CopyConstructTwelveByteElement,
         0x004A3200, (std::uint32_t* dst, std::uint32_t* src));

// __CxxThrowException@8 - used to rethrow out of the catch(...) block.
CSF_FUNC(void, __stdcall, RethrowCppException,
         0x007AF666, (void* exceptionObject, void* throwInformation));

// Referenced by the SEH frame emitted for this function.
CSF_FUNC(int, __cdecl, ___CxxFrameHandler,
         0x007AF226, (void* exceptionRecord, void* frame, void* context, void* dispatcher));

// Data referenced by the frame's handler funclet.
CSF_GLOBAL(std::uint8_t, g_TypeRangeExceptionInformation, 0x0088BD74);

// Copies the range of 12-byte elements [source, sourceEnd) to `destination`,
// invoking the element copy helper for every element.
// The fourth argument is the copy context the original passes in ecx to the helper;
// the fifth argument is never read by the original.
// Returns the end of the written destination range.
static std::uint32_t* __cdecl TypeRegistry_CopyTwelveByteRange(
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

HOOK(0x0054C240, TypeRegistry_CopyTwelveByteRange, "auto");
