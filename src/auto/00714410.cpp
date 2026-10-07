#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

// Element copy/construct helper used by the range loop (takes the destination
// element first, then the source element on the stack).
CSF_FUNC(std::uint32_t*, __stdcall, CopyConstructTwelveByteElement,
         0x004A3200, (std::uint32_t* destination, std::uint32_t* source));

// __CxxThrowException@8 - used to rethrow from the catch(...) block.
CSF_FUNC(void, __stdcall, RethrowCppException,
         0x007AF666, (void* exceptionObject, void* throwInformation));

// Referenced by the SEH funclet emitted for this function.
CSF_FUNC(int, __cdecl, HandleCppExceptionFrame, 0x007AF226, (...));
CSF_GLOBAL(std::uint8_t, g_TwelveByteRangeCopyExceptionInformation, 0x00891868);

// Copies a range of 12-byte elements from [source, sourceEnd) into
// `destination`, copy-constructing each element through the helper above.
// Returns the end of the written destination range.
static int __cdecl CopyConstructTwelveByteRange(
    std::uint32_t source,
    std::uint32_t sourceEnd,
    std::uint32_t destination,
    std::uint32_t /*constructionContext*/)
{
    try
    {
        while (source != sourceEnd)
        {
            CopyConstructTwelveByteElement(
                reinterpret_cast<std::uint32_t*>(destination),
                reinterpret_cast<std::uint32_t*>(source));
            destination += 12u;
            source += 12u;
        }
    }
    catch (...)
    {
        RethrowCppException(nullptr, nullptr);
    }

    return static_cast<int>(destination);
}

HOOK(0x00714410, CopyConstructTwelveByteRange, "auto");
